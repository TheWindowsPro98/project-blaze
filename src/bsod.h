#pragma once

#include "includes.h"

enum stopCodes {genericErr, lvlOutOfRange, z80Overload, featureNotFound};

void killExec(u32 stopcode);