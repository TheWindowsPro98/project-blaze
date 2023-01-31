#include "genesis.h"

__attribute__((externally_visible))
const ROMHeader rom_header = {
#if (ENABLE_BANK_SWITCH != 0)
    "SEGA SSF        ",
#elif (MODULE_MEGAWIFI != 0)
    "SEGA MEGAWIFI   ",
#else
    "SEGA GENESIS    ", // Needed, or else cart won't boot
#endif
    "(C)TWP98 2017-22", // Copyright date
    "PROJECT BLAZE           MEGADRIVE BUILD 20230130", // JUE/EUR Cart Name
    "PROJECT BLAZE           GENESIS   BUILD 20230130", // USA Cart Name
    "GM 00000001-05", // Build Version
    0x000,
    "JD              ", // Joypads
    0x00000000,
#if (ENABLE_BANK_SWITCH != 0)
    0x003FFFFF,
#else
    0x000FFFFF,
#endif
    0xE0FF0000,
    0xE0FFFFFF,
    "RA",
    0xF820,
    0x00200000,
    0x0020FFFF,
    "            ",
    "80 days on the clock; hit the damn gas! ", // Notes (not processed)
    "USA             " // Country
};
