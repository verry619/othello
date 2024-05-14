#pragma once

#include <windows.h>

class GuiSettingDialog
{
private:
	INT_PTR CALLBACK GameSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void InitItems(HWND hDlg);
public:
	static INT_PTR CALLBACK CallbackLauncher_GameSetting(HWND, UINT, WPARAM, LPARAM);
};