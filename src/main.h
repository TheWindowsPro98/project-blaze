#pragma once
#include "includes.h"

enum consoleRegions {ntscUSA = 0xA0, ntscJPN = 0x20, palEUR = 0xE0, palJPN = 0x60};

extern VRAMRegion* options_vram;
extern s16 ind;

void mainscrn();

/// @brief Fade in all palette entries.
/// @param fadePalette First 48 entries (usually function-specific palette)
/// @param staticPalette Last 16 entries (usually player/text palette)
/// @param fadeTime Time to complete fade (in frames)
/// @param async Do async process
void fadeInPalette(u16 fadePalette[48], u16 staticPalette[16], u8 fadeTime, bool async);

/// @brief Gets the console's region.
/// @return The console's region.
u8 getConsoleRegion();