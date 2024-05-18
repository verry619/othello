// Gui.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "Gui.h"
#include "GuiFsm.h"
#include "GuiCom.h"
#include "GuiMainWnd.h"
#include "CmnLog.h"

GuiFsm* g_pcFsm;
static bool g_bWaitingComStart = true;

// このコード モジュールに含まれる関数の宣言を転送します:

static void CALLBACK GameStartTrigger(void);
static void CALLBACK PutDiscTrigger(DISC_POS);

static void GuiComStartCompleted(bool bResult);
static void GuiGameStartCompleted(bool bResult);
static void GuiPutDiscCompleted(bool bResult, DISC enDiscCol);
static void GuiPassTurn(DISC enDiscCol);
static void GuiGameEnd(void);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HINSTANCE hInst = hInstance;

	GuiComCallbackFuncs funcs_GuiCom;
	funcs_GuiCom.funcComStart = std::bind(GuiComStartCompleted, std::placeholders::_1);
	funcs_GuiCom.funcGameStart = std::bind(GuiGameStartCompleted, std::placeholders::_1);
	funcs_GuiCom.funcPutDisc = std::bind(GuiPutDiscCompleted, std::placeholders::_1, std::placeholders::_2);
	funcs_GuiCom.funcPassTurn = std::bind(GuiPassTurn, std::placeholders::_1);
	funcs_GuiCom.funcGameEnd = std::bind(GuiGameEnd);

	GuiMainWndCallbackFuncs funcs_MainWnd;
	funcs_MainWnd.m_callbackGameStart = std::bind(GameStartTrigger);
	funcs_MainWnd.m_callbackPutDisc = std::bind(PutDiscTrigger, std::placeholders::_1);

	g_pcFsm = new GuiFsm(funcs_GuiCom, funcs_MainWnd, hInst, nCmdShow);

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

	return (int)msg.wParam;
}

static void GuiComStartCompleted(bool bResult)
{
	if (bResult)
	{
		g_pcFsm->DoAction({ EN_GUI_ACTION_COM_START_COMMP,0,0,0,0 });
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
		g_pcFsm->DoAction({ EN_GUI_ACTION_GAME_START_COMP,0,0,0,0 });
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}

static void GuiPutDiscCompleted(bool bResult, DISC enDiscCol)
{
	g_pcFsm->DoAction({ EN_GUI_ACTION_PUT_DISC_COMP,&bResult,&enDiscCol,0,0 });
}

static void GuiPassTurn(DISC enDiscCol)
{
	g_pcFsm->DoAction({ EN_GUI_ACTION_PASS_TURN,&enDiscCol,0,0,0 });
}

static void GuiGameEnd(void)
{
	g_pcFsm->DoAction({ EN_GUI_ACTION_GAME_END, 0,0,0,0 });
}

void CALLBACK GameStartTrigger(void)
{
	g_pcFsm->DoAction({ EN_GUI_ACTION_GAME_START,0,0,0,0 });
}

void CALLBACK PutDiscTrigger(DISC_POS enDiscPos)
{
	g_pcFsm->DoAction({ EN_GUI_ACTION_PUT_DISC, &enDiscPos,0,0,0 });
}
