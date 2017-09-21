#include "menu.h"

int turn = 2;
int press = 0;
int music_switch = 0;

void startup_menu_show()
{
	pausemenu_position.x = 366;
	pausemenu_position.y = -360;

	loadimage(&Start, _T("resource\\start_menu.jpg"), 1280, 720);
	loadimage(&Start_Game, _T("resource\\start_interface.jpg"), 1280, 720);

	loadimage(&Pause_home, _T("resource\\pause_menu_home.jpg"), 548, 359);
	loadimage(&Pause_resume, _T("resource\\pause_menu_resume.jpg"), 548, 359);
	loadimage(&Pause_music, _T("resource\\pause_menu_music.jpg"), 548, 359);
	loadimage(&Pause_musicoff, _T("resource\\pause_menu_music_off.jpg"), 548, 359);
}

void start_menu()
{
	int input;

	putimage(0, 0, 1280, 720, &Start, 0, 0);
	FlushBatchDraw();

	music_switch = 0;
	//播放音乐      
	if (Stage <= 2)
		mciSendString(_T("play resource\\start.wav repeat"), NULL, 0, NULL);

	while (1) {
		input = _getch();
		if (input == 'z' || input == 'Z')    //开始游戏
		{
			mciSendString(_T("play resource\\start_button.wav"), NULL, 0, NULL);
			putimage(0, 0, 1280, 720, &Start_Game, 0, 0);
			FlushBatchDraw();
			Sleep(1000);
			mciSendString(_T("close resource\\start_button.wav"), NULL, 0, NULL);
			if (Stage <= 2) {
				mciSendString(_T("close resource\\start.wav"), NULL, 0, NULL);
				mciSendString(_T("open resource\\start.wav type MPEGVideo"), NULL, 0, NULL);
				mciSendString(_T("play resource\\start.wav repeat"), NULL, 0, NULL);
			}
			else if (Stage == 3) {
				mciSendString(_T("play resource\\nextstage.wav repeat"), NULL, 0, NULL);
			}
			else if (Stage == 4) { 
				mciSendString(_T("play resource\\BOSS.wav repeat"), NULL, 0, NULL);
			}
			break;
		}
	}
}

void pause_menu() {

	if (pausemenu_position.y < 180) 
	{
		putimage(pausemenu_position.x, pausemenu_position.y, 548, 359, &Pause_resume, 0, 0);
		pausemenu_position.y += 90;
	}
	if (pausemenu_position.y == 180) 
	{
		switch (turn)
		{
		case 1: 
			putimage(366, 180, 548, 359, &Pause_home, 0, 0);
			if (press == 1) {
				pause = 0;
				press = 0;
				pausemenu_position.y = -360;
				turn = 2;
				if (Stage <= 2) {
					mciSendString(_T("close resource\\start.wav"), NULL, 0, NULL);
					mciSendString(_T("open resource\\start.wav type MPEGVideo"), NULL, 0, NULL);
				}
				else if (Stage == 3) {
					mciSendString(_T("close resource\\nextstage.wav"), NULL, 0, NULL);
					mciSendString(_T("open resource\\nextstage.wav type MPEGVideo"), NULL, 0, NULL);
				}
				else if (Stage == 4) {
					mciSendString(_T("close resource\\BOSS.wav"), NULL, 0, NULL);
					mciSendString(_T("open resource\\BOSS.wav type MPEGVideo"), NULL, 0, NULL);
				}
				start_menu();
			}
			break;
		case 2:		
			putimage(366, 180, 548, 359, &Pause_resume, 0, 0);
			if (press == 2) {
				pause = 0;
				press = 0;
			}
			break;			
		case 3: 
			putimage(366, 180, 548, 359, &Pause_music, 0, 0);
			if (press == 3) {
				if (music_switch == 1) {
					putimage(366, 180, 548, 359, &Pause_musicoff, 0, 0);
				}
				else { 
					putimage(366, 180, 548, 359, &Pause_music, 0, 0); 
				}
			}
			break;
		}
	}

	FlushBatchDraw();
}

void pause_back() {

	if (pausemenu_position.y > -360) {
		putimage(pausemenu_position.x, pausemenu_position.y, 548, 359, &Pause_resume, 0, 0);
		pausemenu_position.y -= 90;
	}

	FlushBatchDraw();
}

