#include "includes.h"

enum hudPos {livesStrX = 19, scoreStrX = 12, fpsStrX = 19};
enum playerAnim {idleAnim, walkAnim, jumpAnim};
enum camScrlBounds {leftCamBnd = 95, rightCamBnd = 96, highCamBnd = 48, lowCamBnd = 128};
enum vdpViewport {horzRes = 320, vertRes = 224};
enum playerSize {playerWidth = 16, playerHeight = 16};

Sprite* sPlayer;
Sprite* healthbar[17];
Sprite* icon;
Map* lvlBG;
Map* lvlFG;
VRAMRegion* stg1_vram;
VRAMRegion* stg2_vram;
VRAMRegion* stg3_vram;
VRAMRegion* stg4_vram;
VRAMRegion* stg5_vram;
VRAMRegion* stg6_vram;
VRAMRegion* stg7_vram;
VRAMRegion* stg8_vram;
VRAMRegion* bsod_vram;

fix32 player_x = FIX32(0);
fix32 player_y = FIX32(352);
fix32 player_spd_x;
fix32 player_spd_y;
fix32 player_jump;
bool paused = FALSE;
bool isJumping = FALSE;
bool isGrounded = TRUE;
u8 health = 128;
const u32 mapWidths[lvlMax] = 
{
    0,0,0,0,0,0,0,1280
};
const u32 mapHeights[lvlMax] = 
{
    0,0,0,0,0,0,0,448
};
s16 new_cam_x;
s16 new_cam_y;
const char playerNames[7][10] = {"Jade","Stephanie","Emma","Selina","Alexia","Christina","Vanellope"};

static u8 sPosToTPos(u8 basePos)
{
    return basePos * 8;
}


static void sprDealloc()
{
    SPR_end();
    SPR_init();
}


static void pauseChk()
{
    XGM_setPCM(64,back_xgm,sizeof(back_xgm));
    XGM_setPCM(65,sel_xgm,sizeof(sel_xgm));
    switch (paused)
    {
    case FALSE:
    {
        XGM_startPlayPCM(64,0,SOUND_PCM_CH2);
        XGM_resumePlay();
        sPlayer->timer = 12;
        PAL_getColors(0,uncPal,64);
        for (u8 i = 0; i < 64; i++)
        {
            PAL_setColor(i,uncPal[i]*2);
        }
        JOY_setEventHandler(&gameInputHdl);
        break;
    }
    case TRUE:
    {
        XGM_startPlayPCM(65,0,SOUND_PCM_CH2);
        XGM_pausePlay();
        sPlayer->timer = 0;
        player_spd_x = 0;
        player_spd_y = 0;
        PAL_getColors(0,uncPal,64);
        for (u8 i = 0; i < 64; i++)
        {
            PAL_setColor(i,uncPal[i]/2);
        }
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
            aplib_unpack(lucy_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,fix16ToInt(player_x),fix16ToInt(player_y),TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 1:
        {
            aplib_unpack(stephanie_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 2:
        {
            aplib_unpack(emma_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 3:
        {
            aplib_unpack(selina_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 4:
        {
            aplib_unpack(alexia_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 5:
        {
            aplib_unpack(christina_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        default:
        {
            aplib_unpack(emma_pal,uncPal);
            PAL_setPalette(PAL3,uncPal,DMA);
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
    }
}

static void joyEvent_BSOD(u16 joy, u16 changed, u16 state)
{
    if (changed & state & BUTTON_START)
    {
        SYS_hardReset();
    }
}

void killExec(char err[39])
{
    for (u8 i = 0; i <= 3; i++)
    {
        PAL_setPalette(i,palette_black,DMA);
    }
    VRAM_createRegion(&bsod_vram,TILE_USER_INDEX,32);
    ind[12] = VRAM_alloc(&bsod_vram,32);
    aplib_unpack(bsod_palette,uncPal);
    PAL_fadeIn(0,31,uncPal,30,TRUE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_clearPlane(WINDOW,TRUE);
    VDP_setWindowVPos(FALSE,0);
    SPR_end();
    XGM_stopPlay();
    PSG_reset();
    u8 consoleRegion = *(u8 *)0xA10001;
    VDP_drawImageEx(BG_A,&bsod_frown,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[12]),1,1,FALSE,TRUE);
    if (consoleRegion == ntscUSA)
    {
        VDP_drawTextEx(BG_A,"Your Genesis has ran into a problem",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,9,DMA);
    }
    else
    {
        VDP_drawTextEx(BG_A,"Your Megadrive has ran into a problem",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,9,DMA);
    }
    VDP_drawTextEx(BG_A,"and needs to restart. We're just",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,10,DMA);
    VDP_drawTextEx(BG_A,"collecting some error info, and then",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,11,DMA);
    VDP_drawTextEx(BG_A,"You can press START to restart.",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,12,DMA);
    VDP_drawTextEx(BG_A,"If you would like to know more, here",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,14,DMA);
    VDP_drawTextEx(BG_A,"is the error in question:",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,15,DMA);
    VDP_drawTextEx(BG_A,err,TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,17,DMA);
    XGM_setPCM(64,testxgm,sizeof(testxgm));
    XGM_startPlayPCM(64,15,SOUND_PCM_CH1);
    JOY_setEventHandler(joyEvent_BSOD);
    while (1)
    {
        SYS_doVBlankProcess();
        XGM_nextFrame();
    }
}

static void spawnHUD()
{
    u8 i = 0;
    for (i; i < 16; i++)
    {
        healthbar[i] = SPR_addSprite(&bars,sPosToTPos(2+i),sPosToTPos(1),TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
        SPR_setAnimAndFrame(healthbar[i],1,8);
    }
    healthbar[17] = SPR_addSprite(&bars,sPosToTPos(2+i),sPosToTPos(1),TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
    SPR_setAnim(healthbar[17],3);
    VDP_setWindowVPos(FALSE,2);
    if (player_ci > 5)
    {
        player_ci = 6;
    }
    VDP_drawTextEx(WINDOW,playerNames[player_ci],TILE_ATTR(PAL3,TRUE,FALSE,FALSE),2,0,DMA);
    icon = SPR_addSprite(&plr_icns,0,0,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
    SPR_setAnim(icon,player_ci);
    VDP_drawTextEx(WINDOW,"{",TILE_ATTR(PAL3,TRUE,FALSE,FALSE),scoreStrX-1,0,DMA);
}

static void playerJump()
{
    XGM_setPCM(64,jump_sfx,sizeof(jump_sfx));
    if (isGrounded == TRUE)
    {
        if (isJumping == TRUE)
        {
            XGM_startPlayPCM(64,15,SOUND_PCM_CH2);
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
    if (state & BUTTON_LEFT)
    {
        SPR_setAnim(sPlayer,walkAnim);
        SPR_setHFlip(sPlayer,TRUE);
        player_spd_x = FIX32(-1);
    }
    else if (state & BUTTON_RIGHT)
    {
        SPR_setAnim(sPlayer,walkAnim);
        SPR_setHFlip(sPlayer,FALSE);
        player_spd_x = FIX32(1);
    }
    else
    {
        SPR_setAnim(sPlayer,idleAnim);
        player_spd_x = FIX32(0);
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

static void camPos()
{
    s16 px = fix32ToInt(player_x);
    s16 py = fix32ToInt(player_y);
    s16 cam_x;
    s16 cam_y;
    s16 scrn_x = px - cam_x;
    s16 scrn_y = py - cam_y;
    if (scrn_x > rightCamBnd)
    {
        new_cam_x = px - rightCamBnd;
    }
    else if (scrn_x < leftCamBnd)
    {
        new_cam_x = px - leftCamBnd;
    }
    else
    {
        new_cam_x = cam_x;
    }
    if (scrn_y > lowCamBnd)
    {
        new_cam_y = py - lowCamBnd;
    }
    else if (scrn_y < highCamBnd)
    {
        new_cam_y = py - highCamBnd;
    }
    else
    {
        new_cam_y = cam_y;
    }
    if (new_cam_x < 0)
    {
        new_cam_x = 0;
    }
    else if (new_cam_x > (mapWidths[round-1] - horzRes))
    {
        new_cam_x = mapWidths[round-1] - horzRes;
    }
    if (new_cam_y < 0)
    {
        new_cam_y = 0;
    }
    else if (new_cam_y > (mapHeights[round-1] - vertRes))
    {
        new_cam_y = mapHeights[round-1] - vertRes;
    }
    if ((cam_x != new_cam_x) || (cam_y != new_cam_y))
    {
        cam_x = new_cam_x;
        cam_y = new_cam_y;
    }
    MAP_scrollTo(lvlFG,cam_x,cam_y);
    MAP_scrollTo(lvlBG,cam_x/2,cam_y/2);
}

static void playerPos()
{
    SPR_setPosition(sPlayer,fix32ToInt(player_x) - new_cam_x,fix32ToInt(player_y) - new_cam_y);
    player_x += player_spd_x;
    player_y += player_spd_y;
    if (player_x < FIX32(0))
    {
        player_x = FIX32(0);
    }
    else if (player_x > FIX32(mapWidths[round-1] - playerWidth))
    {
        player_x = FIX32(mapWidths[round-1] - playerWidth);
    }
    if (player_y < FIX32(0))
    {
        player_y = FIX32(0);
    }
    else if (player_y > FIX32(mapHeights[round-1] - playerHeight))
    {
        player_y = FIX32(mapHeights[round-1] - playerHeight);
    }
}

static void drawIntToHex(s32 value, const char destStr, u8 minChr, u8 x, u8 y)
{
    intToHex(value, destStr, minChr);
    VDP_drawTextEx(WINDOW,destStr,TILE_ATTR(PAL3,TRUE,FALSE,FALSE),x,y,DMA);
}

static void updateHUD()
{
    char livesStr[3] = "05";
    char scoreStr[7] = "000000";
    char z80Str[3] = "00";
    char pxStr[3], pyStr[3], cxStr[3], cyStr[3];
    char pauseStr[3] = "00";
    char healthStr[3] = "00";
    char jmpStr[3] = "00";
    char gndStr[3] = "00";
    u8 z80ld = XGM_getCPULoad();
    if (z80ld > 100)
    {
        Z80_unloadDriver();
        Z80_loadDriver(Z80_DRIVER_XGM,TRUE);
        switch (round)
        {
        case 8:
        {
            XGM_startPlay(testtrck);
            break;
        }
        default:
        {
            XGM_setPCM(64,testxgm,sizeof(testxgm));
            XGM_startPlayPCM(64,15,SOUND_PCM_CH1);
            break;
        }
        }
    }
    s16 px = fix32ToInt(player_x);
    s16 py = fix32ToInt(player_y);
    u16 mem = MEM_getFree();
    u8 sprAmnt = SPR_getNumActiveSprite();
    intToStr(sprAmnt,livesStr,2);
    VDP_drawTextEx(WINDOW,livesStr,TILE_ATTR(PAL3,TRUE,FALSE,FALSE),livesStrX,1,DMA);
    intToStr(mem,scoreStr,6);
    VDP_drawTextEx(WINDOW,scoreStr,TILE_ATTR(PAL3,TRUE,FALSE,FALSE),scoreStrX,0,DMA);
    drawIntToHex(z80ld,z80Str,2,fpsStrX,0);
    drawIntToHex(px,pxStr,4,fpsStrX+3,0);
    drawIntToHex(py,pyStr,4,fpsStrX+7,0);
    drawIntToHex(paused,pauseStr,2,fpsStrX+3,1);
    drawIntToHex(isJumping,jmpStr,2,fpsStrX+5,1);
    drawIntToHex(isGrounded,gndStr,2,fpsStrX+7,1);
    drawIntToHex(health,healthStr,2,fpsStrX+9,1);
}

static void drawLevel()
{
    switch (round)
    {
    case 8:
    {
        VRAM_free(&options_vram,ind[2]);
        VRAM_clearRegion(&options_vram);
        VRAM_createRegion(&stg1_vram,TILE_USER_INDEX,192);
        aplib_unpack(test_palette,uncPal);
        ind[3] = VRAM_alloc(&stg1_vram,192);
        fadeInPalette(uncPal,palette_black,30,TRUE);
        VDP_loadTileSet(&test_tiles,ind[3],DMA);
        lvlBG = MAP_create(&test_bg,BG_B,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind[3]));
        lvlFG = MAP_create(&test_fg,BG_A,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind[3]));
        XGM_startPlay(testtrck);
        break;
    }
    default:
    {
        break;
    }
    }
}

void gameInit()
{
    player_spd_x = FIX32(0);
    sprDealloc();
    spawnHUD();
    drawLevel();
    spawnPlayer();
    playerPos();
    JOY_setEventHandler(&gameInputHdl);
    while(1)
    {
        camPos();
        SYS_doVBlankProcess();
        SPR_update();
        XGM_nextFrame();
        playerPos();
        updateHUD();
    }
}