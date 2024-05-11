// Gui.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "Gui.h"
#include "GuiCom.h"
#include "GuiMainWnd.h"
#include "CmnLog.h"

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス

GuiCom* g_pcCom;
GuiMainWnd* g_pcMainWnd;
DISC* g_penBoard = nullptr;
static bool g_bWaitingComStart = true;

// このコード モジュールに含まれる関数の宣言を転送します:

static void CALLBACK GameStartTrigger(void);
static void CALLBACK PutDiscTrigger(DISC_POS);
static void CALLBACK UpdateBoardTrigger(void);

static void GuiComStartCompleted(bool bResult);
static void GuiGameStartCompleted(bool bResult);
static void GuiPutDiscCompleted(bool bResult);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	GuiComCallbackFuncs* funcs_GuiCom;
	funcs_GuiCom = new GuiComCallbackFuncs();
	funcs_GuiCom->funcComStart = std::bind(GuiComStartCompleted, std::placeholders::_1);
	funcs_GuiCom->funcGameStart = std::bind(GuiGameStartCompleted, std::placeholders::_1);
	funcs_GuiCom->funcPutDisc = std::bind(GuiPutDiscCompleted, std::placeholders::_1);
	g_pcCom = new GuiCom(*funcs_GuiCom);

	GuiMainWndCallbackFuncs* funcs_MainWnd;
	funcs_MainWnd = new GuiMainWndCallbackFuncs();
	funcs_MainWnd->m_callbackGameStart = std::bind(GameStartTrigger);
	funcs_MainWnd->m_callbackPutDisc = std::bind(PutDiscTrigger, std::placeholders::_1);
	funcs_MainWnd->m_callbackUpdateBoard = std::bind(UpdateBoardTrigger);
	g_pcMainWnd = new GuiMainWnd(hInst, *funcs_MainWnd);

	// アプリケーション初期化の実行:
	if (!g_pcMainWnd->InitInstance(hInst, nCmdShow))
	{
		return FALSE;
	}

	while (g_bWaitingComStart)
	{
		NOP_FUNCTION;
	}

	HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_GUI));
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
	delete g_pcMainWnd;

	return (int)msg.wParam;
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

static void GuiGameStartCompleted(bool bResult)
{
	if (bResult)
	{
		DISC* penBoard = g_pcCom->FetchCurrentBoard();

		g_pcMainWnd->DrawBoard(BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);

		delete penBoard;
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}

static void GuiPutDiscCompleted(bool bResult)
{
	if (bResult)
	{
		DISC* penBoard = g_pcCom->FetchCurrentBoard();

		g_pcMainWnd->DrawBoard(BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);

		delete penBoard;
	}
	else
	{
		g_pcMainWnd->PopupDialog();
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}

void CALLBACK GameStartTrigger(void)
{
	if (!g_penBoard)
	{
		g_penBoard = new DISC[BOARD_ROW_LEN * BOARD_COL_LEN];
		/* <TBD> Must delete g_penBoard when game ends */
	}

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
void CALLBACK PutDiscTrigger(DISC_POS enDiscPos)
{
	DISC* penBoard = g_pcCom->FetchCurrentBoard();
	penBoard[enDiscPos.ucRow * BOARD_COL_LEN + enDiscPos.ucCol] = DISC::WHITE;
	memcpy(g_penBoard, penBoard, sizeof(DISC) * BOARD_ROW_LEN * BOARD_COL_LEN);

	g_pcMainWnd->DrawBoard(BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);

	delete penBoard;

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::PUT_DISC;
	msg.p1 = static_cast<unsigned short>(DISC::WHITE); /* <TBD> COLOR must be variant */
	msg.p2 = enDiscPos.ucRow;
	msg.p3 = enDiscPos.ucCol;

	g_pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
}
void CALLBACK UpdateBoardTrigger(void)
{
	/* <TBD> Draw empty board before game start */
	DISC* penBoard = g_pcCom->FetchCurrentBoard();
	g_pcMainWnd->DrawBoard(BOARD_ROW_LEN, BOARD_COL_LEN, penBoard);
	delete penBoard;
}