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
    "(C)TWP  2023.FEB", // Copyright date
    "PROJECT BLAZE           MEGADRIVE BUILD 20230225", // JAP/EUR Cart Name
    "PROJECT BLAZE           GENESIS   BUILD 20230225", // USA Cart Name
    "GM 62979736-05", // Build Version
    0x000,
    "J6              ", // Joypads
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
    "54 days on the clock; still no game     ", // Notes (not processed)
    "JU              " // Country
};