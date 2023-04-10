#pragma once

extern Sprite* cursor_cst;
enum numOpts {optNum = 14, mainNum = 3};
enum maxVals {scoreMax = 999990, livesMax = 99, lvlMax = 7};

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
extern float* mapScrl;

Option menu_main[mainNum];
void mainCurUpd();
void curMoveUpMain();
void curMoveDownMain();
void selectOptMain(u16 Option);
void pickSG();
void pickLvlSel();
void pickOpts();
void pickCG();
void clearMenuSprites();
static void joyEvent_ops(u16 joy,u16 changed,u16 state);