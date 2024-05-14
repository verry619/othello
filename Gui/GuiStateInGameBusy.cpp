#include "GuiStateInGameBusy.h"

void GuiStateInGameBusy::PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes)
{
	if (!bRes)
	{
		pcMainWnd->PopupDialog();
	}

	PutDiscComp_InGame(pcCom, pcMainWnd, bRes);
}

void GuiStateInGameBusy::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameStart_Sub(pcCom, pcMainWnd);
}