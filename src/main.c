#include "includes.h"

VRAMRegion* sega_scrn;
VRAMRegion* title_vram;
VRAMRegion* options_vram;
s16 ind; // VRAM Index
bool memAllocated = FALSE;

const Option menu_main[mainNum] = {
	{mainX, mainY, "New Game"},
    {mainX, mainY+1, "Continue Game"},
	{mainX, mainY+2, "Statistics"},
	{mainX, mainY+3, "Preferences"},
};

static void mainCurUpd()
{
    SPR_setPosition(cursor_cst,sPosToTPos(menu_main[*currentIndex].x)-8,sPosToTPos(menu_main[*currentIndex].y));
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

static void joyEvent_Stats(u16 joy, u16 changed, u16 state)
{
	if (changed & state & BUTTON_ALL)
	{
		SND_startPlay_PCM(select,sizeof(select),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
		mainscrn();
	}
}

static void pickStats()
{
	u8 statX = 7;
	u8 statY = 8;
	u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
	fadeInPalette(options_pal,player_palettes[player_ci],30,TRUE);
	VDP_clearPlane(BG_A,TRUE);
	VDP_drawTextEx(BG_A,"Score:",basetile,statX,statY+2,DMA);
	VDP_drawTextEx(BG_A,"Lives:",basetile,statX,statY+1,DMA);
	VDP_drawTextEx(BG_A,"Player:",basetile,statX,statY,DMA);
	VDP_drawTextEx(BG_A,"Health:",basetile,statX,statY+3,DMA);
	VDP_drawTextEx(BG_A,"Difficulty:",basetile,statX,statY+4,DMA);
	VDP_drawTextEx(BG_A,"Beat the game:",basetile,statX,statY+6,DMA);
	VDP_drawTextEx(BG_A,"Current Level:",basetile,statX,statY+7,DMA);
	VDP_drawTextEx(BG_A,"Enemies killed:",basetile,statX,statY+9,DMA);
	VDP_drawTextEx(BG_A,"Items collected:",basetile,statX,statY+10,DMA);
	VDP_drawTextEx(BG_A,"Back",basetile,statX+11,statY+12,DMA);
	healthbar = SPR_addSprite(&bars,sPosToTPos(statX+8),sPosToTPos(statY+3),basetile);
	SPR_setFrame(healthbar,fix16ToRoundedInt(health));
	SPR_setPosition(cursor_cst,sPosToTPos(statX+10),sPosToTPos(statY+12));
	char scoreStr[6];
	char livesStr[2];
	char lsStr[2][4] = {{"No"},{"Yes"}};
	char lvlStr[lvlMax+1][12] = {{"TBD"},{"TBD"},{"TBD"},{"TBD"},{"TBD"},{"TBD"},{"TBD"},{"TBD"},{"Test Level"}};
	char diffStr[4][7] = {{"Easy"},{"Normal"},{"Hard"},{"Harder"}};
	VDP_drawTextEx(BG_A,lvlStr[round],basetile,statX+17,statY+7,DMA);
	VDP_drawTextEx(BG_A,"0",basetile,statX+17,statY+10,DMA);
	VDP_drawTextEx(BG_A,"0",basetile,statX+17,statY+9,DMA);
	VDP_drawTextEx(BG_A,playerNames[player_ci],basetile,statX+17,statY,DMA);
	VDP_drawTextEx(BG_A,lsStr[lsul],basetile,statX+15,statY+6,DMA);
	intToStr(score,scoreStr,6);
	VDP_drawTextEx(BG_A,scoreStr,basetile,statX+7,statY+2,DMA);
	intToStr(lives,livesStr,2);
	VDP_drawTextEx(BG_A,livesStr,basetile,statX+7,statY+1,DMA);
	VDP_drawTextEx(BG_A,diffStr[difficulty],basetile,statX+12,statY+4,DMA);
	JOY_setEventHandler(joyEvent_Stats);
	while (1)
	{
		SYS_doVBlankProcess();
		SPR_update();
		*mapScrl -= FIX16(0.3334);
		VDP_setHorizontalScroll(BG_B,fix16ToRoundedInt(*mapScrl));
	}
	
}

static void selectOptMain(u8 Option)
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
        killExec(featureNotFound);
        break;
    }
	case 2:
	{
		pickStats();
		break;
	}
    case 3:
    {
        pickOpts();
        break;
    }
    default:
    {
		break;
	}
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
		currentIndex = MEM_alloc(sizeof(u8));
		mapScrl = MEM_alloc(sizeof(fix16));
		sndIndex = MEM_alloc(sizeof(u8));
		memAllocated = TRUE;
	}
	*mapScrl = 0;
	VRAM_free(&title_vram, ind);
	VRAM_clearRegion(&title_vram);
	VRAM_clearRegion(&title_vram);
	VRAM_createRegion(&options_vram,TILE_USER_INDEX,18);
	ind = VRAM_alloc(&options_vram, 18);
	*currentIndex = 0;
	SPR_end();
	SPR_init();
	u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
	u16 basetileDisabled = TILE_ATTR(PAL0,FALSE,FALSE,FALSE);
	u16 basetileFull = TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind);
	cursor_cst = SPR_addSprite(&cursor,320,0,basetile);
	cursor_cnf = SPR_addSprite(&cursor,328,0,basetileDisabled);
	cursor_plr = SPR_addSprite(&cursor,336,0,basetileDisabled);
	fadeInPalette(options_pal,player_palettes[1],30,TRUE);
	VDP_clearPlane(BG_A,TRUE);
	VDP_loadTileSet(&opts_tiles,ind,DMA);
	VDP_setTileMapEx(BG_B,&options_map,basetileFull,0,0,0,0,64,28,DMA);
	VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
	JOY_setEventHandler(joyEvent_ms);
	mainCurUpd();
	for (u8 i = 0; i < mainNum; i++)
	{
    	Option o = menu_main[i];
    	VDP_drawTextEx(BG_A,o.label,basetile,o.x,o.y,DMA);
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
	ind = VRAM_alloc(&title_vram,440);
	u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
	u16 basetileVRAM = TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind);
	VDP_drawImageEx(BG_A,&title_logo,basetileVRAM,0,0,FALSE,TRUE);
	VDP_drawTextEx(BG_A, "} TheWindowsPro98 2022-2023", basetile,0,27,DMA);
	VDP_drawTextEx(BG_A, "Version pa5.17",basetile,12,12,DMA);
	VDP_drawTextEx(BG_A,"PRESS  START",basetile,13,13,DMA);
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
	fadeInPalette(sega_pal,player_palettes[0],30,TRUE);
	VDP_loadFont(menu_font.tileset,DMA);
	u16 basetile = TILE_ATTR(PAL3,FALSE,FALSE,FALSE);
	u16 basetileVRAM = TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind);
	VRAM_createRegion(&sega_scrn,TILE_USER_INDEX,56);
	ind = VRAM_alloc(&sega_scrn,56);
	switch (consoleType)
	{
	case palEUR:
	{
		VDP_drawTextEx(BG_A,ctStrs[0],basetile,0,0,DMA);
		VDP_drawTextEx(BG_A,ctStrs[1],basetile,0,1,DMA);
		break;
	}
	case palJPN:
	{
		VDP_drawTextEx(BG_A,ctStrs[0],basetile,0,0,DMA);
		VDP_drawTextEx(BG_A,ctStrs[1],basetile,0,1,DMA);
		VDP_drawTextEx(BG_A,ctStrs[2],basetile,0,2,DMA);
		VDP_drawTextEx(BG_A,ctStrs[3],basetile,0,3,DMA);
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
	VDP_drawImageEx(BG_A, &sega_logo,basetileVRAM, 12, 12, FALSE, TRUE);
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