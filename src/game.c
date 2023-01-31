#include "includes.h"

Sprite* sPlayer;
int player_x = 0;
int player_y = 0;
u8 player_spd_x = 0;
u8 player_spd_y = 0;

static void gameInputHdl(u16 joy, u16 changed, u16 state)
{
    
}

static void playerPos()
{

}

void gametest()
{
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, palette_black, DMA);
    PAL_setPalette(PAL1, stephanie.palette->data, DMA_QUEUE_COPY);
    PAL_fadeIn(0,15,mainpal.data,30,TRUE);
    VDP_drawImageEx(BG_B,&options_bg,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind),0,0,FALSE,TRUE);
    XGM_startPlay(stg1);
    JOY_setEventHandler(&gameInputHdl);
}