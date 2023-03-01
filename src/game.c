#include "includes.h"

#define NUM_OPTS_PAUSE 3
#define OPTX_PAUSE 0
#define OPTY_PAUSE 0

Sprite* sPlayer;
VRAMRegion* stg1_vram;
VRAMRegion* stg2_vram;
VRAMRegion* stg3_vram;
VRAMRegion* stg4_vram;
VRAMRegion* stg5_vram;
VRAMRegion* stg6_vram;
VRAMRegion* stg7_vram;
VRAMRegion* stg8_vram;
s16 player_x = 16;
s16 player_y = 144;
u16 cam_x = 0;
u16 cam_y = 0; 
s8 player_spd_x = 0;
s8 player_jump = 0;
bool paused = FALSE;
bool isJumping = FALSE;
bool isGrounded = TRUE;

Option menu_pause[NUM_OPTS_PAUSE] = {
    {OPTX_PAUSE, OPTY_PAUSE, "Resume"},
    {OPTX_PAUSE, OPTY_PAUSE+1, "Restart"},
    {OPTX_PAUSE, OPTY_PAUSE+2, "Quit Game"},
};

static void pauseChk()
{
    sampleDefs();
    switch (paused)
    {
    case FALSE:
    {
        XGM_startPlayPCM(93,0,SOUND_PCM_CH2);
        XGM_resumePlay();
        sPlayer->timer = 12;
        VDP_setHilightShadow(FALSE);
        JOY_setEventHandler(&gameInputHdl);
        break;
    }
    case TRUE:
    {
        XGM_startPlayPCM(90,0,SOUND_PCM_CH2);
        XGM_pausePlay();
        sPlayer->timer = 0;
        player_spd_x = 0;
        VDP_setHilightShadow(TRUE);
        JOY_setEventHandler(&pauseInputHdl);
        break;
    }
    default:
    {
        break;
    }
    }
}

static void spawnPlayer()
{
    switch (player_ci)
    {
        case 0:
        {
            PAL_setPalette(PAL0,lucy.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
        case 1:
        {
            PAL_setPalette(PAL0,stephanie.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
        case 2:
        {
            PAL_setPalette(PAL0,lucy.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
        case 3:
        {
            PAL_setPalette(PAL0,lucy.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
        case 4:
        {
            PAL_setPalette(PAL0,stephanie.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
        case 5:
        {
            PAL_setPalette(PAL0,stephanie.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
        default:
        {
            PAL_setPalette(PAL0,lucy.palette->data,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
            break;
        }
    }
}

static void playerJump()
{
    sampleDefs();
    if (isGrounded == TRUE)
    {
        if (isJumping == TRUE)
        {
            XGM_startPlayPCM(77,15,SOUND_PCM_CH2);
            isJumping = !isJumping;
        }
        else
        {
        }
    }
    else
    {
    }
}

static void pauseInputHdl(u16 joy, u16 changed, u16 state)
{
    if (changed & state & BUTTON_START)
    {
        paused = !paused;
        pauseChk();
    }
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
        isJumping = !isJumping;
        playerJump();
    }
    if (changed & state & BUTTON_START)
    {
        paused = !paused;
        pauseChk();
    }
    
}

static void playerPos()
{
    SPR_setPosition(sPlayer,player_x,player_y);
    player_x = player_x + player_spd_x;
    if (player_x <= 0)
    {
        player_x = 0;
        player_spd_x = 0;
    }
    else if (player_x >= 0x200)
    {
        player_x = 0;
    }
}

void gametest()
{
    player_spd_x = 0;
    VRAM_free(&options_vram,ind[2]);
    VRAM_clearRegion(&options_vram);
    VRAM_createRegion(&stg1_vram,TILE_USER_INDEX,16);;
    fadeInPalette(test_palette.data,palette_black,0x000,30,TRUE);
    ind[3] = VRAM_alloc(&stg1_vram,16);
    VDP_loadTileSet(&test_tiles,ind[3],DMA);
    VDP_setTileMapEx(BG_B,&test_map_bg,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[3]),0,0,0,0,64,28,DMA);
    VDP_setTileMapEx(BG_A,&test_map_fg,TILE_ATTR_FULL(PAL1,TRUE,FALSE,FALSE,ind[3]),0,0,0,0,64,28,DMA);
    XGM_startPlay(testtrck);
    spawnPlayer();
    PAL_setColor(0,0x000);
    JOY_setEventHandler(&gameInputHdl);
    while(1)
    {
        SPR_update();
        SYS_doVBlankProcess();
        XGM_nextFrame();
        playerPos();
    }
}