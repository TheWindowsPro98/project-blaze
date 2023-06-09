#include "includes.h"

VRAMRegion* bsod_vram;

const char stopStrings[4][39] = {"Generic error.","Level ID > 8!","Z80 usage < 100%!","Feature currently unimplemented!"};
u32* stopcode_public;
static void joyEvent_BSOD(u16 joy, u16 changed, u16 state)
{
    u16 basetile = TILE_ATTR(PAL0,FALSE,FALSE,FALSE);
    
    if (changed & state & BUTTON_START)
    {
        SND_startPlay_PCM(select,sizeof(select),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
        VDP_drawTextEx(BG_A,"Fixing errors...",basetile,1,20,DMA);
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
            MEM_free(stopcode_public);
            mainscrn();
            break;
        }
        case 2:
        {
            SYS_hardReset();
            break;
        }
        case 3:
        {
            MEM_free(stopcode_public);
            mainscrn();
            break;
        }
        default:
        {
            killExec(genericErr);
            break;
        }
        }
    }
    else if (changed & state & BUTTON_ALL)
    {
        SND_startPlay_PCM(select,sizeof(select),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
        VDP_drawTextEx(BG_A,"Resetting console...",basetile,1,20,DMA);
        SYS_hardReset();
    }
}

void killExec(u32 stopcode)
{
    VDP_resetScreen();
    VDP_loadFont(menu_font.tileset,DMA);
    VRAM_createRegion(&bsod_vram,TILE_USER_INDEX,32);
    ind = VRAM_alloc(&bsod_vram,32);
    SPR_end();
    PSG_reset();
    fadeInPalette(bsod_palette,palette_black,30,TRUE);
    u16 basetile = TILE_ATTR(PAL0,FALSE,FALSE,FALSE);
    u16 basetileVRAM = TILE_ATTR_FULL(PAL1,FALSE,FALSE,FALSE,ind);
    VDP_drawImageEx(BG_A,&bsod_frown,basetileVRAM,1,1,FALSE,TRUE);
    VDP_drawTextEx(BG_A,"An error has occured. To continue:",basetile,1,9,DMA);
    VDP_drawTextEx(BG_A,"Press any other button to restart, or",basetile,1,10,DMA);
    VDP_drawTextEx(BG_A,"Press START to attempt to fix the",basetile,1,11,DMA);
    VDP_drawTextEx(BG_A,"problem.",basetile,1,12,DMA);
    VDP_drawTextEx(BG_A,"If you would like to know more, here",basetile,1,14,DMA);
    VDP_drawTextEx(BG_A,"is the error in question:",basetile,1,15,DMA);
    VDP_drawTextEx(BG_A,stopStrings[stopcode],basetile,1,17,DMA);
    VDP_drawTextEx(BG_A,"Error code:",basetile,1,18,DMA);
    char scStr[9] = "00000000";
    intToHex(stopcode,scStr,8);
    VDP_drawTextEx(BG_A,scStr,basetile,15,18,DMA);
    VDP_drawTextEx(BG_A,"0x",basetile,13,18,DMA);
    stopcode_public = MEM_alloc(sizeof(u32));
    memcpy(stopcode_public,&stopcode,sizeof(u32));
    SND_startPlay_PCM(crash,sizeof(crash),SOUND_RATE_13400,SOUND_PAN_CENTER,FALSE);
    JOY_setEventHandler(&joyEvent_BSOD);
    while (1)
    {
        SYS_doVBlankProcess();
    }
}