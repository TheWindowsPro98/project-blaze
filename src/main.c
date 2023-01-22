#include <genesis.h>
#include <resources.h>
#include "opts.h"
#include "main.h"

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
	VDP_releaseAllSprites();
	currentIndex = 0;
	u16 ind = TILE_USER_INDEX;
	PAL_setPalette(PAL0,palette_black,DMA);
    PAL_setPalette(PAL1,palette_black,DMA);
	PAL_fadeInAll(mainpal.data,30,TRUE);
	cursor_cst = SPR_addSprite(&cursor,0,0,TILE_ATTR(PAL1,TRUE,FALSE,FALSE));
	VDP_clearPlane(BG_A,TRUE);
	VDP_drawImageEx(BG_B,&options_bg,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind),0,0,FALSE,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
	VDP_setScrollingMode(HSCROLL_TILE,VSCROLL_PLANE);
	JOY_setEventHandler(&joyEvent_ms);
	mainCurUpd();
	for (int i = 0; i < NUM_OPTS_MAIN; i++)
	{
    	Option o = menu_main[i];
    	VDP_drawTextEx(BG_A,o.label,TILE_ATTR(PAL1,FALSE,FALSE,FALSE),o.x,o.y,DMA);
	}
}
static void title()
{
	PAL_setPalette(PAL0,palette_black,DMA);
	PAL_fadeInPalette(PAL1,title_tmp.data,30,TRUE);
	VDP_loadFont(custom_font.tileset,DMA);
	VDP_drawTextEx(BG_A, "@ TWP98 2017-2022", TILE_ATTR(PAL1,FALSE,FALSE,FALSE),0,27,DMA);
	VDP_drawTextEx(BG_A, "Project Blaze Version pa5.86",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),5,12,DMA);
	VDP_drawTextEx(BG_A,"PRESS START",TILE_ATTR(PAL1,FALSE,FALSE,FALSE),14,13,DMA);
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
}

int main()
{
	SPR_init();
	u16 ind = TILE_USER_INDEX;
	PAL_fadeInPalette(PAL0,sega_logo.palette->data,30,TRUE);
	VDP_drawImageEx(BG_A, &sega_logo,TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind), 13, 12, FALSE, TRUE);	
	waitTick(128);
	SND_startPlay_PCM(&segapcm, sizeof(segapcm), SOUND_RATE_11025, SOUND_PAN_CENTER, FALSE);
	waitMs(2536);
	PAL_fadeOutAll(30,TRUE);
	waitMs(1000);
	VDP_clearPlane(BG_A,TRUE);
	sampleDefs();
	title();
	while(1)
	{   
		SPR_update();
		SYS_doVBlankProcess();
	}
	return (0);
}