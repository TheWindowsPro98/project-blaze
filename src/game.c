#include "includes.h"

enum hudPos {livesStrX = 19, scoreStrX = 12, fpsStrX = 19};
enum playerAnim {idleAnim, walkAnim, jumpAnim};
enum camScrlBounds {leftCamBnd = 135, rightCamBnd = 135, highCamBnd = 127, lowCamBnd = 127};
enum vdpViewport {horzRes = 320, vertRes = 224};
enum playerSize {playerWidth = 32, playerHeight = 32};

Sprite* sPlayer;
Sprite* healthbar;
Sprite* icon;
Map* lvlBG;
Map* lvlFG;
VRAMRegion* stg_vram[lvlMax+1];
VRAMRegion* windowgfx_vram;

fix32 player_x;
fix32 player_y;
fix16 player_spd_x;
fix16 player_spd_y;
const u8 jumpMax = 48;
bool paused;
bool isJumping;
bool isGrounded;
fix16 health;
const u32 mapWidths[lvlMax+1] = 
{
    0,0,0,0,0,0,0,0,1280
};
const u32 mapHeights[lvlMax+1] = 
{
    0,0,0,0,0,0,0,0,448
};
s16 new_cam_x;
s16 new_cam_y;
const char playerNames[7][10] = {"Jade","Stephanie","Emma","Selina","Christina","Carolyn","Vanellope"};

u16 sPosToTPos(u8 basePos)
{
    return basePos * 8;
}

static void pauseChk()
{
    switch (paused)
    {
    case FALSE:
    {
        XGM_resumePlay();
        sPlayer->timer = 12;
        JOY_setEventHandler(gameInputHdl);
        break;
    }
    case TRUE:
    {
        XGM_pausePlay();
        sPlayer->timer = 0;
        player_spd_x = 0;
        player_spd_y = 0;
        JOY_setEventHandler(pauseInputHdl);
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
            sPlayer = SPR_addSprite(&lucy,fix16ToInt(player_x),fix16ToInt(player_y),TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 1:
        {
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 2:
        {
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 3:
        {
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 4:
        {
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        case 5:
        {
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
        default:
        {
            sPlayer = SPR_addSprite(&lucy,player_x,player_y,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
            break;
        }
    }
}

static void spawnHUD()
{
    u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
    u16 end = &stg_vram[round]->endIndex;
    VRAM_createRegion(&windowgfx_vram,end,4);
    ind = VRAM_alloc(&windowgfx_vram,4);
    u16 basetileVRAM = TILE_ATTR_FULL(PAL3,FALSE,FALSE,FALSE,ind);
    VDP_loadTileSet(&window_tiles,ind,DMA);
    healthbar = SPR_addSprite(&bars,sPosToTPos(2),sPosToTPos(1),basetile);
    VDP_setWindowVPos(FALSE,2);
    VDP_setTileMapEx(WINDOW,&window_map,basetileVRAM,0,0,0,0,40,2,DMA);
    if (player_ci > 5)
    {
        player_ci = 6;
    }
    VDP_drawTextEx(WINDOW,playerNames[player_ci],basetile,2,0,DMA);
    icon = SPR_addSprite(&plr_icns,0,0,basetile);
    SPR_setAnim(icon,player_ci);
    VDP_drawTextEx(WINDOW,"{",basetile,scoreStrX-1,0,DMA);
    VDP_drawTextEx(WINDOW,"|",basetile,18,1,DMA);
}

static void playerJump()
{
    if (isGrounded != TRUE)
    {
        return;
    }
    if (isJumping != TRUE)
    {
        return;
    }
    XGM_setPCM(64,jump_sfx,sizeof(jump_sfx));
    XGM_startPlayPCM(64,15,SOUND_PCM_CH2);
    isJumping = !isJumping;
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
    if (state & BUTTON_UP)
    {
        player_spd_y = FIX32(-1);
    }
    else if (state & BUTTON_DOWN)
    {
        player_spd_y = FIX32(1);
    }
    else
    {
        player_spd_y = FIX32(0);
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
    else if (new_cam_x > (mapWidths[round] - horzRes))
    {
        new_cam_x = mapWidths[round] - horzRes;
    }
    if (new_cam_y < 16)
    {
        new_cam_y = 16;
    }
    else if (new_cam_y > (mapHeights[round] - vertRes))
    {
        new_cam_y = mapHeights[round] - vertRes;
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
    else if (player_x > FIX32(mapWidths[round] - playerWidth))
    {
        player_x = FIX32(mapWidths[round] - playerWidth);
    }
    if (player_y < FIX32(playerHeight))
    {
        player_y = FIX32(playerHeight);
    }
    else if (player_y > FIX32(mapHeights[round] - playerHeight))
    {
        player_y = FIX32(mapHeights[round] - playerHeight);
    }
}

/// @brief Draw a string of hexadecimal characters from a base integer.
/// @param value Value to convert.
/// @param destStr Character string recieving text
/// @param minChr Minimum amount of characters to draw
/// @param x X coordinate
/// @param y Y coordinate
static void drawIntToHex(s32 value, const char destStr[], u8 minChr, u8 x, u8 y)
{
    intToHex(value, destStr, minChr);
    VDP_drawTextEx(WINDOW,destStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),x,y,DMA);
}

static void updateHUD()
{
    char livesStr[3];
    char scoreStr[7];
    char z80Str[3];
    char pxStr[3], pyStr[3];
    char pauseStr[3];
    char healthStr[3];
    char jmpStr[3];
    char gndStr[3];
    char memStr[5];
    char sprStr[3];
    u8 z80ld = XGM_getCPULoad();
    if (z80ld >= 100)
    {
        killExec(z80Overload);
    }
    s16 px = fix32ToInt(player_x);
    s16 py = fix32ToInt(player_y);
    u16 mem = MEM_getFree();
    u8 sprAmnt = SPR_getNumActiveSprite();
    u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
    intToStr(lives,livesStr,2);
    VDP_drawTextEx(WINDOW,livesStr,basetile,livesStrX,1,DMA);
    intToStr(score,scoreStr,6);
    VDP_drawTextEx(WINDOW,scoreStr,basetile,scoreStrX,0,DMA);
    drawIntToHex(z80ld,z80Str,2,fpsStrX,0);
    drawIntToHex(px,pxStr,4,fpsStrX+3,0);
    drawIntToHex(py,pyStr,4,fpsStrX+7,0);
    drawIntToHex(paused,pauseStr,2,fpsStrX+3,1);
    drawIntToHex(isJumping,jmpStr,2,fpsStrX+5,1);
    drawIntToHex(isGrounded,gndStr,2,fpsStrX+7,1);
    drawIntToHex(fix16ToRoundedInt(health),healthStr,2,fpsStrX+9,1);
    drawIntToHex(mem,memStr,4,fpsStrX+12,0);
    drawIntToHex(sprAmnt,sprStr,2,fpsStrX+16,0);
    SPR_setFrame(healthbar,fix16ToRoundedInt(health));
}

static void drawLevel()
{
    switch (round)
    {
    case 8:
    {
        VRAM_free(&options_vram,ind);
        VRAM_clearRegion(&options_vram);
        VRAM_releaseRegion(&options_vram);
        VRAM_createRegion(&stg_vram[round],TILE_USER_INDEX,192);
        ind = VRAM_alloc(&stg_vram[round],192);
        fadeInPalette(test_palette,player_palettes[player_ci],30,TRUE);
        player_x = FIX32(16);
        player_y = FIX32(352);
        VDP_loadTileSet(&test_tiles,ind,DMA);
        lvlBG = MAP_create(&test_bg,BG_B,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind));
        lvlFG = MAP_create(&test_fg,BG_A,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind));
        MEM_free(lvlFG);
        MEM_free(lvlFG);
        XGM_startPlay(testtrck); 
        break;
    }
    default:
    {
        killExec(lvlOutOfRange);
        break;
    }
    }
}

void gameInit()
{
    player_spd_x = FIX16(0);
    player_spd_y = FIX16(0);
    health = FIX16(32);
    paused = FALSE;
    isJumping = FALSE;
    isGrounded = TRUE;
    drawLevel();
    spawnPlayer();
    spawnHUD();
    playerPos();
    VDP_loadFont(game_font.tileset,DMA);
    JOY_setEventHandler(gameInputHdl);
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