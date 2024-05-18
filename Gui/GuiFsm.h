#pragma once

#include "GuiState.h"
#include "GuiCom.h"
#include "GuiMainWnd.h"
#include "GuiBoardVV.h"

#include <functional>

enum EN_GUI_ACTION
{
	EN_GUI_ACTION_COM_START_COMMP = 0,
	EN_GUI_ACTION_GAME_START,
	EN_GUI_ACTION_GAME_START_COMP,
	EN_GUI_ACTION_PUT_DISC,
	EN_GUI_ACTION_PUT_DISC_COMP,
	EN_GUI_ACTION_PASS_TURN,
	EN_GUI_ACTION_GAME_END_COMP,
	EN_GUI_ACTION_NUM
};

struct ST_ACT_MSG
{
	EN_GUI_ACTION enAction;
	void* p1;
	void* p2;
	void* p3;
	void* p4;
};

class GuiFsm
{
private:
	EN_GUI_STATE m_currentState;
	GuiCom* m_pcCom;
	GuiMainWnd* m_pcMainWnd;
	GuiState* m_stateList[EN_GUI_STATE_NUM];
	std::function<void(ST_ACT_MSG)> m_actionList[EN_GUI_ACTION_NUM];
	GuiBoardVV* m_discVV;

	void ComStartComp(ST_ACT_MSG msg);
	void GameStart(ST_ACT_MSG msg);
	void GameStartComp(ST_ACT_MSG msg);
	void PutDisc(ST_ACT_MSG msg);
	void PutDiscComp(ST_ACT_MSG msg);
	void PassTurn(ST_ACT_MSG msg);
	void GameEndComp(ST_ACT_MSG msg);

public:
	GuiFsm(GuiComCallbackFuncs& callback_com, GuiMainWndCallbackFuncs& callback_mainWnd, HINSTANCE hInst, int nCmdShow);
	void DoAction(ST_ACT_MSG msg);
};

