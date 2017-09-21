/*
*    Soul Knight Project
*	 Team production only for study and communication.
*	 Team member: Pengjin Wei, Qi Shao, Zefan Lu, Hhengjie Zhang.
*    This file was created by Pengjin Wei on 6/5/2017.
*/

#include "Soul Knight.h"

int object[6074][5260] = { 0 };

Weapon weapon[WEAPONS];
Player knight;              

int timer_1, timer_2, timer_3;
int pause, trans;
int Stage = 1;

void main()
{
	HWND hwnd = GetHWnd();   //获取窗体句柄
	MSG msg;

	initgraph(window_width, window_height);     //启动游戏界面
	srand((unsigned)time(NULL));

	startup_music();                            //音乐初始化
	startup_menu_show();                        //开始、暂停菜单初始化
	startup_map_show();                         //地图初始化
	startup_weapon_show();						//武器信息初始化
	startup_status_show();                      //人物状态初始化
	startup_enemy();							//怪物信息初始化
	startup_character_show();				    //人物初始化
	start_menu();								//显示开始界面

	timer_1 = SetTimer(hwnd, 1, 30, (TIMERPROC)timer);     //计时器1， 周期30ms
	timer_2 = SetTimer(hwnd, 2, 30, (TIMERPROC)timer);
	timer_3 = SetTimer(hwnd, 2, 100, (TIMERPROC)timer);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_TIMER)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}          //计时器回调信息处理
}

VOID CALLBACK timer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == timer_1)
	{
		if (trans == 0)
		{
			cleardevice();
			map_show();
			character_show();
			bullet_show();
			enemy_bullet();
			status_show();
			if (pause == 1)
				pause_menu();
			if (pause == 0) {
				pause_back();
				if (knight.HP > 0) {
					operate_control();
					armor_recover();
				}
			}
		}	
	}
	if (idEvent == timer_2)
	{	
		if (trans == 0)
			operate_select();
	}
	if (idEvent == timer_3)
	{
		if (trans == 1) 
			transform();
	}
}
