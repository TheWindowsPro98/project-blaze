#include "includes.h"

const char stopStrings[4][41] = {"ERROR_USER_GENERIC","ERROR_VALUE_OUT_OF_RANGE","ERROR_XGM_Z80_OVERLOAD","ERROR_FUNCTION_UNIMPLEMENTED"};
u32* stopcode_public;
VRAMRegion* bsod_vram;
static void joyEvent_BSOD(u16 joy, u16 changed, u16 state)
{
    u16 basetile = TILE_ATTR(PAL0,FALSE,FALSE,FALSE);
    if (changed & state & BUTTON_START)
    {
        SND_startPlay_2ADPCM(select,sizeof(select),SOUND_PCM_CH_AUTO,FALSE);
        VDP_drawTextEx(BG_A,"Fixing errors...",basetile,0,20,DMA);
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
            health = FIX16(32);
            SYS_disableInts();
            SRAM_enable();
            SRAM_writeByte(lvlSrm,round);
            SRAM_writeByte(lsSrm,lsul);
            SRAM_writeLong(scoreSrm,score);
            SRAM_writeWord(healthSrm,health);
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
}

void killExec(u32 stopcode)
{
    VDP_resetScreen();
    VDP_loadFont(menu_font.tileset,DMA);
    SPR_end();
    PSG_reset();
    PAL_setPalette(PAL0,bsod_palette,DMA);
    PAL_setPalette(PAL1,player_palettes[0],DMA);
    u16 basetile = TILE_ATTR(PAL1,FALSE,FALSE,FALSE);
    VRAM_createRegion(&bsod_vram,TILE_USER_INDEX,16);
    ind = VRAM_alloc(&bsod_vram,16);
    u16 basetileVRAM = TILE_ATTR_FULL(PAL0,FALSE,FALSE,FALSE,ind);
    char bsodStrings[14][41] = {"A problem has been detected and the","current process has ended to prevent","damage to your console.","If this is the first time you've seen","this screen, reset your console. If you","see this screen again, press the","START button. This will make an attempt","to fix any problems you are having.","If problems continue, contact the","developer for a potential fix, if any","can be provided, or erase all save data","and start over.","Technical Information:","***** STOP: 0|"};
    u8 i = 0;
    for (i; i < 3; i++)
    {
        VDP_drawTextEx(BG_A,bsodStrings[i],basetile,0,i,DMA);
    }
    VDP_drawTextEx(BG_A,stopStrings[stopcode],basetile,0,4,DMA);
    for (i = 3; i < 8; i++)
    {
        VDP_drawTextEx(BG_A,bsodStrings[i],basetile,0,i+3,DMA);
    }
    for (i = 8; i < 12; i++)
    {
        VDP_drawTextEx(BG_A,bsodStrings[i],basetile,0,i+4,DMA);
    }
    for (i = 12; i < 14; i++)
    {
        VDP_drawTextEx(BG_A,bsodStrings[i],basetile,0,i+5,DMA);
    }
    char scStr[9] = "00000000";
    intToHex(stopcode,scStr,8);
    VDP_drawTextEx(BG_A,scStr,basetile,14,18,DMA);
    stopcode_public = MEM_alloc(sizeof(u32));
    memcpy(stopcode_public,&stopcode,sizeof(u32));
    SND_startPlay_2ADPCM(crash,sizeof(crash),SOUND_PCM_CH_AUTO,FALSE);
    JOY_setEventHandler(joyEvent_BSOD);
    while (1)
    {
        SYS_doVBlankProcess();
    }
}