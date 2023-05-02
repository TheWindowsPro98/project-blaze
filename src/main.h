#pragma once
#include "includes.h"

enum consoleRegions {ntscUSA = 0xA0, ntscJPN = 0x20, palEUR = 0xE0, palJPN = 0x60};

extern VRAMRegion* options_vram;
extern u16 ind[12];
extern u16 uncPal[64];

void mainscrn();
void fadeInPalette(Palette* fadePalette, Palette* staticPalette, u8 fadeTime, bool async);