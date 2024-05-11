#pragma once

#include "framework.h"
#include "Cmn.h"
#include <functional>

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
	HWND m_hWnd;
	WCHAR m_szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
	GuiMainWndCallbackFuncs m_callbacks;

	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK CallbackLauncher_WndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK CallbackLauncher_GameSetting(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK GameSetting(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK CallbackLauncher_InvalidDiscPos(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK InvalidDiscPos(HWND, UINT, WPARAM, LPARAM);

public:
	GuiMainWnd(HINSTANCE hInstance, GuiMainWndCallbackFuncs callbacks);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	void DrawBoard(unsigned char ucRow, unsigned char ucCol, DISC* penBoard);
	void PopupDialog(void);
};

