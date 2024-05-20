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

constexpr BOARD_SIZE DEFAULT_BOARD_SIZE = { 8,8 };

GAME_SETTING GuiSettingDialog::s_currentGameSetting = GAME_SETTING::HUMAN_HUMAN;
BOARD_SIZE GuiSettingDialog::s_currentBoardSize = DEFAULT_BOARD_SIZE;

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

	SendMessage(GetDlgItem(hDlg, GetCurrentBoardSizeId()), BM_SETCHECK, BST_CHECKED, 0);
}

INT_PTR GuiSettingDialog::ProcCmd(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDOK:
		UpdateGameSetting(
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_BLACK), CB_GETCURSEL, 0, 0),
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_WHITE), CB_GETCURSEL, 0, 0));
		UpdateBoardSize(GetCheckedBoardSizeId(hDlg));
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;

	case IDCANCEL:
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;

	default:
		break;
	}

	switch (HIWORD(wParam))
	{
#if 0
	case CBN_SELCHANGE:
	{
	}
	return (INT_PTR)TRUE;
#endif
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

int GuiSettingDialog::GetCurrentBoardSizeId(void)
{
	const std::vector < BOARD_SIZE > boardSizeList = { { 4,4 } ,{ 6,6 } ,{ 8,8 } };
	const std::vector < int > idcList = { IDC_RADIO_4X4 ,IDC_RADIO_6X6 ,IDC_RADIO_8X8 };

	for (auto i = 0;i < boardSizeList.size();i++)
	{
		if (0 == memcmp(&boardSizeList[i], &s_currentBoardSize, sizeof(BOARD_SIZE)))
		{
			return idcList[i];
		}
	}
	return -1;
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

void GuiSettingDialog::UpdateBoardSize(int idBoardSizeId)
{
	if (IDC_RADIO_4X4 == idBoardSizeId)
	{
		s_currentBoardSize = { 4,4 };
		return;
	}

	if (IDC_RADIO_6X6 == idBoardSizeId)
	{
		s_currentBoardSize = { 6,6 };
		return;
	}

	if (IDC_RADIO_8X8 == idBoardSizeId)
	{
		s_currentBoardSize = { 8,8 };
		return;
	}
}

int GuiSettingDialog::GetCheckedBoardSizeId(HWND hDlg)
{
	const std::vector < int > idcList = { IDC_RADIO_4X4 ,IDC_RADIO_6X6 ,IDC_RADIO_8X8 };

	for (auto idc : idcList) {
		if (BST_CHECKED == SendMessage(GetDlgItem(hDlg, idc), BM_GETCHECK, 0, 0))
		{
			return idc;
		}
	}

	return -1;
}

void GuiSettingDialog::InitSetting(void)
{
	s_currentGameSetting = GAME_SETTING::CPU_HUMAN;
	s_currentBoardSize = DEFAULT_BOARD_SIZE;
}

GAME_SETTING GuiSettingDialog::GetCurrentGameSetting(void)
{
	return s_currentGameSetting;
}

BOARD_SIZE GuiSettingDialog::GetCurrentBoardSize(void)
{
	return s_currentBoardSize;
}