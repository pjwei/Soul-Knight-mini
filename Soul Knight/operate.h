#ifndef OPERATE_H
#define OPERATE_H

#include "Soul Knight.h"

extern int object[6074][5260];
extern int pause, trans, over_time, Stage, runout;
extern int turn, press, music_switch;
extern int shoot, r1_mid, r1_left, r1_down, r2_mid, r2_right, r2_down, r3_mid, r3_right, r3_down, r3_downright, rBOSS;
extern POINT map, character, character_position;
extern Weapon weapon[WEAPONS];
extern Player knight;
extern enemy_species enemy[SPECIES];
extern IMAGE Start, Start_Game;

#endif // !OPERATE_H
