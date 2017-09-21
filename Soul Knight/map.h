#ifndef MAP_H
#define MAP_H

#include "Soul Knight.h"

IMAGE Map, Map2, Map3, Map4, door_row, door_col, door_row_vice, door_col_vice, background, gateway, crystal, crystal_vice;

extern int object[6074][5260];
extern POINT map;
extern Player knight;
extern enemy_species enemy[SPECIES];
extern int pause, trans, Stage, over_time, recover, runout, music_switch;
extern int r1_mid, r1_left, r1_down, r2_mid, r2_right, r2_down, r3_mid, r3_right, r3_down, r3_downright;

#endif // !MAP_H

