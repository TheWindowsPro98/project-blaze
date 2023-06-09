#pragma once

extern Sprite* cursor_cst;
extern Sprite* cursor_cnf;
extern Sprite* cursor_plr;
enum numOpts {optNum = 14, mainNum = 4};
enum maxVals {scoreMax = 999990, livesMax = 99, lvlMax = 8, plrIDMax = 5};
enum srmOffsets {lvlSrm, lsSrm, livesSrm, diffSrm, plrSrm, scoreSrm};
enum xposes {optX = 23, mainX = 13, pauseX = 1};
enum yposes {optY = 12, mainY = 13, pauseY = 2};

typedef struct 
{
    u8 x;
    u8 y;
    char label[24];
} Option;

extern u8* currentIndex;
extern u8 round;
extern bool lsul;
extern u8 lives;
extern u8 difficulty;
extern u32 score;
extern u8 player_ci;
extern fix16* mapScrl;
extern u8* sndIndex;