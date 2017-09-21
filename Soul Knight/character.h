#ifndef CHARACTER_H
#define CHARACTER_H

#include "Soul Knight.h"

IMAGE Character, Character_vice;
IMAGE init_bullet, init_bullet_vice, bullet, bullet_vice, bullet_burst, bullet_burst_vice, enemy_bullet_burst;

extern int object[6074][5260];
extern int pause, Stage;
extern POINT character, character_position, map;
extern Weapon weapon[WEAPONS];
extern enemy_species enemy[SPECIES];
extern Player knight;

#endif // !CHARACTER_H
