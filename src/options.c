#include "includes.h"

enum specialOpts {livesOpts = 10, sndOpts, lsOpts};

u8 currentIndex = 0;			// Currently selected menu item
u8 round = 0x08;                // Current Level
bool lsul = FALSE;              // Is level select enabled? (1 - yes, 0 - no)
u8 lives = 0x05;
u8 difficulty = 0x01;           // 0 = Easy, 1 = Normal, 2 = Hard, 3 = Even Harder (that's what she said)
u32 score = 0x000000;
u8 player_ci = 0x02;            // 0 = Jade, 1 = Stephanie, 2 = Emma, 3 = Selina, 4 = Alexia, 5 = Christina
u8 sndIndex = 0x00;      // Sound Select Index
Sprite* cursor_cnf;
Sprite* cursor_plr;
fix16 mapScrl;
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
	{optX, optY+3, "Alexia"},
    {optX, optY+4, "Jessica"},
    {optX+1, optY+6, ""},
    {optX, optY+7, ""},
    {optX+1, optY+8, ""},
    {optX-6, optY+10, "Exit"},
};

void opsCurUpd()
{
    SPR_setPosition(cursor_cst,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8);
}

static void curMoveUpOps()
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

static void curMoveDownOps()
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

void pickSG()
{
    waitMs(1131);
    PAL_fadeOutAll(30,FALSE);
    waitMs(1000);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_clearSprites();
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
	/*-------------------------------------*/
	XGM_setPCM(80,fem_die,sizeof(fem_die));
	XGM_setPCM(81,kikoku,sizeof(kikoku));
	XGM_setPCM(82,fem_kick,sizeof(fem_throw));
	XGM_setPCM(83,fem_throw,sizeof(fem_throw));
	/*---------------------------------------*/
	XGM_setPCM(84,mans_ded1,sizeof(mans_ded1));
	XGM_setPCM(85,mans_ded2,sizeof(mans_ded2));
	XGM_setPCM(86,fasthit,sizeof(fasthit));
	XGM_setPCM(87,mans_throw_enemy,sizeof(mans_throw_enemy));
	XGM_setPCM(88,mans_throw_item,sizeof(mans_throw_item));
	/*---------------------------------------------------*/
	XGM_setPCM(89,testxgm,sizeof(testxgm));
	XGM_setPCM(90,back_xgm,sizeof(back_xgm));
	XGM_setPCM(91,hvr_xgm,sizeof(hvr_xgm));
	XGM_setPCM(92,segaxgm,sizeof(segaxgm));
	XGM_setPCM(93,sel_xgm,sizeof(sel_xgm));
	XGM_setPCM(94,life_sfx,sizeof(life_sfx));
    XGM_setPCM(95,stop_sfx,sizeof(stop_sfx));
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
        XGM_startPlay(titlevgm);
        break;
    }
    default:
        break;
    }
}

void selectOptionOpts(u16 Option)
{
    u8 pcmMax = 95;
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
                XGM_startPlayPCM(90,15,SOUND_PCM_CH1);
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
    char lsTxt[14] = "Stage Select:";
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
        Option o = menu_ops[i];
        VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),o.x,o.y,DMA);
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
    round = SRAM_readByte(lvlSrm);
    lives = SRAM_readByte(livesSrm);
    score = SRAM_readLong(scoreSrm);
    player_ci = SRAM_readByte(plrSrm);
    difficulty = SRAM_readByte(diffSrm);
    SRAM_disable();
    SYS_enableInts();
    waitMs(1131);
    PAL_fadeOutAll(30,FALSE);
    waitMs(1000);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_releaseAllSprites();
    MEM_free(mapScrl);
    gameInit();
}