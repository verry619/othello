#pragma once

#include <windows.h>
#include "Cmn.h"

class GuiSettingDialog
{
private:
	static GAME_SETTING s_currentGameSetting;
	static BOARD_SIZE s_currentBoardSize;
	INT_PTR CALLBACK GameSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void InitItems(HWND hDlg);
	INT_PTR ProcCmd(HWND hDlg, WPARAM wParam, LPARAM lParam);
	int GetCurrentSettingId(DISC enCol);
	int GetCurrentBoardSizeId(void);
	void UpdateGameSetting(int idBlack, int idWhite);
	void UpdateBoardSize(int idBoardSizeId);
	int GetCheckedBoardSizeId(HWND hDlg);
public:
	static INT_PTR CALLBACK CallbackLauncher_GameSetting(HWND, UINT, WPARAM, LPARAM);
	static void InitSetting(void);
	static GAME_SETTING GetCurrentGameSetting(void);
	static BOARD_SIZE GetCurrentBoardSize(void);
};