#include "includes.h"

enum hudPos {livesStrX = 19, scoreStrX = 12, fpsStrX = 19};

Sprite* sPlayer;
Sprite* healthbar[17];
Sprite* icon;
Sprite* post;
VRAMRegion* stg1_vram;
VRAMRegion* stg2_vram;
VRAMRegion* stg3_vram;
VRAMRegion* stg4_vram;
VRAMRegion* stg5_vram;
VRAMRegion* stg6_vram;
VRAMRegion* stg7_vram;
VRAMRegion* stg8_vram;

static float player_x = 16.0f;
static float player_y = 144.0f;
static float cam_x = 0.0f;
static float cam_y = 0.0f; 
static float player_spd_x;
static float player_jump;
static bool paused = FALSE;
static bool isJumping = FALSE;
static bool isGrounded = TRUE;
static u8 health = 128;
static float playerSpeedMax = 2.0f;
static float playerAcceleration = 1.025f;
static const float playerBaseSpeed = 0.5f;
static char playerName[10];

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
        VDP_setHilightShadow(FALSE);
        JOY_setEventHandler(&gameInputHdl);
        break;
    }
    case TRUE:
    {
        XGM_startPlayPCM(65,0,SOUND_PCM_CH2);
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
            sPlayer = SPR_addSpriteEx(&lucy,player_x,player_y,TILE_ATTR(PAL0,FALSE,FALSE,FALSE),NULL,NULL);
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

static void joyEvent_BSOD(u16 joy, u16 changed, u16 state)
{
    if (changed & state & BUTTON_START)
    {
        SYS_hardReset();
    }
}

static void killExec(char err[39])
{
    for (u8 i = 0; i <= 3; i++)
    {
        PAL_setPalette(i,palette_black,DMA);
    }
    aplib_unpack(bsod_palette,uncPal);
    PAL_fadeIn(0,31,uncPal,30,TRUE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_clearPlane(WINDOW,TRUE);
    VDP_setWindowVPos(FALSE,0);
    SPR_end();
    XGM_stopPlay();
    PSG_reset();
    VDP_drawImageEx(BG_A,&bsod_frown,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[round+3]),1,1,FALSE,TRUE);
    u8 consoleRegion = *(u8 *)0xA10001;
    if (consoleRegion == 0xA0)
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
    XGM_startPlay(gameover);
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
        healthbar[i] = SPR_addSprite(&bars,sPosToTPos(2+i),sPosToTPos(1),TILE_ATTR(PAL0,TRUE,FALSE,FALSE));
        SPR_setAnimAndFrame(healthbar[i],1,8);
    }
    healthbar[17] = SPR_addSprite(&bars,sPosToTPos(2+i),sPosToTPos(1),TILE_ATTR(PAL0,TRUE,FALSE,FALSE));
    SPR_setAnim(healthbar[17],3);
    VDP_setWindowVPos(FALSE,2);
    u8 unpacked[64];
    aplib_unpack(playerNames,unpacked);
    if (player_ci > 5)
    {
        killExec("Player ID invalid!");
    }
    for (u8 j = 0; j < 9; j++)
    {
        playerName[j] = unpacked[player_ci*10+j-player_ci];
    }
    VDP_drawTextEx(WINDOW,playerName,TILE_ATTR(PAL0,TRUE,FALSE,FALSE),2,0,DMA);
    icon = SPR_addSprite(&plr_icns,0,0,TILE_ATTR(PAL0,TRUE,FALSE,FALSE));
    SPR_setAnim(icon,player_ci);
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
    if (state & BUTTON_RIGHT)
    {
        SPR_setHFlip(sPlayer,FALSE);
        SPR_setAnim(sPlayer,1);
        player_spd_x = playerBaseSpeed;
    }
    else if (state & BUTTON_LEFT)
    {
        SPR_setHFlip(sPlayer,TRUE);
        SPR_setAnim(sPlayer,1);
        player_spd_x = -playerBaseSpeed;
    }
    else
    {
        SPR_setAnim(sPlayer,0);
        player_spd_x = 0.0f;
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
    switch (round)
    {
    case 8:
    {
        cam_x = -player_x;
        cam_y = -player_y;
        VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
        VDP_setHorizontalScroll(BG_A,cam_x);
        VDP_setHorizontalScroll(BG_B,cam_x/2);
        break;
    }    
    default:
    {
        cam_x = -player_x;
        cam_y = -player_y;
        VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
        VDP_setHorizontalScroll(BG_A,cam_x);
        VDP_setHorizontalScroll(BG_B,cam_x/2);
        break;
    }
    }
}

static void playerPos()
{
    SPR_setPosition(sPlayer,player_x,player_y);
    player_x = player_x + player_spd_x;
    player_spd_x = player_spd_x * playerAcceleration;
    if (player_spd_x >= playerSpeedMax)
    {
        player_spd_x = playerSpeedMax;
    }
    else if (player_spd_x <= -playerSpeedMax)
    {
        player_spd_x = -playerSpeedMax;
    }
    if (player_x <= 0)
    {
        player_x = 0;
        player_spd_x = 0;
    }
    else if (player_x >= 512)
    {
        player_x = 0;
    }
}

static void drawIntToHex(s32 value, const char destStr, u8 minChr, u8 x, u8 y)
{
    intToHex(value, destStr, minChr);
    VDP_drawTextEx(WINDOW,destStr,TILE_ATTR(PAL0,TRUE,FALSE,FALSE),x,y,DMA);
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
    s16 px = (signed int)player_x;
    s16 py = (signed int)player_y;
    s16 cx = (signed int)cam_x;
    s16 cy = (signed int)cam_y;
    u16 mem = MEM_getFree();
    u8 sprAmnt = SPR_getNumActiveSprite();
    intToStr(sprAmnt,livesStr,2);
    VDP_drawTextEx(WINDOW,livesStr,TILE_ATTR(PAL0,TRUE,FALSE,FALSE),livesStrX,1,DMA);
    intToStr(mem,scoreStr,6);
    VDP_drawTextEx(WINDOW,scoreStr,TILE_ATTR(PAL0,TRUE,FALSE,FALSE),scoreStrX,0,DMA);
    VDP_drawTextEx(WINDOW,"{",TILE_ATTR(PAL0,TRUE,FALSE,FALSE),scoreStrX-1,0,DMA);
    drawIntToHex(z80ld,z80Str,2,fpsStrX,0);
    drawIntToHex(px,pxStr,4,fpsStrX+3,0);
    drawIntToHex(py,pyStr,4,fpsStrX+7,0);
    drawIntToHex(cx,cxStr,4,fpsStrX+3,1);
    drawIntToHex(cy,cyStr,4,fpsStrX+7,1);
    drawIntToHex(paused,pauseStr,2,fpsStrX+12,0);
    drawIntToHex(isJumping,jmpStr,2,fpsStrX+14,0);
    drawIntToHex(isGrounded,gndStr,2,fpsStrX+16,0);
    drawIntToHex(health,healthStr,2,fpsStrX+12,1);
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
        fadeInPalette(uncPal,palette_black,0x000,30,TRUE);
        ind[3] = VRAM_alloc(&stg1_vram,192);
        VDP_drawImageEx(BG_A,&title_logo,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[3]),0,0,FALSE,TRUE);
        VDP_drawImageEx(BG_B,&sega_logo,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[3]),12,11,FALSE,TRUE);
        VDP_loadTileSet(&test_tiles,ind[3],DMA);
        XGM_startPlay(testtrck2);
        break;
    }
    default:
    {
        break;
    }
    }
}

void gametest()
{
    player_spd_x = 0.0f;
    sprDealloc();
    drawLevel();
    spawnHUD();
    spawnPlayer();
    PAL_setColor(0,0xE84);
    JOY_setEventHandler(&gameInputHdl);
    while(1)
    {
        SPR_update();
        SYS_doVBlankProcess();
        XGM_nextFrame();
        playerPos();
        camPos();
        updateHUD();
    }
}