#include "GuiSettingDialog.h"
#include "resource.h"
#include <windowsx.h>

enum PLAYER_TYPE
{
	PLAYER_TYPE_HUMAN = 0,
	PLAYER_TYPE_CPU,
	PLAYER_TYPE_NUM
};

static const wchar_t* PLAYER_TEXT_LIST[PLAYER_TYPE_NUM] =
{
	TEXT("HUMAN"),
	TEXT("CPU")
};

GAME_SETTING GuiSettingDialog::s_currentGameSetting = GAME_SETTING::HUMAN_HUMAN;

LRESULT CALLBACK GuiSettingDialog::CallbackLauncher_GameSetting(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiSettingDialog* app;

	if (message == WM_INITDIALOG) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		app = reinterpret_cast<GuiSettingDialog*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (GuiSettingDialog*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (app)
	{
		return app->GameSetting(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

INT_PTR CALLBACK GuiSettingDialog::GameSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		InitItems(hDlg);
		break;

	case WM_COMMAND:
		return ProcCmd(hDlg, wParam, lParam);
	}
	return (INT_PTR)FALSE;
}

void GuiSettingDialog::InitItems(HWND hDlg)
{
	HWND hCombo;

	hCombo = GetDlgItem(hDlg, IDC_COMBO_BLACK);
	for (int i = 0;i < PLAYER_TYPE_NUM;i++)
	{
		SendMessage(hCombo, CB_INSERTSTRING, i, (LPARAM)PLAYER_TEXT_LIST[i]);
	}
	SendMessage(hCombo, CB_SETCURSEL, GetCurrentSettingId(DISC::BLACK), 0);

	hCombo = GetDlgItem(hDlg, IDC_COMBO_WHITE);
	for (int i = 0;i < PLAYER_TYPE_NUM;i++)
	{
		SendMessage(hCombo, CB_INSERTSTRING, i, (LPARAM)PLAYER_TEXT_LIST[i]);
	}
	SendMessage(hCombo, CB_SETCURSEL, GetCurrentSettingId(DISC::WHITE), 0);
}

INT_PTR GuiSettingDialog::ProcCmd(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDOK:
	case IDCANCEL:
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;

	default:
		break;
	}

	switch (HIWORD(wParam))
	{
	case CBN_SELCHANGE:
	{
		UpdateGameSetting(
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLACK), CB_GETCURSEL, 0, 0),
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_WHITE), CB_GETCURSEL, 0, 0));
	}
	return (INT_PTR)TRUE;

	default:
		return (INT_PTR)FALSE;
	}
}

int GuiSettingDialog::GetCurrentSettingId(DISC enCol)
{
	if (DISC::BLACK == enCol)
	{
		switch (s_currentGameSetting)
		{
		case GAME_SETTING::CPU_CPU:
		case GAME_SETTING::CPU_HUMAN:
			return PLAYER_TYPE_CPU;
		case GAME_SETTING::HUMAN_CPU:
		case GAME_SETTING::HUMAN_HUMAN:
			return PLAYER_TYPE_HUMAN;
		default:
			return -1;
		}
	}
	else if (DISC::WHITE == enCol)
	{
		switch (s_currentGameSetting)
		{
		case GAME_SETTING::CPU_CPU:
		case GAME_SETTING::HUMAN_CPU:
			return PLAYER_TYPE_CPU;
		case GAME_SETTING::CPU_HUMAN:
		case GAME_SETTING::HUMAN_HUMAN:
			return PLAYER_TYPE_HUMAN;
		default:
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void GuiSettingDialog::UpdateGameSetting(int idBlack, int idWhite)
{
	if (PLAYER_TYPE_HUMAN == idBlack)
	{
		if (PLAYER_TYPE_HUMAN == idWhite)
		{
			s_currentGameSetting = GAME_SETTING::HUMAN_HUMAN;
		}
		else
		{
			s_currentGameSetting = GAME_SETTING::HUMAN_CPU;
		}
	}
	else
	{
		if (PLAYER_TYPE_HUMAN == idWhite)
		{
			s_currentGameSetting = GAME_SETTING::CPU_HUMAN;
		}
		else
		{
			s_currentGameSetting = GAME_SETTING::CPU_CPU;
		}
	}
}

void GuiSettingDialog::InitSetting(void)
{
	s_currentGameSetting = GAME_SETTING::CPU_HUMAN;
}

GAME_SETTING GuiSettingDialog::GetCurrentGameSetting(void)
{
	return s_currentGameSetting;
}