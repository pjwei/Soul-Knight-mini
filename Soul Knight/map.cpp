#include "map.h"

POINT map;
int sound;

void startup_map_show()
{
	if (Stage == 1)
	{
		runout = 0;
		recover = 0;
		sound = 0;
		map.x = 4460;
		map.y = 2130;    //初始化地图位置

		obj_mark(709, 283, 1237, 815, FLOOR);               //左上房间
		obj_mark(853, 815, 1093, 2135, FLOOR);              //左上过道
		obj_mark(617, 2135, 1333, 2859, FLOOR);				//左侧房间
		obj_mark(1333, 2375, 2676, 2619, FLOOR);			//左侧过道
		obj_mark(2676, 2135, 3400, 2859, FLOOR);			//中间房间
		obj_mark(2772, 283, 3300, 815, CRYSTAL);			//上方房间          
		obj_mark(2916, 815, 3156, 2135, FLOOR);			    //上方过道
		obj_mark(2916, 2859, 3156, 4199, FLOOR);		    //下方过道
		obj_mark(2676, 4199, 3400, 4923, FLOOR);			//下方房间
		obj_mark(3400, 2375, 4738, 2619, FLOOR);			//右侧过道
		obj_mark(4738, 2135, 5458, 2859, FLOOR);			//右侧房间
		obj_mark(899, 2446, 1091, 2490, IMMORTAL_OBJ);		//左侧房间的树
		obj_mark(2868, 4560, 2916, 4603, IMMORTAL_OBJ);		//下方房间的木桩
		obj_mark(2820, 2296, 2964, 2336, IMMORTAL_OBJ);     //中间房间左上横向的3颗树
		obj_mark(2820, 2336, 2866, 2432, IMMORTAL_OBJ);		//中间房间左上纵向的3颗树
		obj_mark(2820, 2686, 2964, 2726, IMMORTAL_OBJ);		//中间房间左下横向的3颗树
		obj_mark(2820, 2590, 2866, 2686, IMMORTAL_OBJ);		//中间房间左下纵向的3颗树
		obj_mark(3105, 2296, 3250, 2336, IMMORTAL_OBJ);		//中间房间右上横向的3颗树
		obj_mark(3204, 2336, 3250, 2432, IMMORTAL_OBJ);		//中间房间右上纵向的3颗树
		obj_mark(3105, 2686, 3250, 2726, IMMORTAL_OBJ);     //中间房间右下横向的3颗树
		obj_mark(3204, 2590, 3250, 2686, IMMORTAL_OBJ);     //中间房间右下纵向的3颗树
		obj_mark(904, 452, 1031, 620, GATEWAY);				//传送门位置
	}

	if (Stage == 2)
	{
		runout = 0;
		recover = 0;
		sound = 0;
		map.x = 544;
		map.y = 2234;

		obj_mark(709, 283, 1237, 815, IMMORTAL_OBJ);              
		obj_mark(853, 815, 1093, 2135, IMMORTAL_OBJ);             
		obj_mark(617, 2135, 1333, 2859, IMMORTAL_OBJ);				
		obj_mark(1333, 2375, 2676, 2619, IMMORTAL_OBJ);			
		obj_mark(2676, 2135, 3400, 2859, IMMORTAL_OBJ);			
		obj_mark(2772, 283, 3300, 815, IMMORTAL_OBJ);			          
		obj_mark(2916, 815, 3156, 2135, IMMORTAL_OBJ);			   
		obj_mark(2916, 2859, 3156, 4199, IMMORTAL_OBJ);		    
		obj_mark(2676, 4199, 3400, 4923, IMMORTAL_OBJ);			
		obj_mark(3400, 2375, 4738, 2619, IMMORTAL_OBJ);			
		obj_mark(4738, 2135, 5458, 2859, IMMORTAL_OBJ);		//取消第一关标记	

		obj_mark(823, 2224, 1542, 2947, FLOOR);				//左边房间
		obj_mark(1542, 2470, 2792, 2708, FLOOR);			//左边走廊
		obj_mark(2792, 2220, 3543, 2966, FLOOR);			//中间房间
		obj_mark(3044, 768, 3293, 2220, FLOOR);				//上面走廊
		obj_mark(2895, 221, 3444, 768, CRYSTAL);			//上面房间
		obj_mark(3044, 2966, 3293, 4314, FLOOR);			//下面走廊
		obj_mark(2697, 4314, 3642, 4957, FLOOR);			//下面房间
		obj_mark(3543, 2471, 4791, 2708, FLOOR);			//右边走廊
		obj_mark(4791, 2328, 5318, 2852, FLOOR);			//右边房间
		obj_mark(4934, 1032, 5173, 2328, FLOOR);			//右上走廊
		obj_mark(4789, 507, 5316, 1032, FLOOR);				//右上房间
		obj_mark(3243, 2662, 3344, 2821, IMMORTAL_OBJ);		//中间房间的石头
		obj_mark(5002, 693, 5109, 841, GATEWAY);			//传送门位置
	}

	if (Stage == 3)
	{
		runout = 0;
		recover = 0;
		sound = 0;
		map.x = 532;
		map.y = 2112;

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
		obj_mark(4789, 507, 5316, 1032, IMMORTAL_OBJ);		 //取消第二关标记

		obj_mark(788, 2110, 1537, 2844, FLOOR);				//中左房间
		obj_mark(1537, 2360, 2678, 2594, FLOOR);			//中左过道
		obj_mark(2678, 1950, 3732, 2991, FLOOR);			//中间房间
		obj_mark(3075, 856, 3323, 1950, FLOOR);				//中上过道
		obj_mark(2927, 315, 3473, 856, CRYSTAL);			//上方房间
		obj_mark(3732, 2360, 4848, 2592, FLOOR);			//中右过道
		obj_mark(4848, 2106, 5592, 2835, FLOOR);			//右侧房间
		obj_mark(3075, 2991, 3325, 4093, FLOOR);			//中下过道
		obj_mark(2828, 4093, 3577, 4832, FLOOR);			//下方房间
		obj_mark(1237, 4347, 2828, 4581, FLOOR);			//下左过道
		obj_mark(691, 4193, 1237, 4732, FLOOR);				//下左房间
		obj_mark(3577, 4347, 4860, 4584, FLOOR);			//下右过道
		obj_mark(4860, 4087, 5606, 4831, FLOOR);			//下右房间
		obj_mark(3225, 4320, 3275, 4461, IMMORTAL_OBJ);		//下方房间石柱
		obj_mark(917, 4350, 1017, 4525, GATEWAY);			//传送门位置	

		if (music_switch == 0) {
			mciSendString(_T("close resource\\start.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\nextstage.wav repeat"), NULL, 0, NULL);
		}		
	}

	if (Stage == 4)
	{
		runout = 0;
		recover = 0;
		sound = 0;
		map.x = 2562;
		map.y = 2134;
		obj_mark(2927, 315, 3473, 856, FLOOR);				//上方房间
		obj_mark(2630, 2360, 2678, 2594, IMMORTAL_OBJ);		//左侧
		obj_mark(3732, 2360, 3780, 2592, IMMORTAL_OBJ);		//右侧
		obj_mark(3075, 2991, 3325, 3039, IMMORTAL_OBJ);		//下方
		obj_mark(3148, 502, 3253, 654, GATEWAY);			//传送门位置
		if (music_switch == 0) {
			mciSendString(_T("close resource\\nextstage.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\BOSS.wav repeat"), NULL, 0, NULL);
		}
	}

	loadimage(&Map, _T("resource\\map.jpg"), 6074, 5260);
	loadimage(&Map2, _T("resource\\map2.jpg"), 6074, 5260); 
	loadimage(&Map3, _T("resource\\map3.jpg"), 6074, 5260);					
	loadimage(&Map4, _T("resource\\map4.jpg"), 6074, 5260);			//载入图片
	loadimage(&door_row, _T("resource\\door_row.jpg"), 239, 99);
	loadimage(&door_row_vice, _T("resource\\door_row_vice.jpg"), 239, 99);
	loadimage(&door_col, _T("resource\\door_col.jpg"), 48, 267);
	loadimage(&door_col_vice, _T("resource\\door_col_vice.jpg"), 48, 267);
	loadimage(&background, _T("resource\\background.jpg"), 1280, 720);
	loadimage(&gateway, _T("resource\\gateway.jpg"), 2240, 360);
	loadimage(&crystal, _T("resource\\crystal.jpg"), 240, 120);
	loadimage(&crystal_vice, _T("resource\\crystal_vice.jpg"), 240, 120);
}

void map_show()
{
	if (over_time != 50) {
		if (Stage == 1) {
			putimage(0, 0, window_width, window_height, &Map, map.x, map.y);    //绘制地图

			if (runout == 0) {
				putimage(2978 - map.x, 492 - map.y, 120, 120, &crystal_vice, 0, 0, NOTSRCERASE);
				putimage(2978 - map.x, 492 - map.y, 120, 120, &crystal, 0, 0, SRCINVERT);
			}
			else {
				putimage(2978 - map.x, 492 - map.y, 120, 120, &crystal_vice, 120, 0, NOTSRCERASE);
				putimage(2978 - map.x, 492 - map.y, 120, 120, &crystal, 120, 0, SRCINVERT);
				if (sound == 0) {
					mciSendString(_T("close resource\\crystal_break.wav"), NULL, 0, NULL);
					mciSendString(_T("play resource\\crystal_break.wav"), NULL, 0, NULL);
					sound = 1;
				}				
			}

			if (map.x + knight.right < 3400 || r1_mid == -1) room1_mid();
			if (map.x + knight.right < 1333 || r1_left == -1) room1_left();
			if (map.y + knight.top > 4199 || r1_down == -1) room1_down();
		}

		if (Stage == 2) {
			putimage(0, 0, window_width, window_height, &Map2, map.x, map.y);    //绘制地图

			if (runout == 0) {
				putimage(3112 - map.x, 449 - map.y, 120, 120, &crystal_vice, 0, 0, NOTSRCERASE);
				putimage(3112 - map.x, 449 - map.y, 120, 120, &crystal, 0, 0, SRCINVERT);
			}
			else {
				putimage(3112 - map.x, 449 - map.y, 120, 120, &crystal_vice, 120, 0, NOTSRCERASE);
				putimage(3112 - map.x, 449 - map.y, 120, 120, &crystal, 120, 0, SRCINVERT);
				if (sound == 0) {
					mciSendString(_T("close resource\\crystal_break.wav"), NULL, 0, NULL);
					mciSendString(_T("play resource\\crystal_break.wav"), NULL, 0, NULL);
					sound = 1;
				}
			}

			if (map.x + knight.left > 2792 || r2_mid == -1) room2_mid();
			if (map.x + knight.left > 4791 || r2_right == -1) room2_right();
			if (map.y + knight.top > 4314 || r2_down == -1) room2_down();
		}

		if (Stage == 3) {
			putimage(0, 0, window_width, window_height, &Map3, map.x, map.y);    //绘制地图

			if (runout == 0) {
				putimage(3139 - map.x, 527 - map.y, 120, 120, &crystal_vice, 0, 0, NOTSRCERASE);
				putimage(3139 - map.x, 527 - map.y, 120, 120, &crystal, 0, 0, SRCINVERT);
			}
			else {
				putimage(3139 - map.x, 527 - map.y, 120, 120, &crystal_vice, 120, 0, NOTSRCERASE);
				putimage(3139 - map.x, 527 - map.y, 120, 120, &crystal, 120, 0, SRCINVERT);
				if (sound == 0) {
					mciSendString(_T("close resource\\crystal_break.wav"), NULL, 0, NULL);
					mciSendString(_T("play resource\\crystal_break.wav"), NULL, 0, NULL);
					sound = 1;
				}
			}

			if (map.x + knight.left > 2678 || r3_mid == -1) room3_mid();
			if ((map.x + knight.left > 4848 && map.y + knight.bottom < 3060) || r3_right == -1) room3_right();
			if (map.y + knight.top > 4093 || r3_down == -1) room3_down();
			if ((map.x + knight.left > 4860 && map.y + knight.top > 4000) || r3_downright == -1) room3_downright();
		}

		if (Stage == 4) {
			putimage(0, 0, window_width, window_height, &Map4, map.x, map.y);    //绘制地图
			
			BOSS_room();
		}
	}

}

void transform()
{
	static int cnt;

	if (Stage <= STAGE) {
		putimage(0, 0, &background);
		if (cnt <= 7)
			putimage(500, 140, 280, 360, &gateway, cnt * 280, 0);
		else if (cnt <= 14)
			putimage(500, 140, 280, 360, &gateway, (cnt - 7) * 280, 0);
		else if (cnt <= 21)
			putimage(500, 140, 280, 360, &gateway, (cnt - 14) * 280, 0);
		else putimage(500, 140, 280, 360, &gateway, (cnt - 21) * 280, 0);
		FlushBatchDraw();

		if (cnt == 28) {
			cnt = 0;
			trans = 0;
			startup_map_show();
		}
		else cnt++;
	}
	else trans = 0;
}

void obj_mark(int left, int top, int right, int bottom, int obj)
{
	int row, col;
	for (row = left; row <= right; row++)
		for (col = top; col <= bottom; col++)
			object[row][col] = obj;
}