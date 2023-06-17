#include "includes.h"

enum specialOpts {livesOpts = 10, sndOpts, lsOpts};

u8* currentIndex = 0;			// Currently selected menu item
u8 round = 0x08;                // Current Level
bool lsul = FALSE;              // Is level select enabled? (1 - yes, 0 - no)
u8 lives = 0x05;
u8 difficulty = 0x01;           // 0 = Easy, 1 = Normal, 2 = Hard, 3 = Even Harder (that's what she said)
u32 score = 0x000000;
u8 player_ci = 0x03;            // 0 = Jade, 1 = Stephanie, 2 = Emma, 3 = Selina, 4 = Christina, 5 = Carolyn
u8* sndIndex = 0x00;             // Sound Select Index
Sprite* cursor_cnf;
Sprite* cursor_plr;
Sprite* cursor_cst;

const Option menu_ops[optNum] = {
    {optX, optY-6, "Easy"},
    {optX, optY-5, "Normal"},
    {optX, optY-4, "Hard"},
    {optX, optY-3, "Harder"}, // That's still what she said
    {optX, optY-1, "Jade"},
    {optX, optY, "Stephanie"},
    {optX, optY+1, "Emma"},
    {optX, optY+2, "Selina"},
	{optX, optY+3, "Christina"},
    {optX, optY+4, "Carolyn"},
    {optX+1, optY+6, ""},
    {optX, optY+7, ""},
    {optX+1, optY+8, ""},
    {optX-6, optY+10, "Exit"},
};

static void opsCurUpd()
{
    SPR_setPosition(cursor_cst,sPosToTPos(menu_ops[*currentIndex].x)-8,sPosToTPos(menu_ops[*currentIndex].y));
}

static void curMoveUpOps()
{
    if (*currentIndex > 0)
    {
        *currentIndex -= 1;
        opsCurUpd();
    }
    else if (*currentIndex == 0)
    {
        *currentIndex = optNum-1;
        opsCurUpd();
    }
}

static void curMoveDownOps()
{
    if (*currentIndex < optNum-1)
    {
        *currentIndex += 1;
        opsCurUpd();
    }
    else if (*currentIndex == optNum-1)
    {
        *currentIndex = 0;
        opsCurUpd();
    }
}

void pickSG()
{
    waitMs(49);
    PAL_fadeOutAll(30,FALSE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    SPR_reset();
    MEM_free(sndIndex);
    MEM_free(currentIndex);
    score = 0;
    SYS_disableInts();
    SRAM_enableRO();
    lsul = SRAM_readByte(lsSrm);
    SRAM_disable();
    SYS_enableInts();
    if (lsul == FALSE)
    {
        round = 8;
    }
    gameInit();
}

static void sampleDefs()
{
	XGM_setPCM(64,barrel_brk,sizeof(barrel_brk));
	XGM_setPCM(65,angy_yell,sizeof(angy_yell));
	XGM_setPCM(66,max_punch,sizeof(max_punch));
	XGM_setPCM(67,punch1,sizeof(punch1));
	XGM_setPCM(68,punch2,sizeof(punch2));
	XGM_setPCM(69/*nice*/,punch3,sizeof(punch3));
	XGM_setPCM(70,punch4,sizeof(punch4));
	XGM_setPCM(71,punch5,sizeof(punch5));
	XGM_setPCM(72,consume,sizeof(consume));
	XGM_setPCM(73,boom_sfx,sizeof(boom_sfx));
	XGM_setPCM(74,fall,sizeof(fall));
	XGM_setPCM(75,break_gen,sizeof(break_gen));
	XGM_setPCM(76,itm_cons,sizeof(itm_cons));
	XGM_setPCM(77,jump_sfx,sizeof(jump_sfx));
	XGM_setPCM(78,land,sizeof(land));
	XGM_setPCM(79,stab_sfx,sizeof(stab_sfx));
	XGM_setPCM(80,fem_die,sizeof(fem_die));
	XGM_setPCM(81,kikoku,sizeof(kikoku));
	XGM_setPCM(82,fem_kick,sizeof(fem_throw));
	XGM_setPCM(83,fem_throw,sizeof(fem_throw));
	XGM_setPCM(84,mans_ded1,sizeof(mans_ded1));
	XGM_setPCM(85,mans_ded2,sizeof(mans_ded2));
	XGM_setPCM(86,fasthit,sizeof(fasthit));
	XGM_setPCM(87,mans_throw_enemy,sizeof(mans_throw_enemy));
	XGM_setPCM(88,mans_throw_item,sizeof(mans_throw_item));
    XGM_setPCM(89,life_sfx,sizeof(life_sfx));
    XGM_setPCM(90,stop_sfx,sizeof(stop_sfx));
}

static void selectMusOpts()
{
    switch (*sndIndex)
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
        XGM_startPlay(titlevgm);
        break;
    }
    default:
        break;
    }
}

static void selectSfxOpts()
{
    u8 xgmCount = 90;
    if (*sndIndex <= xgmCount)
    {
        sampleDefs();
        XGM_startPlayPCM(*sndIndex,15,SOUND_PCM_CH2);
    }
    else
    {
        switch (*sndIndex)
        {
        case 91:
        {
            SND_startPlay_2ADPCM(crash,sizeof(crash),SOUND_PCM_CH_AUTO,FALSE);
            break;
        }
        case 92:
        {
            SND_startPlay_2ADPCM(back,sizeof(back),SOUND_PCM_CH_AUTO,FALSE);
            break;
        }
        case 93:
        {
            SND_startPlay_2ADPCM(segapcm,sizeof(segapcm),SOUND_PCM_CH_AUTO,FALSE);
            break;
        }
        case 94:
        {
            SND_startPlay_2ADPCM(select,sizeof(select),SOUND_PCM_CH_AUTO,FALSE);
            break;
        }
        case 95:
        {
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    
}

static void selectOptionOpts(u16 Option)
{
    switch (Option)
    {
    case 0:
    {
        SPR_setPosition(cursor_cnf, menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        difficulty = 0;
        break;
    }
    case 1:
    {
        SPR_setPosition(cursor_cnf, menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        difficulty = 1;
        break;
    }
    case 2:
    {
        SPR_setPosition(cursor_cnf, menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        difficulty = 2;
        break;
    }
    case 3:
    {
        SPR_setPosition(cursor_cnf, menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        difficulty = 3;
        break;
    }
    case 4:
    {
        SPR_setPosition(cursor_plr,menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        player_ci = 0;
        break;
    }
    case 5:
    {
        SPR_setPosition(cursor_plr,menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        player_ci = 1;
        break;
    }
    case 6:
    {
        SPR_setPosition(cursor_plr,menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        player_ci = 2;
        break;
    }
    case 7:
    {
        SPR_setPosition(cursor_plr,menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        player_ci = 3;
        break;
    }
    case 8:
    {
        SPR_setPosition(cursor_plr,menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        player_ci = 4;
        break;
    }
    case 9:
    {
        SPR_setPosition(cursor_plr,menu_ops[*currentIndex].x*8-8,menu_ops[*currentIndex].y*8);
        player_ci = 5;
        break;
    }
    case 10:
    {
        SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
        lives = livesMax;
        break;
    }
    case 11:
    {
        if (*sndIndex <= 63)
        {
            PSG_reset();
            XGM_stopPlay();
            selectMusOpts();
        }
        else
        {
            selectSfxOpts();
        }
        break;
    }
    case 12:
    {
        if (lsul == FALSE)
        {
            SND_startPlay_2ADPCM(back,sizeof(back),SOUND_PCM_CH_AUTO,FALSE);
        }
        else
        {
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
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
        break;
    }
    }
}

static void joyEvent_ops(u16 joy,u16 changed,u16 state)
{
    if (changed & state & BUTTON_UP)
	{
        PSG_reset();
		SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
		curMoveUpOps();
	}
	else if (changed & state & BUTTON_DOWN)
	{
        PSG_reset();
		SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
		curMoveDownOps();
	}
    if (changed & state & BUTTON_RIGHT)
    {
        if (*currentIndex == livesOpts)
        {
            if (lives < livesMax)
            {
                SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
                lives++;
            }
        }
        else if (*currentIndex == sndOpts)
        {
            PSG_reset();
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            *sndIndex += 1;
        }
        else if (*currentIndex == lsOpts)
        {
            if (lsul == FALSE)
            {
                SND_startPlay_2ADPCM(back,sizeof(back),SOUND_PCM_CH_AUTO,FALSE);
            }
            else
            {
                if (round < lvlMax)
                {
                    SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
                    round++;
                }
            }
        }
    }
    else if (changed & state & BUTTON_LEFT)
    {
        if (*currentIndex == livesOpts)
        {
            if (lives > 1)
            {
                SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
                lives--;
            }
        }
        else if (*currentIndex == sndOpts)
        {
            PSG_reset();
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            *sndIndex -= 1;
        }
        else if (*currentIndex == lsOpts)
        {
            if (lsul == FALSE)
            {
                SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            }
            else
            {
                if (round > 0)
                {
                    SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
                    round--;
                }
            }
        }
    }

	if (changed & state & BUTTON_START)
	{
		SND_startPlay_2ADPCM(select,sizeof(select),SOUND_PCM_CH_AUTO,FALSE);
		selectOptionOpts(*currentIndex);
	}
    else if (changed & state & BUTTON_MODE)
    {
        if (*currentIndex == livesOpts)
        {
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            lives = 1;
        }
    }
    if (changed & state & BUTTON_B)
    {
        XGM_stopPlay();
        PSG_reset();
        SND_startPlay_2ADPCM(back,sizeof(back),SOUND_PCM_CH_AUTO,FALSE);
        mainscrn();
    }
    if (changed & state & BUTTON_A)
    {
        if (*currentIndex == sndOpts)
        {
            PSG_reset();
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            *sndIndex -= 10;
        }
    }
    else if (changed & state & BUTTON_C)
    {
        if (*currentIndex == sndOpts)
        {
            PSG_reset();
            SND_startPlay_2ADPCM(hover,sizeof(hover),SOUND_PCM_CH_AUTO,FALSE);
            *sndIndex += 10;
        }
    }
}

void pickOpts()
{
    char lsTxt[14 ] = "Stage Select:";
    char livesStr[3] = "05";
    char sndStr[4] = "000";
    char lvlStr[3] = "00";
    u8 z80ld = 0;
    char z80str[4] = "000";
    u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
    u16 basetileDisabled = TILE_ATTR(PAL0,FALSE,FALSE,FALSE);
    *currentIndex = 0;
    *sndIndex = 0;
    VDP_clearPlane(BG_A,TRUE);
    fadeInPalette(options_pal,player_palettes[0],30,TRUE);
    VDP_drawTextEx(BG_A,"Changes will only take effect upon",basetile,2,0,DMA);
    VDP_drawTextEx(BG_A,"starting a new game.",basetile,2,1,DMA);
    VDP_drawTextEx(BG_A,"Difficulty:",basetile,optX-13,optY-6,DMA);
    VDP_drawTextEx(BG_A,"Player:",basetile,optX-13,optY-1,DMA);
    VDP_drawTextEx(BG_A,"Lives:",basetile,optX-13,optY+6,DMA);
    VDP_drawTextEx(BG_A,"Sound Test:",basetile,optX-13,optY+7,DMA);
    VDP_drawTextEx(BG_A,"Z80 Load:",basetile,0,27,DMA);
    VDP_drawTextEx(BG_A,"%",basetile,13,27,DMA);
    SYS_disableInts();
    SRAM_enableRO();
    lsul = SRAM_readByte(lsSrm);
    round = SRAM_readByte(lvlSrm);
    SRAM_disable();
    SYS_enableInts();
    if (lsul == FALSE)
    {
        VDP_drawTextEx(BG_A,lsTxt,basetileDisabled,optX-13,optY+8,DMA);
    }
    else
    {
        VDP_drawTextEx(BG_A,lsTxt,basetile,optX-13,optY+8,DMA);
    }
    for (u8 i = 0; i < optNum; i++)
    {
        Option o = menu_ops[i];
        VDP_drawTextEx(BG_A,o.label,basetile,o.x,o.y,DMA);
    }
    JOY_setEventHandler(joyEvent_ops);
    SPR_setPosition(cursor_cnf,sPosToTPos(menu_ops[difficulty].x)-8,sPosToTPos(menu_ops[difficulty].y));
    SPR_setPosition(cursor_plr,sPosToTPos(menu_ops[player_ci].x)-8,sPosToTPos(menu_ops[player_ci+4].y));
    opsCurUpd();
    while(1)
    {
        SPR_update();
        SYS_doVBlankProcess();
        XGM_nextFrame();
        intToStr(lives,livesStr,2);
        VDP_drawTextEx(BG_A,livesStr,basetile,optX+1,optY+6,DMA);
        intToStr(*sndIndex,sndStr,3);
        VDP_drawTextEx(BG_A,sndStr,basetile,optX,optY+7,DMA);
        intToStr(round,lvlStr,2);
        if (lsul == FALSE)
        {
            VDP_drawTextEx(BG_A,lvlStr,basetileDisabled,optX+1,optY+8,DMA);
        }
        else
        {
            VDP_drawTextEx(BG_A,lvlStr,basetile,optX+1,optY+8,DMA);
        }
        z80ld = XGM_getCPULoad();
        intToStr(z80ld,z80str,3);
        VDP_drawTextEx(BG_A,z80str,basetile,10,27,DMA);
    }
}

static void pickCG()
{
    MEM_free(sndIndex);
    MEM_free(currentIndex);
    SYS_disableInts();
    SRAM_enableRO();
    round = SRAM_readByte(lvlSrm);
    lives = SRAM_readByte(livesSrm);
    score = SRAM_readLong(scoreSrm);
    player_ci = SRAM_readByte(plrSrm);
    difficulty = SRAM_readByte(diffSrm);
    health = FIX16(SRAM_readWord(healthSrm));
    SRAM_disable();
    SYS_enableInts();
    waitMs(49);
    PAL_fadeOutAll(30,FALSE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_releaseAllSprites();
    gameInit();
}