// Gui.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "Gui.h"
#include "GuiCom.h"
#include "GuiPainter.h"
#include "CmnLog.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6
GuiCom* g_pcCom;
static bool g_bWaitingComStart = true;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static void GuiComStartCompleted(bool bResult);
static void GuiGameStartCompleted(HWND hWnd, bool bResult);
static void GuiPutDiscCompleted(HWND hWnd, bool bResult);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	GuiComCallbackFuncs* funcs;
	funcs = new GuiComCallbackFuncs();
	funcs->funcComStart = std::bind(GuiComStartCompleted, std::placeholders::_1);
	funcs->funcGameStart = std::bind(GuiGameStartCompleted, std::placeholders::_1, std::placeholders::_2);
	funcs->funcPutDisc = std::bind(GuiPutDiscCompleted, std::placeholders::_1, std::placeholders::_2);

	g_pcCom = new GuiCom(*funcs);

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUI));

	while (g_bWaitingComStart)
	{
		NOP_FUNCTION;
	}

	MSG msg;

	// メイン メッセージ ループ:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete g_pcCom;

	return (int)msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUI);
	wcex.lpszClassName = szWindowClass;
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	g_pcCom->SetWndHundler(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_GAME_START:
		{
			GAME_SETTING enGameSetting = GAME_SETTING::CPU_HUMAN;

			OTHELLO_MSG msg;
			msg.enId = OTHELLO_MSG_ID::GAME_START;
			msg.p1 = BOARD_ROW_LEN;
			msg.p2 = BOARD_COL_LEN;
			msg.p3 = static_cast<unsigned short>(enGameSetting);
			msg.p4 = 0;

			g_pcCom->UpdateBoardSize(BOARD_ROW_LEN, BOARD_COL_LEN);
			g_pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
		}
		break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		DISC* penBoard = g_pcCom->FetchCurrentBoard();

		GuiPainter* pcPainter = new GuiPainter();
		pcPainter->DrawBoard(hWnd, hInst, BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);
		delete pcPainter;

		delete penBoard;
	}
	break;
	case WM_LBUTTONUP: //マウス左クリック
	{
		//lParamからマウス座標を取り出す
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		DISC_POS enDiscPos;
		if (GuiCalc::ConvDrawPosToDiscPos({ x, y }, enDiscPos, 6, 6))
		{
			DISC* penBoard = g_pcCom->FetchCurrentBoard();
			penBoard[enDiscPos.ucRow * BOARD_COL_LEN + enDiscPos.ucCol] = DISC::WHITE;

			GuiPainter* pcPainter = new GuiPainter();
			pcPainter->DrawBoard(hWnd, hInst, BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);
			delete pcPainter;

			delete penBoard;


			OTHELLO_MSG msg;
			msg.enId = OTHELLO_MSG_ID::PUT_DISC;
			msg.p1 = static_cast<unsigned short>(DISC::WHITE); /* <TBD> COLOR must be variant */
			msg.p2 = enDiscPos.ucRow;
			msg.p3 = enDiscPos.ucCol;

			g_pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
		}

		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

static void GuiComStartCompleted(bool bResult)
{
	if (bResult)
	{
		g_bWaitingComStart = false;
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}

static void GuiGameStartCompleted(HWND hWnd, bool bResult)
{
	if (bResult)
	{
		DISC* penBoard = g_pcCom->FetchCurrentBoard();

		GuiPainter* pcPainter = new GuiPainter();
		pcPainter->DrawBoard(hWnd, hInst, BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);
		delete pcPainter;

		delete penBoard;

		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}

static void GuiPutDiscCompleted(HWND hWnd, bool bResult)
{
	if (bResult)
	{
		DISC* penBoard = g_pcCom->FetchCurrentBoard();

		GuiPainter* pcPainter = new GuiPainter();
		pcPainter->DrawBoard(hWnd, hInst, BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);
		delete pcPainter;

		delete penBoard;

		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}