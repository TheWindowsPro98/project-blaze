#include "includes.h"

Sprite* sPlayer;
VRAMRegion* stg1_vram;
VRAMRegion* stg2_vram;
VRAMRegion* stg3_vram;
VRAMRegion* stg4_vram;
VRAMRegion* stg5_vram;
VRAMRegion* stg6_vram;
VRAMRegion* stg7_vram;
VRAMRegion* stg8_vram;
s16 player_x = 0;
s16 player_y = 0;
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
    VRAM_free(&options_vram,0);
    VRAM_clearRegion(&options_vram);
    VRAM_createRegion(&stg1_vram,0,18);
    VRAM_alloc(&stg1_vram,18);
    PAL_setPalette(PAL0,palette_black,DMA);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_setPalette(PAL3,stephanie.palette->data,DMA);
    PAL_fadeIn(16,47,options_pal,30,TRUE);
    VDP_setTileMapEx(BG_B,&options_map,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,stg1_vram),0,0,0,0,64,28,DMA);
    XGM_startPlay(stg1);
    sPlayer = SPR_addSprite(&bars,16,72,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
    JOY_setEventHandler(&gameInputHdl);
    while(1)
    {
        SPR_update();
        SYS_doVBlankProcess();
        XGM_nextFrame();
    }
}