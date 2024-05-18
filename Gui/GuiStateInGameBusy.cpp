#include "GuiStateInGameBusy.h"

void GuiStateInGameBusy::PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol)
{
	if (!bRes)
	{
		pcMainWnd->PopupDialog();
	}

	ProceedNextTurn(pcCom, pcMainWnd, bRes, enDiscCol);
}

void GuiStateInGameBusy::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameStart_Sub(pcCom, pcMainWnd);
}

void GuiStateInGameBusy::PassTurn(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC enDiscCol)
{
	ProceedNextTurn(pcCom, pcMainWnd, true, enDiscCol);
}

void GuiStateInGameBusy::GameEnd(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameEnd_Sub(pcCom, pcMainWnd);
}