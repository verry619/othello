#pragma once

#include <windows.h>
#include "Cmn.h"

class GuiSettingDialog
{
private:
	static GAME_SETTING s_currentGameSetting;
	INT_PTR CALLBACK GameSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void InitItems(HWND hDlg);
	INT_PTR ProcCmd(HWND hDlg, WPARAM wParam, LPARAM lParam);
	int GetCurrentSettingId(DISC enCol);
	void UpdateGameSetting(int idBlack, int idWhite);
public:
	static INT_PTR CALLBACK CallbackLauncher_GameSetting(HWND, UINT, WPARAM, LPARAM);
	static void InitSetting(void);
	static GAME_SETTING GetCurrentGameSetting(void);
};