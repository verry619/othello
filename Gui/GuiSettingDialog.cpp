#include "GuiSettingDialog.h"
#include "resource.h"
#include <windowsx.h>

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
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		InitItems(hDlg);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void GuiSettingDialog::InitItems(HWND hDlg)
{
	HWND hCombo;

	hCombo = GetDlgItem(hDlg, IDC_COMBO_BLACK);
	SendMessage(hCombo, CB_INSERTSTRING, 0, (LPARAM)TEXT("HUMAN"));
	SendMessage(hCombo, CB_INSERTSTRING, 1, (LPARAM)TEXT("CPU"));
	SendMessage(hCombo, CB_SETCURSEL, 1, 0);

	hCombo = GetDlgItem(hDlg, IDC_COMBO_WHITE);
	SendMessage(hCombo, CB_INSERTSTRING, 0, (LPARAM)TEXT("HUMAN"));
	SendMessage(hCombo, CB_INSERTSTRING, 1, (LPARAM)TEXT("CPU"));
	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
}