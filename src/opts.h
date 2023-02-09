#pragma once

#define NUM_OPTS_MAIN 4
#define NUM_OPTS_OPS 11
#define OPTX_MAIN 13
#define OPTY_MAIN 12
#define SRAM_OFFSET     0x0000      // Can go up to $7FFF (32767 bytes)
#define SCORE_MAX       0x0F4236    // Limits score variable to dec. 999990 because yes
#define LIVES_MAX       0x63        // Limits lives variable to dec. 99 because yes
extern Sprite* cursor_cst;

typedef struct 
{
    u8 x;
    u8 y;
    char label[24];
} Option;

extern u8 currentIndex;
extern u8 round;
extern bool lsul;
extern u8 lives;
extern u8 difficulty;
extern u32 score;
extern u8 player_ci;

Option menu_main[NUM_OPTS_MAIN];
Option menu_ops[NUM_OPTS_OPS];
void mainCurUpd();
void curMoveUpMain();
void curMoveDownMain();
void selectOptMain(u16 Option);
void pickSG();
void pickLvlSel();
void pickOpts();
void pickCG();
static void joyEvent_ops(u16 joy,u16 changed,u16 state);