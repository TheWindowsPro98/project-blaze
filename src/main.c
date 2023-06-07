#include "includes.h"

VRAMRegion* sega_scrn;
VRAMRegion* title_vram;
VRAMRegion* options_vram;
u16 ind; // VRAM Index
bool memAllocated = FALSE;

const Option menu_main[mainNum] = {
	{mainX, mainY, "New Game"},
    {mainX, mainY+1, "Continue Game"},
	{mainX, mainY+2, "Preferences"},
};

static void mainCurUpd()
{
    SPR_setPosition(cursor_cst, menu_main[*currentIndex].x*8-8, menu_main[*currentIndex].y*8);
}

static void curMoveUpMain()
{
    if(*currentIndex > 0)
    {
        *currentIndex -= 1;
        mainCurUpd();
    }
    else if (*currentIndex == 0)
    {
        *currentIndex = mainNum-1;
        mainCurUpd();
    }
}

static void curMoveDownMain()
{
    if(*currentIndex < mainNum-1)
    {
        *currentIndex += 1;
        mainCurUpd();
    }
    else if (*currentIndex == mainNum-1)
    {
        *currentIndex = 0;
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
	if (changed & state & BUTTON_START)
	{
		SND_startPlay_PCM(select,sizeof(select),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
		SPR_init();
		mainscrn();
	}
}

static void joyEvent_ms(u16 joy, u16 changed, u16 state)
{
	if (changed & state & BUTTON_UP)
	{
		SND_startPlay_PCM(hover,sizeof(hover),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
		curMoveUpMain();
	}
	else if (changed & state & BUTTON_DOWN)
	{
		SND_startPlay_PCM(hover,sizeof(hover),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
		curMoveDownMain();
	}
	if (changed & state & BUTTON_START)
	{
		SND_startPlay_PCM(select,sizeof(select),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
		selectOptMain(*currentIndex);
	}
}

void mainscrn()
{
	if (memAllocated == FALSE)
	{
		ind = VRAM_alloc(&title_vram,440);
		currentIndex = MEM_alloc(sizeof(u8));
		mapScrl = MEM_alloc(sizeof(fix16));
		sndIndex = MEM_alloc(sizeof(u8));
		memAllocated = TRUE;
	}
	char scoreStr[6];
	*mapScrl = 0;
	VRAM_free(&title_vram, ind);
	VRAM_clearRegion(&title_vram);
	VRAM_clearRegion(&title_vram);
	VRAM_createRegion(&options_vram,TILE_USER_INDEX,18);
	ind = VRAM_alloc(&options_vram, 18);
	*currentIndex = 0;
	SPR_init();
	cursor_cst = SPR_addSprite(&cursor,320,0,TILE_ATTR(PAL3,TRUE,FALSE,FALSE));
	cursor_cnf = SPR_addSprite(&cursor,328,0,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
	cursor_plr = SPR_addSprite(&cursor,336,0,TILE_ATTR(PAL0,FALSE,FALSE,FALSE));
	fadeInPalette(options_pal,player_palettes[1],30,TRUE);
	VDP_clearPlane(BG_A,TRUE);
	VDP_loadTileSet(&opts_tiles,ind,DMA);
	VDP_setTileMapEx(BG_B,&options_map,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind),0,0,0,0,64,28,DMA);
	VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
	SYS_disableInts();
	SRAM_enable();
	score = SRAM_readLong(scoreSrm);
	SRAM_disable();
	SYS_enableInts();
	intToStr(score,scoreStr,6);
	VDP_drawTextEx(BG_A,"Score:",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A,scoreStr,TILE_ATTR(PAL3,FALSE,FALSE,FALSE),7,27,DMA);
	JOY_setEventHandler(joyEvent_ms);
	mainCurUpd();
	for (u8 i = 0; i < mainNum; i++)
	{
    	Option o = menu_main[i];
    	VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL3,TRUE,FALSE,FALSE),o.x,o.y,DMA);
	}
	while(1)
	{
		*mapScrl -= FIX16(0.3334);
		VDP_setHorizontalScroll(BG_B,fix16ToRoundedInt(*mapScrl));
		SPR_update();
		SYS_doVBlankProcess();
	}
}
static void title()
{
	VRAM_free(&sega_scrn,ind);
	VRAM_clearRegion(&sega_scrn);
	VRAM_releaseRegion(&sega_scrn);
	VDP_clearPlane(BG_A,TRUE);
	fadeInPalette(title_pal,player_palettes,30,TRUE);
	VRAM_createRegion(&title_vram,TILE_USER_INDEX,440);
	VDP_drawImageEx(BG_A,&title_logo,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind),0,0,FALSE,TRUE);
	VDP_drawTextEx(BG_A, "} TWP98 2022-2023", TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A, "Version pa5.16",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),12,12,DMA);
	VDP_drawTextEx(BG_A,"PRESS  START",TILE_ATTR(PAL3,FALSE,FALSE,FALSE),13,13,DMA);
	XGM_startPlay(titlevgm);
	JOY_setEventHandler(joyEvent_Title);
	while (1)
	{
		SYS_doVBlankProcess();
	}
}

void fadeInPalette(u16 fadePalette[48], u16 staticPalette[16], u8 fadeTime, bool async)
{
	u16 finalPalette[64];
	PAL_setPalette(PAL0,palette_black,DMA);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_setPalette(PAL3,palette_black,DMA);
	memcpy(&finalPalette[0],fadePalette,48*sizeof(u16));
	memcpy(&finalPalette[48],staticPalette,16*sizeof(u16));
	PAL_fadeInAll(finalPalette,fadeTime,async);
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
	fadeInPalette(sega_pal,player_palettes[5],30,TRUE);
	VDP_loadFont(custom_font.tileset,DMA);
	switch (consoleType)
	{
	case palEUR:
	{
		for (u8 i = 0; i < 2; i++)
		{
			VDP_drawTextEx(BG_A,ctStrs[i],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,i,DMA);
		}
		break;
	}
	case palJPN:
	{
		for (u8 i = 0; i < 4; i++)
		{
			VDP_drawTextEx(BG_A,ctStrs[i],TILE_ATTR(PAL3,FALSE,FALSE,FALSE),0,i,DMA);
		}
		break;
	}
	default:
	{
		break;
	}
	}
	if (resetType == 0)
	{
		killExec(genericErr);
	}
	VRAM_createRegion(&sega_scrn,TILE_USER_INDEX,56);
	ind = VRAM_alloc(&sega_scrn,56);
	VDP_drawImageEx(BG_A, &sega_logo,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind), 12, 12, FALSE, TRUE);
	SND_startPlay_PCM(segapcm,sizeof(segapcm),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
	JOY_setEventHandler(joyEvent_SEGA);
	waitMs(2048);
	PAL_fadeOutAll(30,FALSE);
	title();
	while(1)
	{   
		SYS_doVBlankProcess();
	}
}