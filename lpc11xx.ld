
ENTRY(_vectors)

SECTIONS {
    . = 0;
    
    .vectors : ALIGN(16) SUBALIGN(16) {
        KEEP(*(.vectors))
    } > flash
    
    .init_array : ALIGN(4) SUBALIGN(4) {
        PROVIDE(_init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(_init_array_end = .);
    } > flash
    
    .text : ALIGN(4) SUBALIGN(4) {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.gcc*)
    } > flash
    
    . = ALIGN(4);
    
    PROVIDE(_data_rom = .);
    
    .data : {
        PROVIDE(_data_start = .);
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        PROVIDE(_data_end = .);
    } > ram AT > flash
    
    .bss : {
        PROVIDE(_bss_start = .);
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
    } > ram

    PROVIDE(_end = .);
    PROVIDE(_stacktop = ORIGIN(ram) + LENGTH(ram) - 16);
    PROVIDE(__dso_handle = 0);
}
