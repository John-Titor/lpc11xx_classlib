MEMORY {
    flash (rx) : org = 0x00000000, len = 32k
    ram (rwx)  : org = 0x10000000, len = 4k
}

INCLUDE lpc11xx.ld
