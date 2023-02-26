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
u16 player_x = 16;
u16 player_y = 144;
s8 player_spd_x = 0;
s8 player_jump = 0;


static void playerJump()
{
    
}

static void gameInputHdl(u16 joy, u16 changed, u16 state)
{
    if (state & BUTTON_RIGHT)
    {
        SPR_setHFlip(sPlayer,FALSE);
        SPR_setAnim(sPlayer,1);
        player_spd_x = 1;
    }
    else if (state & BUTTON_LEFT)
    {
        SPR_setHFlip(sPlayer,TRUE);
        SPR_setAnim(sPlayer,1);
        player_spd_x = -1;
    }
    else
    {
        SPR_setAnim(sPlayer,0);
        player_spd_x = 0;
    }
    if (changed & state & BUTTON_C)
    {
        playerJump(FALSE);
    } 
    
}

static void playerPos()
{
    SPR_setPosition(sPlayer,player_x,player_y);
    player_x = player_x + player_spd_x;
}

void gametest()
{
    player_spd_x = 0;
    VRAM_free(&options_vram,ind);
    VRAM_clearRegion(&options_vram);
    VRAM_createRegion(&stg1_vram,TILE_USER_INDEX,16);
    ind = VRAM_alloc(&stg1_vram,16);
    PAL_setPalette(PAL0,palette_black,DMA);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_setPalette(PAL3,palette_black,DMA);
    PAL_fadeInAll(test_palette.data,30,TRUE);
    VDP_loadTileSet(&test_tiles,ind,DMA);
    VDP_setTileMapEx(BG_B,&test_map_bg,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind),0,0,0,0,64,28,DMA);
    VDP_setTileMapEx(BG_A,&test_map_fg,TILE_ATTR_FULL(PAL0,TRUE,FALSE,FALSE,ind),0,0,0,0,64,28,DMA);
    XGM_startPlay(testtrck2);
    sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
    JOY_setEventHandler(&gameInputHdl);
    while(1)
    {
        SPR_update();
        SYS_doVBlankProcess();
        XGM_nextFrame();
        playerPos();
    }
}