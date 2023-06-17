#include "genesis.h"

__attribute__((externally_visible))
const ROMHeader rom_header = {
#if (ENABLE_BANK_SWITCH != 0)
    "SEGA SSF        ",
#elif (MODULE_MEGAWIFI != 0)
    "SEGA MEGAWIFI   ",
#else
    "SEGA MEGA DRIVE ", // Needed, or else cart won't boot
#endif
    "(C)RD23 2023.JUN", // Copyright date
    "PROJECT BLAZE           MEGADRIVE BUILD 20230616", // JAP/EUR Cart Name
    "PROJECT BLAZE           GENESIS   BUILD 20230616", // USA Cart Name
    "GM 62979736-06", // Build Version
    0x0000, // Checksum number
    "J6              ", // Joypads
    0x00000000, // ROM start
#if (ENABLE_BANK_SWITCH != 0)
    0x003FFFFF, // ROM end (bank-switching)
#else
    0x000FFFFF, // ROM end (no bank-switching)
#endif
    0xE0FF0000, // RAM start
    0xE0FFFFFF, // RAM end
    "RA",
    0xF820,
    0x00200000, // SRAM start
    0x0020FFFF, // SRAM end
    "            ",
    "258 days on the clock; holy hell        ", // Notes (not processed)
    "JU              " // Country
};