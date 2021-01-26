MEMORY {
    flash (rx) : org = 0x00000000, len = 32k
    ram (rwx)  : org = 0x10000100, len = 8k - 256
}

INCLUDE lpc11xx.ld
