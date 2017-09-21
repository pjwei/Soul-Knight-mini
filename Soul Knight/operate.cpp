#include "operate.h"

int start;
int cooling, switch_cooling;

void operate_control()
{
	static int count, Bcount;   //地图人物帧率比例调控参数

	if (character.x == 3) character.x = 0;

	if (KEY_DOWN(VK_LEFT))
	{
		if (object[map.x + knight.left - speed][map.y + knight.top] != IMMORTAL_OBJ 
			&& object[map.x + knight.left - speed][map.y + knight.bottom] != IMMORTAL_OBJ
			&& object[map.x + knight.left - speed][map.y + knight.center.y] != IMMORTAL_OBJ)
		{
			map.x -= speed;
		}
		if (!auto_aiming()) {
			knight.aspect = LEFT;
			character.y = knight.aspect + 2;
			if (!(KEY_DOWN(VK_UP) || KEY_DOWN(VK_DOWN) || KEY_DOWN(VK_RIGHT)) && count == 4) {
				character.x++;
			}
			weapon[knight.weapon_number].spin = PI / 2;
		}
	}

	if (KEY_DOWN(VK_RIGHT))
	{
		if (object[map.x + knight.right + speed][map.y + knight.top] != IMMORTAL_OBJ
			&& object[map.x + knight.right + speed][map.y + knight.bottom] != IMMORTAL_OBJ
			&& object[map.x + knight.right + speed][map.y + knight.center.y] != IMMORTAL_OBJ
			) 
		{
			map.x += speed;
		}
		if (!auto_aiming()) {
			knight.aspect = RIGHT;
			character.y = knight.aspect + 2;
			if (!(KEY_DOWN(VK_UP) || KEY_DOWN(VK_DOWN)) && count == 4) {
				character.x++;
			}
			weapon[knight.weapon_number].spin = 3 * PI / 2;
		}
	}

	if (KEY_DOWN(VK_DOWN))
	{
		if (object[map.x + knight.left][map.y + knight.bottom + speed] != IMMORTAL_OBJ
			&& object[map.x + knight.right][map.y + knight.bottom + speed] != IMMORTAL_OBJ
			&& object[map.x + knight.center.x][map.y + knight.bottom + speed] != IMMORTAL_OBJ
			) 
		{
			map.y += speed;
		}	
		if (!auto_aiming()) {
			character.y = knight.aspect + 2;
			if (!KEY_DOWN(VK_UP) && count == 4) {
				character.x++;
			}
			weapon[knight.weapon_number].spin = PI;
			if (KEY_DOWN(VK_LEFT)) weapon[knight.weapon_number].spin = 3 * PI / 4;
			if (KEY_DOWN(VK_RIGHT)) weapon[knight.weapon_number].spin = 5 * PI / 4;
		}
		//knight.HP--;
	}

	if (KEY_DOWN(VK_UP))
	{
		if (object[map.x + knight.left][map.y + knight.top - speed] != IMMORTAL_OBJ
			&& object[map.x + knight.right][map.y + knight.top - speed] != IMMORTAL_OBJ
			&& object[map.x + knight.center.x][map.y + knight.top - speed] != IMMORTAL_OBJ
			) 
		{
			map.y -= speed;
		}
		if (!auto_aiming()) {
			character.y = knight.aspect + 2;
			if (count == 4) {
				character.x++;
			}
			weapon[knight.weapon_number].spin = 0;
			if (KEY_DOWN(VK_LEFT)) weapon[knight.weapon_number].spin = PI / 4;
			if (KEY_DOWN(VK_RIGHT)) weapon[knight.weapon_number].spin = 7 * PI / 4;
		}
	}

	if (auto_aiming())
	{
		if (KEY_DOWN(VK_LEFT) || KEY_DOWN(VK_RIGHT) || KEY_DOWN(VK_DOWN) || KEY_DOWN(VK_UP)) {
			character.y = knight.aspect + 2;
			if(count == 4) 
				character.x++;
		}
	}

	if (!(KEY_DOWN(VK_LEFT) || KEY_DOWN(VK_RIGHT) || KEY_DOWN(VK_DOWN) || KEY_DOWN(VK_UP)))
	{
		character.y = knight.aspect;
		if (count == 4)
			character.x++;
	}

	if (KEY_DOWN(VK_Z))
	{
		if (object[knight.center.x + map.x][knight.center.y + map.y] == GATEWAY) {
			trans = 1;
			Stage++;
			mciSendString(_T("close resource\\transform.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\transform.wav"), NULL, 0, NULL);
		}

		else if (cooling == 0) {
			shoot = 1;
			cooling = 1;
			mciSendString(_T("close resource\\gun.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\gun.wav"), NULL, 0, NULL);		
		}		
	}

	if (KEY_DOWN(VK_X))
	{
		if (switch_cooling == 0) {
			switch_cooling = 1;
			if (knight.weapon_number == 0) {
				knight.weapon_number = 1;
				weapon[1].spin = weapon[0].spin;
			}
			else {
				knight.weapon_number = 0;
				weapon[0].spin = weapon[1].spin;
			}

			mciSendString(_T("close resource\\switch.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\switch.wav"), NULL, 0, NULL);
		}
	}

	if (count == 4) count = 0;
	else count++;											//人物行走图改变与地图变换时间比例1:4

	if (switch_cooling == 7) switch_cooling = 0;
	else switch_cooling++;

	if (cooling == 1) {
		if (Bcount >= weapon[knight.weapon_number].swift) {
			Bcount = 0;
			cooling = 0;
		}
		else Bcount++;
	}
}

void operate_select() 
{
	int input;
	int emy;

	if (_kbhit()) {
		input = _getch();

		if (pause == 1 && (input == KEY_ESC || input == KEY_LEFT || input == KEY_RIGHT || input == 'z' || input == 'Z')) {
			mciSendString(_T("close resource\\start_button.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\start_button.wav"), NULL, 0, NULL);
		} //按键音效

		if (pause == 0 && input == KEY_ESC) {
			mciSendString(_T("play resource\\start_button.wav"), NULL, 0, NULL);
			pause = 1;
		}
		else if (pause == 1 && input == KEY_ESC) {
			pause = 0;
		}

		if (pause == 1) {
			if (input == KEY_LEFT) {
				turn--;
			}

			if (input == KEY_RIGHT) {
				turn++;
			}

			if (turn == 0) {
				turn = 3;
			}

			if (turn == 4) {
				turn = 1;
			}

			if ((input == 'Z' || input == 'z') && turn == 3) {
				press = 3;
				if (music_switch == 0) {
					music_switch = 1;
					
					if (Stage <= 2) mciSendString(_T("close resource\\start.wav"), NULL, 0, NULL);
					else if (Stage == 3) mciSendString(_T("close resource\\nextstage.wav"), NULL, 0, NULL);
					else if (Stage == 4) mciSendString(_T("close resource\\BOSS.wav"), NULL, 0, NULL);
				}
				else {
					music_switch = 0;
					if (Stage <= 2) {
						mciSendString(_T("open resource\\start.wav type MPEGVideo"), NULL, 0, NULL);
						mciSendString(_T("play resource\\start.wav repeat"), NULL, 0, NULL);
					}
					else if (Stage == 3) {
						mciSendString(_T("open resource\\nextstage.wav type MPEGVideo"), NULL, 0, NULL);
						mciSendString(_T("play resource\\nextstage.wav repeat"), NULL, 0, NULL);
					}
					else if (Stage == 4) {
						mciSendString(_T("open resource\\BOSS.wav type MPEGVideo"), NULL, 0, NULL);
						mciSendString(_T("play resource\\BOSS.wav repeat"), NULL, 0, NULL);
					}
				}
			}

			if ((input == 'Z' || input == 'z') && turn == 2) {
				press = 2;
			}

			if ((input == 'Z' || input == 'z') && turn == 1) {
				press = 1;
			}
		}

		if (over_time == 50)
		{
			if (input == 'z' || input == 'Z')
			{
				start = 1;
				if (Stage <= 2) mciSendString(_T("close resource\\start.wav"), NULL, 0, NULL);
				else if (Stage == 3) mciSendString(_T("close resource\\nextstage.wav"), NULL, 0, NULL);
				else if (Stage == 4) mciSendString(_T("close resource\\BOSS.wav"), NULL, 0, NULL);
				mciSendString(_T("open resource\\start.wav type MPEGVideo"), NULL, 0, NULL);
				mciSendString(_T("play resource\\start_button.wav"), NULL, 0, NULL);
				Sleep(500);
				mciSendString(_T("close resource\\start_button.wav"), NULL, 0, NULL);

				if (Stage >= 2) {
					obj_mark(823, 2224, 1542, 2947, IMMORTAL_OBJ);
					obj_mark(1542, 2470, 2792, 2708, IMMORTAL_OBJ);
					obj_mark(2792, 2220, 3543, 2966, IMMORTAL_OBJ);
					obj_mark(3044, 768, 3293, 2220, IMMORTAL_OBJ);
					obj_mark(2895, 221, 3444, 768, IMMORTAL_OBJ);
					obj_mark(3044, 2966, 3293, 4314, IMMORTAL_OBJ);
					obj_mark(2697, 4314, 3642, 4957, IMMORTAL_OBJ);
					obj_mark(3543, 2471, 4791, 2708, IMMORTAL_OBJ);
					obj_mark(4791, 2328, 5318, 2852, IMMORTAL_OBJ);
					obj_mark(4934, 1032, 5173, 2328, IMMORTAL_OBJ);
					obj_mark(4789, 507, 5316, 1032, IMMORTAL_OBJ);
				}
				if (Stage >= 3) {
					obj_mark(788, 2110, 1537, 2844, IMMORTAL_OBJ);				
					obj_mark(1537, 2360, 2678, 2594, IMMORTAL_OBJ);		
					obj_mark(2678, 1950, 3732, 2991, IMMORTAL_OBJ);			
					obj_mark(3075, 856, 3323, 1950, IMMORTAL_OBJ);				
					obj_mark(2927, 315, 3473, 856, IMMORTAL_OBJ);			
					obj_mark(3732, 2360, 4848, 2592, IMMORTAL_OBJ);			
					obj_mark(4848, 2106, 5592, 2835, IMMORTAL_OBJ);			
					obj_mark(3075, 2991, 3325, 4093, IMMORTAL_OBJ);			
					obj_mark(2828, 4093, 3577, 4832, IMMORTAL_OBJ);			
					obj_mark(1237, 4347, 2828, 4581, IMMORTAL_OBJ);			
					obj_mark(691, 4193, 1237, 4732, IMMORTAL_OBJ);			
					obj_mark(3577, 4347, 4860, 4584, IMMORTAL_OBJ);			
					obj_mark(4860, 4087, 5606, 4831, IMMORTAL_OBJ);			
				}

				Stage = 1;
				startup_map_show();
				r1_mid = 0;
				r1_left = 0;
				r1_down = 0;
				r2_mid = 0;
				r2_right = 0;
				r2_down = 0;
				r3_mid = 0;
				r3_mid = 0;
				r3_right = 0;
				r3_down = 0;
				r3_downright = 0;
				rBOSS = 0;
				character.x = 0;
				character.y = 0;			     //初始化人物贴图序号
				knight.aspect = RIGHT;           //初始化人物朝向
				knight.HP = knight.HP_MAX;
				knight.AM = knight.AM_MAX;
				knight.MP = knight.MP_MAX;		 //初始化状态信息
				knight.weapon_number = 0;		 //初始化武器编号
				startup_weapon_show();
				for (emy = 0; emy < SPECIES; emy++) {
					enemy[emy].amount = 0;
					enemy[emy].old_amount = 0;
				}
				start_menu();
			}
		}
	}
}