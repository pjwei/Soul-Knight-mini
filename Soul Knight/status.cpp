#include "status.h"

int over_time;

void startup_status_show()
{
	loadimage(&Status, _T("resource\\status_bar.jpg"), 246, 122);
	loadimage(&game_over_1, _T("resource\\game_over_1.jpg"), 1280, 720);
	loadimage(&victory, _T("resource\\victory.jpg"), 1280, 720);
}

void status_show()
{
	LOGFONT Status_point;
	TCHAR hp[8], am[8], mp[12];

	putimage(30, 7, 246, 122, &Status, 0, 0);

	setfillcolor(RGB(129, 34, 34));
	solidrectangle(81, 21, (int)(260 - (knight.HP_MAX - knight.HP) * 179.0 / knight.HP_MAX), 46);
	setfillcolor(RGB(221, 58, 58));
	solidrectangle(81, 25, (int)(260 - (knight.HP_MAX - knight.HP) * 179.0 / knight.HP_MAX), 42);   //HP

	setfillcolor(RGB(82, 82, 82));
	solidrectangle(81, 53, (int)(260 - (knight.AM_MAX - knight.AM) * 179.0 / knight.AM_MAX), 78);
	setfillcolor(RGB(140, 140, 140));
	solidrectangle(81, 56, (int)(260 - (knight.AM_MAX - knight.AM) * 179.0 / knight.AM_MAX), 74);   //Armor

	setfillcolor(RGB(18, 67, 115));
	solidrectangle(81, 84, (int)(260 - (knight.MP_MAX - knight.MP) * 179.0 / knight.MP_MAX), 109);
	setfillcolor(RGB(31, 114, 196));
	solidrectangle(81, 88, (int)(260 - (knight.MP_MAX - knight.MP) * 179.0 / knight.MP_MAX), 106);  //MP

	gettextstyle(&Status_point);                     // 获取当前字体设置
	Status_point.lfHeight = 22;                      // 设置文字高度
	Status_point.lfWeight = FW_HEAVY;				 // 设置文字粗细
	_tcscpy(Status_point.lfFaceName, _T("楷体"));    // 设置字体
	setbkmode(TRANSPARENT);							 // 设置文字背景
	settextcolor(WHITE);							 // 设置文字颜色
	settextstyle(&Status_point);                     // 设置文字样式

	_stprintf(hp, _T("%d"), knight.HP);              //HP数值显示
	_tcscat(hp, " / 7");
	outtextxy(137, 23, hp);
	_stprintf(am, _T("%d"), knight.AM);				 //Armor数值显示
	_tcscat(am, " / 6");
	outtextxy(137, 55, am);
	_stprintf(mp, _T("%d"), knight.MP);				 //MP数值显示
	_tcscat(mp, " / 200");
	outtextxy(115, 87, mp);

	if (knight.HP == 0) {
		if (over_time == 50) 
			putimage(0, 0, 1280, 720, &game_over_1, 0, 0);
		else if (over_time >= 0) over_time++;
	}
	else over_time = 0;

	if (Stage > STAGE) {
		putimage(0, 0, 1280, 720, &victory, 0, 0);
		mciSendString(_T("close resource\\BOSS.wav"), NULL, 0, NULL);
		mciSendString(_T("play resource\\victory.wav"), NULL, 0, NULL);
		over_time = 50;
	}

	if (pause == 0 && pausemenu_position.y == -359) {
		FlushBatchDraw();
	}
}