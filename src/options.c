#include <genesis.h>
#include "resources.h"
#include "opts.h"
#include "main.h"
#include "jukebox.h"
#include "game.h"

u8 currentIndex = 0;			// Currently selected menu item
u8 round = 0x00;                // Current Level
bool lsul = FALSE;              // Is level select enabled? (1 - yes, 0 - no)
u8 lives = 0x05;
u8 difficulty = 0x01;           // 0 = Easy, 1 = Normal, 2 = Hard, 3 = Even Harder (that's what she said)
u32 score = 0x000000;

Option menu_main[NUM_OPTS_MAIN] = {
	{OPTX_MAIN, OPTY_MAIN, "New Game"},
    {OPTX_MAIN, OPTY_MAIN+1, "Continue Game"},
	{OPTX_MAIN, OPTY_MAIN+2, "Level Select"},
	{OPTX_MAIN, OPTY_MAIN+3, "Preferences"},
};

Option menu_ops[NUM_OPTS_OPS] = {
    {OPTX_OPS+12, OPTY_OPS, "Easy"},
    {OPTX_OPS+12, OPTY_OPS+1, "Normal"},
    {OPTX_OPS+12, OPTY_OPS+2, "Hard"},
    {OPTX_OPS+12, OPTY_OPS+3, "Harder"}, // That's still what she said
    {OPTX_OPS+4, OPTY_OPS+5, "Music Box"},
    {OPTX_OPS+5, OPTY_OPS+6, "SFX Box"},
    {OPTX_OPS+6, OPTY_OPS+8, "Exit"},
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
    SRAM_writeByte(SRAM_OFFSET+2, lives);
    SRAM_writeByte(SRAM_OFFSET+3, difficulty);
    SRAM_writeLong(SRAM_OFFSET+4, score);
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
    PAL_fadeInAll(mainpal.data,30,TRUE);
    PAL_setPalette(PAL1,stephanie.palette->data,DMA);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearSprites();
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
        musPlayer();
        break;
    }
    case 5:
    {
        sfxPlayer();
        break;
    }
    case 6:
    {
        mainscrn();
        break;
    }
    default:
        break;
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
	if (changed & state & BUTTON_START)
	{
		SND_startPlay_PCM(&menu_sel_sfx, sizeof(menu_sel_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		selectOptionOpts(currentIndex);
	}
}

void pickOpts()
{
    currentIndex = difficulty;
    PAL_setPalette(PAL0,palette_black,DMA);
    PAL_setPalette(PAL1,palette_black,DMA);
    PAL_fadeInAll(mainpal.data,30,TRUE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_releaseAllSprites();
    VDP_drawTextEx(BG_A,"Changes will only take effect upon",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),2,0,DMA);
    VDP_drawTextEx(BG_A,"starting a new game.",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),2,1,DMA);
    VDP_drawTextEx(BG_A,"Difficulty:",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),OPTX_OPS,OPTY_OPS,DMA);
    int i = 0;
    for (i; i < NUM_OPTS_OPS; i++)
    {
        Option o = menu_ops[i];
        VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL1,FALSE,FALSE,FALSE),o.x,o.y,DMA);
    }
    cursor_cst = SPR_addSprite(&cursor,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8,TILE_ATTR(PAL1,TRUE,FALSE,FALSE));
    cursor_cnf = SPR_addSprite(&cursor,menu_ops[currentIndex].x*8-8,menu_ops[currentIndex].y*8,TILE_ATTR(PAL1,TRUE,FALSE,FALSE));
    JOY_setEventHandler(&joyEvent_ops);
    opsCurUpd();
}

void pickCG()
{
    SYS_disableInts();
    SRAM_enableRO();
    u8 lvlAddr = SRAM_readByte(SRAM_OFFSET);
    u8 lifeAddr = SRAM_readByte(SRAM_OFFSET+2);
    u8 diffAddr = SRAM_readByte(SRAM_OFFSET+3);
    u32 scoreAddr = SRAM_readLong(SRAM_OFFSET+4);
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
}