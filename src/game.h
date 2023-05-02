#pragma once

static void pauseInputHdl(u16 joy, u16 changed, u16 state);
static void gameInputHdl(u16 joy, u16 changed, u16 state);
void killExec(const char err[]);
void gameInit();