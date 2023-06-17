#pragma once

#include "includes.h"

extern const char playerNames[7][10];
extern fix16 health;
extern Sprite* healthbar;

static void pauseInputHdl(u16 joy, u16 changed, u16 state);
static void gameInputHdl(u16 joy, u16 changed, u16 state);

/// @brief Converts 8*8 tile position to sprite position.
/// @param basePos Position per 8*8 tile
/// @return Position in pixels (tile position * 8)
u16 sPosToTPos(u8 basePos);

void gameInit();