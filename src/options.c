#include "includes.h"

#define OPTX_OPS 11
#define OPTY_OPS 14

u8 currentIndex = 0;			// Currently selected menu item
u8 round = 0x00;                // Current Level
bool lsul = FALSE;              // Is level select enabled? (1 - yes, 0 - no)
u8 lives = 0x05;
u8 difficulty = 0x01;           // 0 = Easy, 1 = Normal, 2 = Hard, 3 = Even Harder (that's what she said)
u32 score = 0x000000;
u8 player_ci = 0x01;            // 0 = Lucy, 1 = Stephanie, 2 = Cynthia, 3 = Selina
u8 sndIndex = 0x00;
Sprite* cursor_cnf;
Sprite* cursor_plr;
Sprite* cursor_cst;

Option menu_main[NUM_OPTS_MAIN] = {
	{OPTX_MAIN, OPTY_MAIN, "New Game"},
    {OPTX_MAIN, OPTY_MAIN+1, "Continue Game"},
	{OPTX_MAIN, OPTY_MAIN+2, "Level Select"},
	{OPTX_MAIN, OPTY_MAIN+3, "Preferences"},
};

Option menu_ops[NUM_OPTS_OPS] = {
    {OPTX_OPS+12, OPTY_OPS-6, "Easy"},
    {OPTX_OPS+12, OPTY_OPS-5, "Normal"},
    {OPTX_OPS+12, OPTY_OPS-4, "Hard"},
    {OPTX_OPS+12, OPTY_OPS-3, "Harder"}, // That's still what she said
    {OPTX_OPS+12, OPTY_OPS-1, "Lucy"},
    {OPTX_OPS+12, OPTY_OPS, "Stephanie"},
    {OPTX_OPS+12, OPTY_OPS+1, "Cynthia"},
    {OPTX_OPS+12, OPTY_OPS+2, "Selina"},
    {OPTX_OPS+13, OPTY_OPS+4, ""},
    {OPTX_OPS+12, OPTY_OPS+5, ""},
    {OPTX_OPS+6, OPTY_OPS+7, "Exit"},
};

void mainCurUpd()
{
    SPR_setPosition(cursor_cst, menu_main[currentIndex].x*8-12, menu_main[currentIndex].y*8);
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
}

void curMoveDownOps()
{
    if (currentIndex < NUM_OPTS_OPS-1)
    {
        currentIndex++;
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
}

void curMoveDownMain()
{
    if(currentIndex < NUM_OPTS_MAIN-1)
    {
        currentIndex++;
        mainCurUpd();
    }
}

static void lsChk()
{
    SYS_disableInts();
    SRAM_enableRO();
    u8 lsAddr = SRAM_readByte(SRAM_OFFSET+1);
    SRAM_disable();
    SYS_enableInts();
    if (lsAddr == TRUE)
    {
        pickLvlSel();
    }
    else
    {
        SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
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
        lsChk();
        break;
    }  
    case 3:
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
    SYS_disableInts();
    SRAM_enable();
    SRAM_writeByte(SRAM_OFFSET, round);
    u8 lsAddr = SRAM_readByte(SRAM_OFFSET+1);
    if (lsAddr != 1)
    {
        SRAM_writeByte(SRAM_OFFSET+1,lsul);
    }
    score = 0x000000;
    SRAM_writeByte(SRAM_OFFSET+2, lives);
    SRAM_writeByte(SRAM_OFFSET+3, difficulty);
    SRAM_writeByte(SRAM_OFFSET+4, player_ci);
    SRAM_writeLong(SRAM_OFFSET+5, score);
    SRAM_disable();
    SYS_enableInts();
    waitMs(1131);
    PAL_fadeOutAll(30,FALSE);
    waitMs(1000);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_clearSprites();
    gametest();
}

void pickLvlSel()
{
    PAL_setPalette(PAL0,palette_black,DMA);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_setPalette(PAL3,palette_white_text,DMA);
    PAL_fadeIn(16,47,options_pal,30,TRUE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearSprites();
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
    default:
        sndIndex = 64;
        break;
    }
}

void selectOptionOpts(u16 Option)
{
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
        SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
        lives = LIVES_MAX;
        break;
    }
    case 9:
    {
        if (sndIndex <= 63)
        {
            PSG_reset();
            XGM_stopPlay();
            selectMusOpts();
        }
        else
        {
            if (sndIndex <= 93)
            {
                sampleDefs();
                XGM_stopPlayPCM(SOUND_PCM_CH1);
                XGM_startPlayPCM(sndIndex,15,SOUND_PCM_CH1);
            }
            else
            {
                sndIndex = 0;
            }
        }
        break;
    }
    case 10:
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
		SND_startPlay_PCM(&menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		curMoveUpOps();
	}
	else if (changed & state & BUTTON_DOWN)
	{
		SND_startPlay_PCM(&menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		curMoveDownOps();
	}
    if (changed & state & BUTTON_RIGHT)
    {
        if (currentIndex == 8)
        {
            if (lives < LIVES_MAX)
            {
                SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
                lives++;
            }
            else
            {
                SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            }
        }
        else if (currentIndex == 9)
        {
            if (sndIndex < 0xFF)
            {
                sndIndex++;
            }
        }
    }
    else if (changed & state & BUTTON_LEFT)
    {
        if (currentIndex == 8)
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
        else if (currentIndex == 9)
        {
            if (sndIndex > 0)
            {
                sndIndex--;
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
        if (currentIndex == 8)
        {
            SND_startPlay_PCM(menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
            lives = 1;
        }
    }
    if (changed & state & BUTTON_B)
    {
        SND_startPlay_PCM(back_sfx,sizeof(back_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
        mainscrn();
    }
}

static void dummyJoyEvent(u16 joy, u16 changed, u16 state)
{

}

void pickOpts()
{
    char livesStr[2] = "05";
    char sndStr[3] = "000";
    currentIndex = 0;
    PAL_setPalette(PAL0,palette_grey,DMA);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_setPalette(PAL3,palette_white_text,DMA);
    PAL_fadeIn(16,47,options_pal,30,TRUE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_releaseAllSprites();
    VDP_drawTextEx(BG_A,"Changes will only take effect upon",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),2,0,DMA);
    VDP_drawTextEx(BG_A,"starting a new game.",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),2,1,DMA);
    VDP_drawTextEx(BG_A,"Difficulty:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),OPTX_OPS,OPTY_OPS-6,DMA);
    VDP_drawTextEx(BG_A,"Player:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),OPTX_OPS,OPTY_OPS-1,DMA);
    VDP_drawTextEx(BG_A,"Lives:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),OPTX_OPS,OPTY_OPS+4,DMA);
    VDP_drawTextEx(BG_A,"Sound Test:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),OPTX_OPS,OPTY_OPS+5,DMA);
    int i = 0;
    for (i; i < NUM_OPTS_OPS; i++)
    {
        Option o = menu_ops[i];
        VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),o.x,o.y,DMA);
    }
    cursor_cst = SPR_addSprite(&cursor,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8,TILE_ATTR(PAL0,TRUE,FALSE,FALSE));
    cursor_cnf = SPR_addSprite(&cursor,menu_ops[difficulty].x*8-8,menu_ops[difficulty].y*8,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
    cursor_plr = SPR_addSprite(&cursor,menu_ops[player_ci].x*8-8,menu_ops[player_ci+4].y*8,TILE_ATTR(PAL3,FALSE,FALSE,FALSE));
    JOY_setEventHandler(&joyEvent_ops);
    opsCurUpd();
    while(1)
    {
        SPR_update();
        XGM_nextFrame();
        SYS_doVBlankProcess();
        intToStr(lives,livesStr,2);
        VDP_drawTextEx(BG_A,livesStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),OPTX_OPS+13,OPTY_OPS+4,DMA);
        intToStr(sndIndex,sndStr,3);
        VDP_drawTextEx(BG_A,sndStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),OPTX_OPS+12,OPTY_OPS+5,DMA);
    }
}

void pickCG()
{
    SYS_disableInts();
    SRAM_enableRO();
    u8 lvlAddr = SRAM_readByte(SRAM_OFFSET);
    u8 lifeAddr = SRAM_readByte(SRAM_OFFSET+2);
    u8 diffAddr = SRAM_readByte(SRAM_OFFSET+3);
    u32 scoreAddr = SRAM_readLong(SRAM_OFFSET+4);
    u8 playerAddr = SRAM_readByte(SRAM_OFFSET+8);
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
    VDP_clearSprites();
    JOY_setEventHandler(dummyJoyEvent);
}