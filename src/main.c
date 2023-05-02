#include "includes.h"

VRAMRegion* sega_scrn;
VRAMRegion* title_vram;
VRAMRegion* options_vram;
u16 ind[12]; // 0 - Sega Screen, 1 - Title Logo, 2 - Main Menu Background, 3-11 - Stage Graphics, 12 - BSOD frown
u16 uncPal[64];

const Option menu_main[mainNum] = {
	{mainX, mainY, "New Game"},
    {mainX, mainY+1, "Continue Game"},
	{mainX, mainY+2, "Preferences"},
};

static void mainCurUpd()
{
    SPR_setPosition(cursor_cst, menu_main[currentIndex].x*8-8, menu_main[currentIndex].y*8);
}

static void curMoveUpMain()
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

static void curMoveDownMain()
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

static void selectOptMain(u16 Option)
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

static void joyEvent_Title(u16 joy, u16 changed, u16 state)
{
	if (state & BUTTON_START)
	{
		SND_startPlay_PCM(&menu_sel_sfx, sizeof(menu_sel_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		SPR_init();
		mainscrn();
	}
}

static void joyEvent_ms(u16 joy, u16 changed, u16 state)
{
	if (changed & state & BUTTON_UP)
	{
		SND_startPlay_PCM(&menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		curMoveUpMain();
	}
	else if (changed & state & BUTTON_DOWN)
	{
		SND_startPlay_PCM(&menu_hvr,sizeof(menu_hvr),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		curMoveDownMain();
	}
	if (changed & state & BUTTON_START)
	{
		SND_startPlay_PCM(&menu_sel_sfx, sizeof(menu_sel_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
		selectOptMain(currentIndex);
	}
}

static void srmDefaults()
{
	SYS_disableInts();
	SRAM_enable();
	if (score > scoreMax || lives > livesMax || round > lvlMax)
	{
		score = 0x000000;
		lives = 0x05;
		round = 0x08;
		lsul = FALSE;
		player_ci = 0x02;
		difficulty = 0x01;
		SRAM_writeLong(scoreSrm,score);
		SRAM_writeByte(livesSrm,lives);
		SRAM_writeByte(lvlSrm,round);
		SRAM_writeByte(lsSrm,lsul);
		SRAM_writeByte(plrSrm,player_ci);
		SRAM_writeByte(diffSrm,difficulty);
	}
	SRAM_disable();
	SYS_enableInts();
}

void mainscrn()
{
	char scoreStr[6];
	mapScrl = 0;
	VDP_releaseAllSprites();
	VRAM_free(&title_vram, ind[1]);
	VRAM_clearRegion(&title_vram);
	VRAM_createRegion(&options_vram,TILE_USER_INDEX,18);
	ind[2] = VRAM_alloc(&options_vram, 18);
	currentIndex = 0;
	aplib_unpack(options_pal,uncPal);
	fadeInPalette(uncPal,palette_black,30,TRUE);
	aplib_unpack(lucy_pal,uncPal);
	PAL_setPalette(PAL3,uncPal,DMA);
	SPR_init();
	cursor_cst = SPR_addSprite(&cursor,0,0,TILE_ATTR(PAL3,TRUE,FALSE,FALSE));
	VDP_clearPlane(BG_A,TRUE);
	VDP_loadTileSet(&opts_tiles,ind[2],DMA);
	VDP_setTileMapEx(BG_B,&options_map,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind[2]),0,0,0,0,64,28,DMA);
	VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
	SYS_disableInts();
	SRAM_enable();
	score = SRAM_readLong(scoreSrm);
	SRAM_disable();
	SYS_enableInts();
	srmDefaults();
	intToStr(score,scoreStr,6);
	VDP_drawTextEx(BG_A,"Score:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A,scoreStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),7,27,DMA);
	JOY_setEventHandler(&joyEvent_ms);
	mainCurUpd();
	for (u8 i = 0; i < mainNum; i++)
	{
    	Option o = menu_main[i];
    	VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL3,TRUE,FALSE,FALSE),o.x,o.y,DMA);
	}
	while(1)
	{
		mapScrl -= FIX16(0.3334);
		XGM_nextFrame();
		VDP_setHorizontalScroll(BG_B,fix16ToRoundedInt(mapScrl));
		SPR_update();
		SYS_doVBlankProcess();
	}
}
static void title()
{
	VRAM_free(&sega_scrn,ind[0]);
	VRAM_clearRegion(&sega_scrn);
	VDP_clearPlane(BG_A,TRUE);
	fadeInPalette(title_logo.palette->data,uncPal,30,TRUE);
	VRAM_createRegion(&title_vram,TILE_USER_INDEX,440);
	ind[1] = VRAM_alloc(&title_vram,440);
	VDP_drawImageEx(BG_A,&title_logo,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind[1]),0,0,FALSE,TRUE);
	VDP_drawTextEx(BG_A, "} TWP98 2022-2023", TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A, "Version pa5.16",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),12,12,DMA);
	VDP_drawTextEx(BG_A,"PRESS  START",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),13,13,DMA);
	XGM_setManualSync(TRUE);
	XGM_startPlay(titlevgm);
	JOY_setEventHandler(&joyEvent_Title);
	while (1)
	{
		SYS_doVBlankProcess();
		XGM_nextFrame();
	}
}

void fadeInPalette(Palette* fadePalette, Palette* staticPalette, u8 fadeTime, bool async)
{
	PAL_setPalette(PAL3,staticPalette,DMA);
	PAL_setPalette(PAL0,palette_black,DMA);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_fadeIn(0,47,fadePalette,fadeTime,async);
}

static void joyEvent_SEGA(u16 joy, u16 changed, u16 state)
{
	
}

u8 getConsoleRegion()
{
	u8 consoleType = *(u8 *)0xA10001;
	return consoleType;
}

int main(bool resetType)
{
	u8 consoleType = getConsoleRegion();
	char ctStrs[4][35] = {"This game is not optimized for PAL","consoles.","Hell, I didn't even know Japan had","PAL consoles."};
	aplib_unpack(lucy_pal,uncPal);
	fadeInPalette(sega_logo.palette->data,uncPal,30,TRUE);
	VDP_loadFont(custom_font.tileset,DMA);
	switch (consoleType)
	{
	case palEUR:
	{
		VDP_drawTextEx(BG_A,ctStrs[0],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,0,DMA);
		VDP_drawTextEx(BG_A,ctStrs[1],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,1,DMA);
		break;
	}
	case palJPN:
	{
		VDP_drawTextEx(BG_A,ctStrs[0],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,0,DMA);
		VDP_drawTextEx(BG_A,ctStrs[1],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,1,DMA);
		VDP_drawTextEx(BG_A,ctStrs[2],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,3,DMA);
		VDP_drawTextEx(BG_A,ctStrs[3],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,4,DMA);
		break;
	}
	default:
	{
		break;
	}
	}
	if (resetType == 0)
	{
		SYS_hardReset();
	}
	VRAM_createRegion(&sega_scrn,TILE_USER_INDEX,48);
	ind[0] = VRAM_alloc(&sega_scrn,48);
	VDP_drawImageEx(BG_A, &sega_logo,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind[0]), 12, 12, FALSE, TRUE);
	XGM_setPCM(64,segaxgm,sizeof(segaxgm));
	XGM_startPlayPCM(64,15,SOUND_PCM_CH1);
	JOY_setEventHandler(&joyEvent_SEGA);
	waitMs(2408);
	PAL_fadeOutAll(30,FALSE);
	title();
	while(1)
	{   
		SYS_doVBlankProcess();
		XGM_nextFrame();
	}
	return 0;
}