#pragma once
#include "includes.h"

extern VRAMRegion* options_vram;
extern s16 ind;

void mainscrn();
void sampleDefs();
void fadeInPalette(Palette* palette, u8 fadeTime, bool async);