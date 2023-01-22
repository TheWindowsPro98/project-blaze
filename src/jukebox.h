#ifndef JUKEBOX_H
#define JUKEBOX_H
#include "opts.h"

#define NUM_OPTS_MUS 16
#define NUM_OPTS_SFX 27
#define OPTX_MUS 14
#define OPTY_MUS 5
#define OPTX_SFX 4
#define OPTY_SFX 4

Option menu_mus[NUM_OPTS_MUS]; 
void musPlayer();
void sfxPlayer();

#endif