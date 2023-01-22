#include <genesis.h>
#include "resources.h"
#include "game.h"

Sprite* sPlayer;

static void gameDPAD()
{
    u16 value = JOY_readJoypad(JOY_1);
}

static void gameBtn(u16 joy,u16 changed,u16 state)
{
    
}

void gametest()
{
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, palette_black, DMA);
    PAL_setPalette(PAL1, stephanie.palette->data, DMA_QUEUE_COPY);
    PAL_fadeIn(0,15,mainpal.data,30,TRUE);
    XGM_startPlay(stg1);
    VDP_drawImageEx(BG_B,&options_bg,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind),0,0,FALSE,TRUE);
    sPlayer = SPR_addSprite(&stephanie, 0,56,TILE_ATTR(PAL1,TRUE,FALSE,FALSE));
    JOY_setEventHandler(gameBtn);
}