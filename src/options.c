#include "includes.h"

enum xposes {optX = 23, mainX = 13};
enum yposes {optY = 12, mainY = 13};
enum specialOpts {livesOpts = 10, sndOpts, lsOpts};

u8 currentIndex = 0;			// Currently selected menu item
u8 round = 0x08;                // Current Level
bool lsul = FALSE;              // Is level select enabled? (1 - yes, 0 - no)
u8 lives = 0x05;
u8 difficulty = 0x01;           // 0 = Easy, 1 = Normal, 2 = Hard, 3 = Even Harder (that's what she said)
u32 score = 0x000000;
u8 player_ci = 0x03;            // 0 = Jade, 1 = Stephanie, 2 = Cynthia, 3 = Selina, 4 = Alexia, 5 = Christina
static u8 sndIndex = 0x00;      // Sound Select Index
static Sprite* cursor_cnf;
static Sprite* cursor_plr;
fix16 mapScrl;
Sprite* cursor_cst;

Option menu_main[mainNum] = {
	{mainX, mainY, "New Game"},
    {mainX, mainY+1, "Continue Game"},
	{mainX, mainY+2, "Preferences"},
};

static const Option menu_ops[optNum] = {
    {optX, optY-6, "Easy"},
    {optX, optY-5, "Normal"},
    {optX, optY-4, "Hard"},
    {optX, optY-3, "Harder"}, // That's still what she said
    {optX, optY-1, "Jade"},
    {optX, optY, "Stephanie"},
    {optX, optY+1, "Cynthia"},
    {optX, optY+2, "Selina"},
	{optX, optY+3, "Alexia"},
    {optX, optY+4, "Christina"},
    {optX+1, optY+6, ""},
    {optX, optY+7, ""},
    {optX+1, optY+8, ""},
    {optX-6, optY+10, "Exit"},
};

void mainCurUpd()
{
    SPR_setPosition(cursor_cst, menu_main[currentIndex].x*8-8, menu_main[currentIndex].y*8);
}

void opsCurUpd()
{
    SPR_setPosition(cursor_cst,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
}

void curMoveUpOps()
{
    if (currentIndex > 0)
    {
        currentIndex--;
        opsCurUpd();
    }
    else if (currentIndex == 0)
    {
        currentIndex = optNum-1;
        opsCurUpd();
    }
}

void curMoveDownOps()
{
    if (currentIndex < optNum-1)
    {
        currentIndex++;
        opsCurUpd();
    }
    else if (currentIndex == optNum-1)
    {
        currentIndex = 0;
        opsCurUpd();
    }
}

void curMoveUpMain()
{
    if(currentIndex > 0)
    {
        currentIndex--;
        mainCurUpd();
    }
    else if (currentIndex == 0)
    {
        currentIndex = mainNum-1;
        mainCurUpd();
    }
}

void curMoveDownMain()
{
    if(currentIndex < mainNum-1)
    {
        currentIndex++;
        mainCurUpd();
    }
    else if (currentIndex == mainNum-1)
    {
        currentIndex = 0;
        mainCurUpd();
    }
}

void selectOptMain(u16 Option)
{
    switch (Option)
    {
    case 0:
    {
        pickSG();
        break;
    }
    case 1:
    {
        pickCG();
        break;
    }
    case 2:
    {
        pickOpts();
        break;
    }
    default:
        break;
    }
}

void pickSG()
{
    VDP_setHorizontalScroll(BG_B,0);
    SYS_disableInts();
    SRAM_enable();
    SRAM_writeByte(0, round);
    u8 lsAddr = SRAM_readByte(1);
    if (lsAddr != 1)
    {
        SRAM_writeByte(+1,lsul);
    }
    score = 0x000000;
    SRAM_writeByte(2, lives);
    SRAM_writeByte(3, difficulty);
    SRAM_writeByte(4, player_ci);
    SRAM_writeLong(5, score);
    SRAM_disable();
    SYS_enableInts();
    waitMs(1131);
    PAL_fadeOutAll(30,FALSE);
    waitMs(1000);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_clearSprites();
    gameInit();
}

static void selectMusOpts()
{
    switch (sndIndex)
    {
    case 0:
    {
        XGM_startPlay(lvlclr);
        break;
    }
    case 1:
    {
        XGM_startPlay(gameover);
        break;
    }
    case 2:
    {
        XGM_startPlay(rboss);
        break;
    }
    case 3:
    {
        XGM_startPlay(fboss);
        break;
    }
    case 4:
    {
        XGM_startPlay(ending);
        break;
    }
    case 5:
    {
        XGM_startPlay(testtrck);
        break;
    }
    case 6:
    {
        XGM_startPlay(elevator);
        break;
    }
    case 7:
    {
        XGM_startPlay(stg1);
        break;
    }
    case 8:
    {
        XGM_startPlay(stg2);
        break;
    }
    case 9:
    {
        XGM_startPlay(stg3);
        break;
    }
    case 10:
    {
        XGM_startPlay(stg4);
        break;
    }
    case 11:
    {
        XGM_startPlay(stg5);
        break;
    }
    case 12:
    {
        XGM_startPlay(stg6);
        break;
    }
    case 13:
    {
        XGM_startPlay(stg7);
        break;
    }
    case 14:
    {
        XGM_startPlay(stg8);
        break;
    }
    case 15:
    {
        XGM_startPlay(testtrck2);
        break;
    }
    case 16:
    {
        XGM_startPlay(titlevgm);
        break;
    }
    case 63:
    {
        XGM_setPCM(64,testxgm,sizeof(testxgm));
        XGM_startPlayPCM(64,15,SOUND_PCM_CH1);
        player_ci = 0xFF;
        break;
    }
    default:
        break;
    }
}

void selectOptionOpts(u16 Option)
{
    const u8 pcmMax = 94;
    switch (Option)
    {
    case 0:
    {
        SPR_setPosition(cursor_cnf, menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        difficulty = 0;
        break;
    }
    case 1:
    {
        SPR_setPosition(cursor_cnf, menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        difficulty = 1;
        break;
    }
    case 2:
    {
        SPR_setPosition(cursor_cnf, menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        difficulty = 2;
        break;
    }
    case 3:
    {
        SPR_setPosition(cursor_cnf, menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        difficulty = 3;
        break;
    }
    case 4:
    {
        SPR_setPosition(cursor_plr,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        player_ci = 0;
        break;
    }
    case 5:
    {
        SPR_setPosition(cursor_plr,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        player_ci = 1;
        break;
    }
    case 6:
    {
        SPR_setPosition(cursor_plr,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        player_ci = 2;
        break;
    }
    case 7:
    {
        SPR_setPosition(cursor_plr,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        player_ci = 3;
        break;
    }
    case 8:
    {
        SPR_setPosition(cursor_plr,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        player_ci = 4;
        break;
    }
    case 9:
    {
        SPR_setPosition(cursor_plr,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
        player_ci = 5;
        break;
    }
    case 10:
    {
        SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
        lives = livesMax;
        break;
    }
    case 11:
    {
        if (sndIndex <= 63)
        {
            PSG_reset();
            XGM_stopPlay();
            selectMusOpts();
        }
        else
        {
            sampleDefs();
            XGM_stopPlayPCM(SOUND_PCM_CH2);
            XGM_startPlayPCM(sndIndex,15,SOUND_PCM_CH2);
            if (sndIndex > pcmMax)
            {
                XGM_stopPlayPCM(SOUND_PCM_CH2);
                SND_startPlay_PCM(&back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            }
        }
        break;
    }
    case 12:
    {
        if (lsul == FALSE)
        {
            SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
        }
        else
        {
            SND_startPlay_PCM(menu_sel_sfx,sizeof(menu_sel_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
        }
        break;
    }
    case 13:
    {
        mainscrn();
        break;
    }
    default:
    {
        XGM_stopPlay();
        PSG_reset();
        break;
    }
    }
}

static void joyEvent_ops(u16 joy,u16 changed,u16 state)
{
    if (changed & state & BUTTON_UP)
	{
        PSG_reset();
		SND_startPlay_PCM(&menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		curMoveUpOps();
	}
	else if (changed & state & BUTTON_DOWN)
	{
        PSG_reset();
		SND_startPlay_PCM(&menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		curMoveDownOps();
	}
    if (changed & state & BUTTON_RIGHT)
    {
        if (currentIndex == livesOpts)
        {
            if (lives < livesMax)
            {
                SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                lives++;
            }
            else
            {
                SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            }
        }
        else if (currentIndex == sndOpts)
        {
            sndIndex++;
        }
        else if (currentIndex == lsOpts)
        {
            if (lsul == FALSE)
            {
                SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            }
            else
            {
                if (round < lvlMax)
                {
                    SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                    round++;
                }
                else
                {
                    SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                }
            }
        }
    }
    else if (changed & state & BUTTON_LEFT)
    {
        if (currentIndex == livesOpts)
        {
            if (lives > 1)
            {
                SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                PSG_reset();
                lives--;
            }
            else
            {
                SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            }
        }
        else if (currentIndex == sndOpts)
        {
            sndIndex--;
        }
        else if (currentIndex == lsOpts)
        {
            if (lsul == FALSE)
            {
                SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            }
            else
            {
                if (round > 0)
                {
                    SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                    round--;
                }
                else
                {
                    SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                }
            }
        }
    }

	if (changed & state & BUTTON_START)
	{
		SND_startPlay_PCM(&menu_sel_sfx, sizeof(menu_sel_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		selectOptionOpts(currentIndex);
	}
    else if (changed & state & BUTTON_MODE)
    {
        if (currentIndex == livesOpts)
        {
            SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            lives = 1;
        }
    }
    if (changed & state & BUTTON_B)
    {
        XGM_stopPlay();
        PSG_reset();
        SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
        mainscrn();
    }
    if (changed & state & BUTTON_A)
    {
        if (currentIndex == sndOpts)
        {
            sndIndex = sndIndex - 10;
        }
    }
    else if (changed & state & BUTTON_C)
    {
        if (currentIndex == sndOpts)
        {
            sndIndex = sndIndex + 10;
        }
    }
}

void pickOpts()
{
    const char lsTxt[14] = "Stage Select:";
    char livesStr[2] = "05";
    char sndStr[3] = "000";
    char lvlStr[2] = "00";
    u8 z80ld = 0;
    char z80str[3] = "000";
    currentIndex = 0;
    VDP_clearPlane(BG_A,TRUE);
    VDP_releaseAllSprites();
    aplib_unpack(options_pal,uncPal);
    fadeInPalette(uncPal,lucy.palette->data,30,TRUE);
    VDP_drawTextEx(BG_A,"Changes will only take effect upon",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),2,0,DMA);
    VDP_drawTextEx(BG_A,"starting a new game.",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),2,1,DMA);
    VDP_drawTextEx(BG_A,"Difficulty:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX-13,optY-6,DMA);
    VDP_drawTextEx(BG_A,"Player:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX-13,optY-1,DMA);
    VDP_drawTextEx(BG_A,"Lives:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX-13,optY+6,DMA);
    VDP_drawTextEx(BG_A,"Sound Test:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX-13,optY+7,DMA);
    VDP_drawTextEx(BG_A,"Z80 Load:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,27,DMA);
    VDP_drawTextEx(BG_A,"%",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),13,27,DMA);
    SYS_disableInts();
    SRAM_enableRO();
    lsul = SRAM_readByte(1);
    round = SRAM_readByte(0);
    SRAM_disable();
    SYS_enableInts();
    if (lsul == FALSE)
    {
        VDP_drawTextEx(BG_A,lsTxt,TILE_ATTR(PAL0,FALSE,FALSE,FALSE),optX-13,optY+8,DMA);
    }
    else
    {
        VDP_drawTextEx(BG_A,lsTxt,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX-13,optY+8,DMA);
    }
    for (u8 i = 0; i < optNum; i++)
    {
        Option *o;
        o = MEM_alloc(sizeof(Option));
        *o = menu_ops[i];
        VDP_drawTextEx(BG_A,o->label,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),o->x,o->y,DMA);
        MEM_free(o);
    }
    cursor_cst = SPR_addSprite(&cursor,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8,TILE_ATTR(PAL3,TRUE,FALSE,FALSE));
    cursor_cnf = SPR_addSprite(&cursor,menu_ops[difficulty].x*8-8,menu_ops[difficulty].y*8,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
    cursor_plr = SPR_addSprite(&cursor,menu_ops[player_ci].x*8-8,menu_ops[player_ci+4].y*8,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
    JOY_setEventHandler(joyEvent_ops);
    opsCurUpd();
    while(1)
    {
        SPR_update();
        XGM_nextFrame();
        SYS_doVBlankProcess();
        intToStr(lives,livesStr,2);
        VDP_drawTextEx(BG_A,livesStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX+1,optY+6,DMA);
        intToStr(sndIndex,sndStr,3);
        VDP_drawTextEx(BG_A,sndStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX,optY+7,DMA);
        intToStr(round,lvlStr,2);
        if (lsul == FALSE)
        {
            VDP_drawTextEx(BG_A,lvlStr,TILE_ATTR(PAL0,FALSE,FALSE,FALSE),optX+1,optY+8,DMA);
        }
        else
        {
            VDP_drawTextEx(BG_A,lvlStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),optX+1,optY+8,DMA);
        }
        z80ld = XGM_getCPULoad();
        intToStr(z80ld,z80str,3);
        VDP_drawTextEx(BG_A,z80str,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),10,27,DMA);
        mapScrl -= FIX16(0.3334);
        VDP_setHorizontalScroll(BG_B,fix16ToRoundedInt(mapScrl));
    }
}

void pickCG()
{
    SYS_disableInts();
    SRAM_enableRO();
    u8 lvlAddr = SRAM_readByte(0);
    u8 lifeAddr = SRAM_readByte(2);
    u8 diffAddr = SRAM_readByte(3);
    u32 scoreAddr = SRAM_readLong(4);
    u8 playerAddr = SRAM_readByte(8);
    SRAM_disable();
    SYS_enableInts();
    if (lvlAddr == 0xFF)
    {
        pickSG();
    }
    waitMs(1131);
    PAL_fadeOutAll(30,FALSE);
    waitMs(1000);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_releaseAllSprites();
    MEM_free(mapScrl);
    killExec("Feature unimplemented!");
}