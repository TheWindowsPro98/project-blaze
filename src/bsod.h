#pragma once

#include "includes.h"

enum stopCodes {genericErr, lvlOutOfRange, z80Overload};

void killExec(u32 stopcode);