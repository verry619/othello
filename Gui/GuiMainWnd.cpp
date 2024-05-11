#include "GuiMainWnd.h"
#include "GuiPainter.h"
#include "resource.h"
#include <windowsx.h>

GuiMainWnd::GuiMainWnd(HINSTANCE hInstance, GuiMainWndCallbackFuncs callbacks)
	:m_hInst(hInstance),
	m_callbacks(callbacks)
{
	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUI, m_szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
}


//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM GuiMainWnd::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CallbackLauncher_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUI);
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL GuiMainWnd::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, this);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

void GuiMainWnd::DrawBoard(unsigned char ucRow, unsigned char ucCol, DISC* penBoard)
{
	GuiPainter* pcPainter = new GuiPainter();
	pcPainter->DrawBoard(m_hWnd, m_hInst, ucRow, ucCol, penBoard);
	delete pcPainter;

	InvalidateRect(m_hWnd, NULL, FALSE);
	UpdateWindow(m_hWnd);
}

void GuiMainWnd::PopupDialog(void)
{
	DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ERR_MSG_INVALID_MOVE), m_hWnd, CallbackLauncher_InvalidDiscPos);
}

LRESULT CALLBACK GuiMainWnd::CallbackLauncher_GameSetting(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiMainWnd* app;
	if (message == WM_CREATE) {
		app = (GuiMainWnd*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (GuiMainWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
	return app->GameSetting(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK GuiMainWnd::GameSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

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

LRESULT CALLBACK GuiMainWnd::CallbackLauncher_InvalidDiscPos(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiMainWnd* app;
	if (message == WM_CREATE) {
		app = (GuiMainWnd*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (GuiMainWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
	return app->InvalidDiscPos(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK GuiMainWnd::InvalidDiscPos(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

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

LRESULT CALLBACK GuiMainWnd::CallbackLauncher_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiMainWnd* app;

	if (message == WM_CREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		app = reinterpret_cast<GuiMainWnd*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (GuiMainWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (app)
	{
		return app->WndProc(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK GuiMainWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int x = 0;
	static int y = 0;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case ID_GAME_START:
			m_callbacks.m_callbackGameStart();
			break;
		case ID_GAME_SETTING:
			DialogBox(m_hInst, MAKEINTRESOURCE(IDD_GAME_SETTING), m_hWnd, CallbackLauncher_GameSetting);
			break;
		case IDM_EXIT:
			DestroyWindow(m_hWnd);
			break;
		default:
			return DefWindowProc(m_hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
		m_callbacks.m_callbackUpdateBoard();
		break;
	case WM_LBUTTONUP: //マウス左クリック
	{
		//lParamからマウス座標を取り出す
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		DISC_POS enDiscPos;
		if (GuiCalc::ConvDrawPosToDiscPos({ x, y }, enDiscPos, 6, 6))
		{
			m_callbacks.m_callbackPutDisc(enDiscPos);
		}

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}