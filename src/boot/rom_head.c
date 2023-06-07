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
    "(C)RD23 2023.MAY", // Copyright date
    "PROJECT BLAZE           MEGADRIVE BUILD 20230606", // JAP/EUR Cart Name
    "PROJECT BLAZE           GENESIS   BUILD 20230606", // USA Cart Name
    "GM 62979736-05", // Build Version
    0x0000, // Checksum number
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
    "268 days on the clock; RD23 = TWP98     ", // Notes (not processed)
    "JU              " // Country
};