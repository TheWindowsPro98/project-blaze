#pragma once
#include "includes.h"

enum consoleRegions {ntscUSA = 0xA0, ntscJPN = 0x20, palEUR = 0xE0, palJPN = 0x60};

extern VRAMRegion* options_vram;
extern s16 ind;

void mainscrn();
void fadeInPalette(u16 fadePalette[48], u16 staticPalette[16], u8 fadeTime, bool async);
u8 getConsoleRegion();