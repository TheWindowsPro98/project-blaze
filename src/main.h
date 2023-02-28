#pragma once
#include "includes.h"

extern VRAMRegion* options_vram;
extern s16 ind[12];

void mainscrn();
void sampleDefs();
void fadeInPalette(Palette* fadePalette, Palette* staticPalette, u16 bgColor, u8 fadeTime, bool async);