#pragma once

#include "framework.h"
#include "Cmn.h"
#include "GuiSettingDialog.h"
#include "GuiBoardVV.h"

#include <functional>
#include <vector>

constexpr int MAX_LOADSTRING = 100;

class GuiMainWndCallbackFuncs
{
	using callbackGameStart_type = std::function<void(void)>;
	using callbackPutDisc_type = std::function<void(DISC_POS)>;
	using callbackUpdateBoard_type = std::function<void(void)>;
public:
	callbackGameStart_type m_callbackGameStart;
	callbackPutDisc_type m_callbackPutDisc;
	callbackUpdateBoard_type m_callbackUpdateBoard;
};

class GuiMainWnd
{
private:
	HINSTANCE m_hInst;
	int m_nCmdShow;
	HWND m_hWnd;
	WCHAR m_szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
	GuiMainWndCallbackFuncs m_callbacks;
	GuiBoardVV*& m_discVV;

	GuiSettingDialog* m_pcSettingDialog;

	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK CallbackLauncher_WndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK CallbackLauncher_InvalidDiscPos(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK InvalidDiscPos(HWND, UINT, WPARAM, LPARAM);

public:
	GuiMainWnd(HINSTANCE hInstance, int nCmdShow, GuiMainWndCallbackFuncs callbacks, GuiBoardVV*& vv);
	BOOL InitInstance(void);
	void DrawBoard(GuiBoardVV*& vv);
	void DrawBoardForPaint(GuiBoardVV*& vv);
	void PopupDialog(void);
	GAME_SETTING GetCurrentGameSetting(void);
};

