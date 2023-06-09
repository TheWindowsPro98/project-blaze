#pragma once

#include "includes.h"

extern const char playerNames[7][10];
extern fix16 health;
extern Sprite* healthbar;

static void pauseInputHdl(u16 joy, u16 changed, u16 state);
static void gameInputHdl(u16 joy, u16 changed, u16 state);
u16 sPosToTPos(u8 basePos);
void gameInit();