#include "includes.h"

VRAMRegion* sega_scrn;
VRAMRegion* options_vram;
s16 ind[12]; // 0 - Sega Screen, 1 - Title Logo, 2 - Main Menu Background, 3 - Hilight/Shadow Tiles, 4-11 - Stage Graphics, 12 - Ending

static void joyEvent_Title(u16 joy, u16 changed, u16 state)
{
	if (state & BUTTON_START)
	{
		SND_startPlay_PCM(&menu_sel_sfx, sizeof(menu_sel_sfx),SOUND_RATE_11025,SOUND_PAN_CENTER,FALSE);
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

void mainscrn()
{
	char scoreStr[6] = "000000";
	float mapScrl = 0;
	VRAM_free(&sega_scrn, ind[1]);
	VRAM_clearRegion(&sega_scrn);
	VRAM_createRegion(&options_vram,TILE_USER_INDEX,18);
	ind[2] = VRAM_alloc(&options_vram, 18);
	VDP_releaseAllSprites();
	currentIndex = 0;
	fadeInPalette(options_pal.data,stephanie.palette->data,0x000,30,TRUE);
	cursor_cst = SPR_addSprite(&cursor,0,0,TILE_ATTR(PAL0,TRUE,FALSE,FALSE));
	VDP_clearPlane(BG_A,TRUE);
	VDP_loadTileSet(&opts_tiles,ind[2],DMA);
	VDP_setTileMapEx(BG_B,&options_map,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[2]),0,0,0,0,64,28,DMA);
	VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
	SYS_disableInts();
	SRAM_enable();
	if (SRAM_readByte(SRAM_OFFSET) == 0xFF)
	{
		SRAM_writeByte(SRAM_OFFSET,0);
		SRAM_writeByte(SRAM_OFFSET+1,FALSE);
		SRAM_writeLong(SRAM_OFFSET+5,0x000000);
	}
	score = SRAM_readLong(SRAM_OFFSET+5);
	SRAM_disable();
	SYS_enableInts();
	intToStr(score,scoreStr,6);
	VDP_drawTextEx(BG_A,"Score:",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A,scoreStr,TILE_ATTR(PAL0,FALSE,FALSE,FALSE),7,27,DMA);
	JOY_setEventHandler(&joyEvent_ms);
	mainCurUpd();
	for (int i = 0; i < NUM_OPTS_MAIN; i++)
	{
    	Option o = menu_main[i];
    	VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL0,TRUE,FALSE,FALSE),o.x,o.y,DMA);
	}
	while(1)
	{
		mapScrl -= 0.333;
		XGM_nextFrame();
		VDP_setHorizontalScroll(BG_B,mapScrl);
		SPR_update();
		SYS_doVBlankProcess();
	}
}
static void title()
{
	PAL_setPalette(PAL0,title_logo.palette->data,DMA);
	PAL_setColor(0,0x000);
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_fadeInPalette(PAL1,stephanie.palette->data,30,TRUE);
	PAL_setPalette(PAL2,palette_black,DMA);
	VDP_loadFont(menu_font.tileset,DMA);
	VRAM_createRegion(&sega_scrn,TILE_USER_INDEX,440);
	ind[1] = VRAM_alloc(&sega_scrn,440);
	VDP_drawImageEx(BG_A,&title_logo,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind[1]),0,0,FALSE,TRUE);
	VDP_drawTextEx(BG_A, "@ TWP98 2022-2023", TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A, "Version pa5.13",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),12,12,DMA);
	VDP_drawTextEx(BG_A,"PRESS  START",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),13,13,DMA);
	waitMs(500);
	JOY_setEventHandler(&joyEvent_Title);
}

void sampleDefs()
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
}

void fadeInPalette(Palette* fadePalette, Palette* staticPalette, u16 bgColor, u8 fadeTime, bool async)
{
	PAL_setPalette(PAL0,staticPalette,DMA);
	PAL_setColor(0,bgColor); 
	PAL_setPalette(PAL1,palette_black,DMA);
	PAL_setPalette(PAL2,palette_black,DMA);
	PAL_setPalette(PAL3,palette_black,DMA);
	PAL_fadeIn(16,63,fadePalette,fadeTime,async);
}

int main(int resetType)
{
	u8 consoleType = *(u8 *)0xA10001;
	if (consoleType == 0xE0)
	{
		VDP_drawTextEx(BG_A,"This game is not optimized for PAL", TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,0,DMA);
		VDP_drawTextEx(BG_A,"consoles.",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,1,DMA);
	}
	else if (consoleType == 0x60)
	{
		VDP_drawTextEx(BG_A,"This game is not optimized for PAL", TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,0,DMA);
		VDP_drawTextEx(BG_A,"consoles.",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,1,DMA);
		VDP_drawTextEx(BG_A,"Hell, I didn't even know Japan had", TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,3,DMA);
		VDP_drawTextEx(BG_A,"PAL consoles.",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,4,DMA);
	}
	if (resetType == 0)
	{
		SYS_hardReset();
	}
	SPR_init();
	SYS_disableInts();
	SRAM_enable();
	SRAM_disable();
	SYS_enableInts();
	VRAM_createRegion(&sega_scrn, TILE_USER_INDEX,48);
	ind[0] = VRAM_alloc(&sega_scrn,48);
	fadeInPalette(sega_logo.palette->data,palette_black,0x000,30,TRUE);
	VDP_drawImageEx(BG_A, &sega_logo,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind[0]), 12, 12, FALSE, TRUE);
	SND_startPlay_PCM(&segapcm, sizeof(segapcm), SOUND_RATE_11025, SOUND_PAN_CENTER, FALSE);
	waitMs(2408);
	PAL_fadeOutAll(30,FALSE);
	VRAM_free(&sega_scrn,ind[0]);
	VDP_clearPlane(BG_A,TRUE);
	title();
	while(1)
	{   
		SYS_doVBlankProcess();
	}
	return 0;
}