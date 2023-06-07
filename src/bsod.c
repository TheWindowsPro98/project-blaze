#include "includes.h"

VRAMRegion* bsod_vram;

const char stopStrings[3][38] = {"Generic error.","Level ID > 8!","Z80 usage < 100%!"};
u32* stopcode_public;
static void joyEvent_BSOD(u16 joy, u16 changed, u16 state)
{
    if (changed & state & BUTTON_START)
    {
        VDP_drawTextEx(BG_A,"Fixing errors...",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,20,DMA);
        switch (*stopcode_public)
        {
        case 0:
        {
            SYS_hardReset();
            break;
        }
        case 1:
        {
            round = 8;
            lsul = FALSE;
            score = 0;
            SYS_disableInts();
            SRAM_enable();
            SRAM_writeByte(lvlSrm,round);
            SRAM_writeByte(lsSrm,lsul);
            SRAM_writeLong(scoreSrm,score);
            SRAM_disable();
            SYS_enableInts();
            XGM_setPCM(64,select,sizeof(select));
            XGM_startPlayPCM(64,15,SOUND_PCM_CH1);
            mainscrn();
            break;
        }
        case 2:
        {
            SYS_hardReset();
            break;
        }
        default:
        {
            killExec(genericErr);
            break;
        }
        }
    }
    else
    {
        VDP_drawTextEx(BG_A,"Resetting console...",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,20,DMA);
        SYS_hardReset();
    }
}

void killExec(u32 stopcode)
{
    VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
    VDP_setHorizontalScroll(BG_A,0);
    VDP_setVerticalScroll(BG_A,0);
    VDP_setHorizontalScroll(BG_B,0);
    VDP_setVerticalScroll(BG_B,0);
    VRAM_createRegion(&bsod_vram,TILE_USER_INDEX,32);
    ind = VRAM_alloc(&bsod_vram,32);
    fadeInPalette(bsod_palette,palette_black,30,TRUE);
    VDP_clearPlane(BG_A,TRUE);
    VDP_clearPlane(BG_B,TRUE);
    VDP_clearPlane(WINDOW,TRUE);
    VDP_setWindowVPos(FALSE,0);
    SPR_end();
    XGM_stopPlay();
    PSG_reset();
    VDP_drawImageEx(BG_A,&bsod_frown,TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind),1,1,FALSE,TRUE);
    VDP_drawTextEx(BG_A,"An error has occured. To continue:",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,9,DMA);
    VDP_drawTextEx(BG_A,"Press any other button to restart, or",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,10,DMA);
    VDP_drawTextEx(BG_A,"Press START to attempt to fix the",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,11,DMA);
    VDP_drawTextEx(BG_A,"problem.",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,12,DMA);
    VDP_drawTextEx(BG_A,"If you would like to know more, here",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,14,DMA);
    VDP_drawTextEx(BG_A,"is the error in question:",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,15,DMA);
    VDP_drawTextEx(BG_A,stopStrings[stopcode],TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,17,DMA);
    VDP_drawTextEx(BG_A,"Error code:",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),1,18,DMA);
    char scStr[8] = "00000000";
    intToHex(stopcode,scStr,8);
    VDP_drawTextEx(BG_A,"0x",TILE_ATTR(PAL0,FALSE,FALSE,FALSE),13,18,DMA);
    VDP_drawTextEx(BG_A,scStr,TILE_ATTR(PAL0,FALSE,FALSE,FALSE),15,18,DMA);
    stopcode_public = &stopcode;
    SND_startPlay_PCM(crash,sizeof(crash),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
    JOY_setEventHandler(joyEvent_BSOD);
    while (1)
    {
        SYS_doVBlankProcess();
    }
}