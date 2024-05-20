#include "GuiMainWnd.h"
#include "GuiPainter.h"
#include "resource.h"
#include <windowsx.h>
#include <string>

constexpr int TEXT_BLACK_POS_X = 0;
constexpr int TEXT_BLACK_POS_Y = 0;
constexpr int TEXT_BLACK_TURN_POS_X = TEXT_BLACK_POS_X + 100;
constexpr int TEXT_BLACK_TURN_POS_Y = TEXT_BLACK_POS_Y;
constexpr int TEXT_BLACK_NUM_POS_X = TEXT_BLACK_POS_X;
constexpr int TEXT_BLACK_NUM_POS_Y = TEXT_BLACK_POS_Y + 30;
constexpr int TEXT_WHITE_POS_X = 550;
constexpr int TEXT_WHITE_POS_Y = 0;
constexpr int TEXT_WHITE_TURN_POS_X = TEXT_WHITE_POS_X - 100;
constexpr int TEXT_WHITE_TURN_POS_Y = TEXT_WHITE_POS_Y;
constexpr int TEXT_WHITE_NUM_POS_X = TEXT_WHITE_POS_X;
constexpr int TEXT_WHITE_NUM_POS_Y = TEXT_WHITE_POS_Y + 30;
constexpr int TEXT_RESULT_POS_X = (TEXT_BLACK_POS_X + TEXT_WHITE_POS_X) / 2;
constexpr int TEXT_RESULT_POS_Y = TEXT_BLACK_POS_Y;

GuiMainWnd::GuiMainWnd(HINSTANCE hInstance, int nCmdShow, GuiMainWndCallbackFuncs callbacks, GuiBoardVV*& vv)
	:m_hInst(hInstance),
	m_nCmdShow(nCmdShow),
	m_callbacks(callbacks),
	m_discVV(vv)
{
	m_pcSettingDialog = new GuiSettingDialog();

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
BOOL GuiMainWnd::InitInstance(void)
{
	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, this);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, m_nCmdShow);
	UpdateWindow(m_hWnd);

	GuiSettingDialog::InitSetting();

	return TRUE;
}

void GuiMainWnd::DrawBoard(GuiBoardVV*& vv)
{
	DrawBoardForPaint(vv);
	UpdateWindow(m_hWnd);
}

void GuiMainWnd::DrawBoardForPaint(GuiBoardVV*& vv)
{
	unsigned char ucRow = vv->GetRow();
	unsigned char ucCol = vv->GetCol();

	DISC* penBoard = new DISC[ucRow * ucCol];

	for (unsigned char r = 0;r < ucRow;r++)
	{
		for (unsigned char c = 0;c < ucCol;c++)
		{
			penBoard[ucCol * r + c] = vv->GetDisc(r, c);
		}
	}

	InvalidateRect(m_hWnd, NULL, FALSE);

	GuiPainter* pcPainter = new GuiPainter();
	pcPainter->DrawBoard(m_hWnd, m_hInst, ucRow, ucCol, penBoard);
	delete pcPainter;
	delete[] penBoard;
}

void GuiMainWnd::PopupDialog(void)
{
	DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ERR_MSG_INVALID_MOVE), m_hWnd, CallbackLauncher_InvalidDiscPos);
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
			DialogBoxParam(m_hInst, MAKEINTRESOURCE(IDD_GAME_SETTING), m_hWnd, GuiSettingDialog::CallbackLauncher_GameSetting, reinterpret_cast<LPARAM>(static_cast<void*>(m_pcSettingDialog)));
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
	{
		DrawBoardForPaint(m_discVV);
		DrawTextInfo(DISC::NONE);
	}
	break;
	case WM_LBUTTONUP: //マウス左クリック
	{
		//lParamからマウス座標を取り出す
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		unsigned char ucRow = m_discVV->GetRow();
		unsigned char ucCol = m_discVV->GetCol();

		DISC_POS enDiscPos;
		if (GuiCalc::ConvDrawPosToDiscPos({ x, y }, enDiscPos, ucRow, ucCol))
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

GAME_SETTING GuiMainWnd::GetCurrentGameSetting(void)
{
	return GuiSettingDialog::GetCurrentGameSetting();
}

BOARD_SIZE GuiMainWnd::GetCurrentBoardSize(void)
{
	return GuiSettingDialog::GetCurrentBoardSize();
}

void GuiMainWnd::DrawTextInfo(DISC enTurn)
{
	DrawTextInfo(enTurn, EN_RESULT_VALID::KEEP);
}

void GuiMainWnd::DrawTextInfo(DISC enTurn, EN_RESULT_VALID bResultValid)
{
	static DISC s_enDisc = DISC::NONE;
	static bool s_bResultValid = false;

	if (EN_RESULT_VALID::VALIDATE == bResultValid)
	{
		s_bResultValid = true;
	}
	else if (EN_RESULT_VALID::INVALIDATE == bResultValid)
	{
		s_bResultValid = false;
	}

	BoardInfo board = BoardInfo(m_discVV->GetVV());
	unsigned short usBlack = 0;
	unsigned short usWhite = 0;
	DISC enWinner = board.CountDiscs(usBlack, usWhite);

	HDC hdc = GetDC(m_hWnd);
	if (s_bResultValid)
	{
		if (DISC::BLACK == enWinner)
		{
			TextOut(hdc, TEXT_RESULT_POS_X, TEXT_RESULT_POS_Y, TEXT("BLACK WIN"), 9);
		}
		else if (DISC::WHITE == enWinner)
		{
			TextOut(hdc, TEXT_RESULT_POS_X, TEXT_RESULT_POS_Y, TEXT("WHITE WIN"), 9);
		}
		else
		{
			TextOut(hdc, TEXT_RESULT_POS_X, TEXT_RESULT_POS_Y, TEXT("DRAW"), 4);
		}
		
	}
	else
	{
		TextOut(hdc, TEXT_RESULT_POS_X, TEXT_RESULT_POS_Y, TEXT("              "), 14); /* overwrite old text */
	}

	TextOut(hdc, TEXT_BLACK_POS_X, TEXT_BLACK_POS_Y, TEXT("BLACK"), 5);
	TextOut(hdc, TEXT_BLACK_NUM_POS_X, TEXT_BLACK_NUM_POS_Y, TEXT("   "), 3); /* overwrite old text */
	TextOut(hdc, TEXT_BLACK_NUM_POS_X, TEXT_BLACK_NUM_POS_Y, std::to_wstring(usBlack).data(), std::to_wstring(usBlack).length());
	TextOut(hdc, TEXT_WHITE_POS_X, TEXT_WHITE_POS_Y, TEXT("WHITE"), 5);
	TextOut(hdc, TEXT_WHITE_NUM_POS_X, TEXT_WHITE_NUM_POS_Y, TEXT("   "), 3); /* overwrite old text */
	TextOut(hdc, TEXT_WHITE_NUM_POS_X, TEXT_WHITE_NUM_POS_Y, std::to_wstring(usWhite).data(), std::to_wstring(usWhite).length());

	if (DISC::BLACK == enTurn)
	{
		s_enDisc = DISC::BLACK;
		TextOut(hdc, TEXT_BLACK_TURN_POS_X, TEXT_BLACK_TURN_POS_Y, TEXT("<-"), 2);
		TextOut(hdc, TEXT_WHITE_TURN_POS_X, TEXT_WHITE_TURN_POS_Y, TEXT("   "), 3); /* overwrite old text */
	}
	else if (DISC::WHITE == enTurn)
	{
		s_enDisc = DISC::WHITE;
		TextOut(hdc, TEXT_BLACK_TURN_POS_X, TEXT_BLACK_TURN_POS_Y, TEXT("   "), 3); /* overwrite old text */
		TextOut(hdc, TEXT_WHITE_TURN_POS_X, TEXT_WHITE_TURN_POS_Y, TEXT("->"), 2);
	}

	ReleaseDC(m_hWnd, hdc);
}