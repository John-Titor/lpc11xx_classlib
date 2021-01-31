// Copyright (c) 2021 Michael Smith, All Rights Reserved
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  o Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  o Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//

/*
 * C_CAN ROM wrappers.
 *
 * We use one message object for transmission (0).
 * By default, object 1 is configured as a wildcard for reception; to
 * narrow reception with filters, it should be turned off.
 */

#ifdef LPC11C24FBD48

#include <config.h>

#include <stdint.h>
#include <etl/queue_spsc_atomic.h>
#include <etl/atomic.h>

#include <can_rom.h>
#include <debug.h>
#include <interrupt.h>
#include <syscon.h>

namespace
{
static const uint8_t    tx_msgobj = 0;
static const uint8_t    rx_std_msgobj = 1;
static const uint8_t    rx_ext_msgobj = 2;

////////////////////////////////////////////////////////////////////////////////
// ROM API
//
struct CAN_MSG_OBJ {
    enum Mode : uint32_t {
        STD = 0x00000000,
        EXT = 0x20000000,
        DAT = 0x00000000,
        RTR = 0x40000000,
    };
    uint32_t    mode_id;
    uint32_t    mask;
    uint8_t     data[8];
    uint8_t     dlc;
    uint8_t     msgobj;
};

struct CAN_CALLBACKS {
    void (*CAN_rx)(uint8_t msg_obj);
    void (*CAN_tx)(uint8_t msg_obj);
    void (*CAN_error)(uint32_t error_info);
    uint32_t (*CANOPEN_sdo_read)(uint16_t index, uint8_t subindex);
    uint32_t (*CANOPEN_sdo_write)(uint16_t index, uint8_t subindex, uint8_t *dat_ptr);
    uint32_t (*CANOPEN_sdo_seg_read)(uint16_t index, uint8_t subindex, uint8_t openclose, uint8_t *length, uint8_t *data, uint8_t *last);
    uint32_t (*CANOPEN_sdo_seg_write)(uint16_t index, uint8_t subindex, uint8_t openclose, uint8_t length, uint8_t *data, uint8_t *fast_resp);
    uint8_t (*CANOPEN_sdo_req)(uint8_t length_req, uint8_t *req_ptr, uint8_t *length_resp, uint8_t *resp_ptr);
};

struct CAN_rom_table {
    void (*init_can)(const uint32_t *can_cfg, uint8_t isr_ena);
    void (*isr)(void);
    void (*config_rxmsgobj)(const CAN_MSG_OBJ *msg_obj);
    uint8_t (*can_receive)(CAN_MSG_OBJ *msg_obj);
    void (*can_transmit)(const CAN_MSG_OBJ *msg_obj);
    void (*config_canopen)(void /*CAN_CANOPENCFG*/ * canopen_cfg);
    void (*canopen_handler)(void);
    void (*config_calb)(const CAN_CALLBACKS *callback_cfg);
};

// You're supposed to look this up through several layers of pointer indirection,
// but why? only one ROM version AFAIK, so let's just hardcode it. Could probably
// do the same for the functions themselves...
auto rom_table = (const CAN_rom_table *const)0x1fff334c;

////////////////////////////////////////////////////////////////////////////////
// receive callback & buffering
//

etl::queue_spsc_atomic<CAN_ROM::Message,
    CONFIG_CAN_RX_QUEUE_SIZE,
    etl::memory_model::MEMORY_MODEL_SMALL>   rx_queue;

static void
CAN_rx(uint8_t msg_obj)
{
    if ((msg_obj == rx_std_msgobj)
            || (msg_obj == rx_ext_msgobj)) {
        // get the received message
        CAN_MSG_OBJ rx_obj = { .msgobj = msg_obj };
        rom_table->can_receive(&rx_obj);

        // try to queue it (silent loss if no space)
        CAN_ROM::Message msg = {
            // extract 29- or 11-bit ID
            .id = (rx_obj.mode_id & CAN_MSG_OBJ::EXT) ? (rx_obj.mode_id & 0x1fffffff) : (rx_obj.mode_id & 0x7ff),
            .extended = (rx_obj.mode_id & CAN_MSG_OBJ::EXT) ? 1U : 0U,
            .rtr = (rx_obj.mode_id & CAN_MSG_OBJ::RTR) ? 1U : 0U,
            .dlc = rx_obj.dlc,
            .data = {
                rx_obj.data[0], rx_obj.data[1], rx_obj.data[2], rx_obj.data[3],
                rx_obj.data[4], rx_obj.data[5], rx_obj.data[6], rx_obj.data[7]
            },
        };
        rx_queue.push(msg);
    }
}

////////////////////////////////////////////////////////////////////////////////
// transmit callback & buffering
//

etl::queue_spsc_atomic<CAN_ROM::Message,
    CONFIG_CAN_TX_QUEUE_SIZE,
    etl::memory_model::MEMORY_MODEL_SMALL>   tx_queue;
etl::atomic<bool> tx_in_progress;

static void
CAN_tx(uint8_t msg_obj)
{
    // if there's another message ready to go for slot 1, send it
    if (msg_obj == tx_msgobj) {
        CAN_ROM::Message msg;

        if (tx_queue.pop(msg)) {
            CAN_MSG_OBJ tx_obj = {
                .mode_id = (msg.id
                            | (msg.extended ? CAN_MSG_OBJ::EXT : 0)
                            | (msg.rtr ? CAN_MSG_OBJ::RTR : 0)),
                .mask = 0,
                .data = {
                    msg.data[0], msg.data[1], msg.data[2], msg.data[3],
                    msg.data[4], msg.data[5], msg.data[6], msg.data[7]
                },
                .dlc = msg.dlc,
                .msgobj = tx_msgobj,
            };
            rom_table->can_transmit(&tx_obj);
            tx_in_progress = true;
        } else {
            tx_in_progress = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// error handling
//

etl::atomic<bool> error_need_reinit;

enum ErrorBits : uint32_t {
    CAN_ERROR_NONE  = 0x00000000,
    CAN_ERROR_PASS  = 0x00000001,
    CAN_ERROR_WARN  = 0x00000002,
    CAN_ERROR_BOFF  = 0x00000004,
    CAN_ERROR_STUF  = 0x00000008,
    CAN_ERROR_FORM  = 0x00000010,
    CAN_ERROR_ACK   = 0x00000020,
    CAN_ERROR_BIT1  = 0x00000040,
    CAN_ERROR_BIT0  = 0x00000080,
    CAN_ERROR_CRC   = 0x00000100
};

static void
CAN_error(uint32_t error_info)
{
    if (error_info & CAN_ERROR_BOFF) {
        error_need_reinit = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
// (re)initialization
//

CAN_ROM::Bitrate current_bitrate;

// http://www.bittiming.can-wiki.info
const uint32_t rate_tab[][2] = {
    // CANCLKDIV    CAN_BTR
    { 0x0, 0x1c1d }, // BR_100000
    { 0x0, 0x1c17}, // BR_125000
    { 0x0, 0x1c0b}, // BR_250000
    { 0x0, 0x1c05}, // BR_500000
    { 0x0, 0x1c02}, // BR_1000000
};

void
CAN_reinit()
{
    // TODO: Maybe should disable the CAN interrupt here, though if we are in
    // bus-off we should not be seeing any tx interrupts...

    // Note: We might have been trying to send a message, and we have probably
    // lost it if so.

    // cancel any in-progress transmissions
    tx_in_progress = false;
    tx_queue.clear();

    // clear any need-reset indication from the error callback
    error_need_reinit = false;

    // look up bitrate & compute BTR numbers
    rom_table->init_can(&rate_tab[current_bitrate][0], 1);
}

};

namespace CAN_ROM
{
void
init(Bitrate bitrate)
{
    SYSCON_CAN.clock(true);

    // set current bitrate and kick off init
    current_bitrate = bitrate;
    CAN_reinit();

    // publish callback table
    static const CAN_CALLBACKS cbtab = { .CAN_rx = CAN_rx, .CAN_tx = CAN_tx, .CAN_error = CAN_error };
    rom_table->config_calb(&cbtab);

    // Set up two wildcard receive objects; one for regular and one for
    // extended messages. This seems to be the only way to receive
    // everything.
    set_filter(rx_std_msgobj, 0, CAN_MSG_OBJ::EXT);
    set_filter(rx_ext_msgobj, CAN_MSG_OBJ::EXT, CAN_MSG_OBJ::EXT);

    // enable interrupts (ROM might do this already?)
    CAN_IRQ.enable();
}

bool send(const Message &msg)
{
#if 0

    // are we in an error state that will prevent us from transmitting?
    if (error_need_reinit) {
        CAN_reinit();
    }

#endif

    while (!tx_queue.push(msg)) {
    }

    // is the sender still working on something?
    if (!tx_in_progress) {
        // fake a tx-done interrupt for object #1
        CAN_tx(tx_msgobj);
    }

    return true;
}

bool recv(Message &msg)
{
    return rx_queue.pop(msg);
}

bool recv_available()
{
    return !rx_queue.empty();
}

bool send_space()
{
    return !tx_queue.full();
}

bool set_filter(uint8_t filter_index,
                uint32_t id_bits,
                uint32_t mask_bits)
{
    if ((filter_index >= 2) && (filter_index <= 32)) {
        CAN_MSG_OBJ filter = {
            .mode_id = id_bits,
            .mask = mask_bits,
            .msgobj = filter_index,
        };
        rom_table->config_rxmsgobj(&filter);
        return true;
    }

    return false;
}
}

// Interrupt thunk
void
CAN_Handler()
{
    rom_table->isr();
}

#endif // LPC11C24FBD48