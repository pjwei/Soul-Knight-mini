#include "music.h"

void startup_music()
{
	mciSendString(_T("open resource\\start.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\nextstage.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\BOSS.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\boss_angry.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\boss_dead.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\start_button.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\player_hurt.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\gun.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\enemy_gun.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\enemy_dead.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\switch.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\door.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\transform.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\crystal_break.wav type MPEGVideo"), NULL, 0, NULL);
	mciSendString(_T("open resource\\victory.wav type MPEGVideo"), NULL, 0, NULL);

}