#include "GuiFsm.h"
#include "GuiStateInit.h"
#include "GuiStateComComp.h"
#include "GuiStateStartWait.h"
#include "GuiStateInGameIdle.h"
#include "GuiStateInGameBusy.h"
#include "GuiStateGameResult.h"
#include "CmnLog.h"

#define BOARD_ROW_LEN_DEFAULT 6
#define BOARD_COL_LEN_DEFAULT 6

constexpr EN_GUI_STATE GUI_STATE_ACTION_TABLE[EN_GUI_ACTION_NUM][EN_GUI_STATE_NUM] =
{
	/*                                   EN_GUI_STATE_INIT	    EN_GUI_STATE_COM_COMP	EN_GUI_STATE_START_WAIT		EN_GUI_STATE_IN_GAME_IDLE	EN_GUI_STATE_IN_GAME_BUSY	EN_GUI_STATE_GAME_RESULT*/
	/* EN_GUI_ACTION_COM_START_COMMP */	{EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_IN_GAME_BUSY,	EN_GUI_STATE_GAME_RESULT	},
	/* EN_GUI_ACTION_GAME_START */		{EN_GUI_STATE_INIT,		EN_GUI_STATE_START_WAIT,EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_START_WAIT		},
	/* EN_GUI_ACTION_GAME_START_COMP */	{EN_GUI_STATE_INIT,		EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_IN_GAME_BUSY,	EN_GUI_STATE_GAME_RESULT	},
	/* EN_GUI_ACTION_PUT_DISC */		{EN_GUI_STATE_INIT,		EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_IN_GAME_BUSY,	EN_GUI_STATE_IN_GAME_BUSY,	EN_GUI_STATE_GAME_RESULT	},
	/* EN_GUI_ACTION_PUT_DISC_COMP */	{EN_GUI_STATE_INIT,		EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_GAME_RESULT	},
	/* EN_GUI_ACTION_PASS_TURN */   	{EN_GUI_STATE_INIT,		EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_IN_GAME_IDLE,	EN_GUI_STATE_GAME_RESULT	},
	/* EN_GUI_ACTION_GAME_END_COMP */	{EN_GUI_STATE_INIT,		EN_GUI_STATE_COM_COMP,	EN_GUI_STATE_START_WAIT,	EN_GUI_STATE_GAME_RESULT,	EN_GUI_STATE_GAME_RESULT,	EN_GUI_STATE_GAME_RESULT	},
};

GuiFsm::GuiFsm(GuiComCallbackFuncs& callback_com, GuiMainWndCallbackFuncs& callback_mainWnd, HINSTANCE hInst, int nCmdShow)
	:m_currentState(EN_GUI_STATE_INIT),
	m_pcCom(new GuiCom(callback_com)),
	m_pcMainWnd(new GuiMainWnd(hInst, nCmdShow, callback_mainWnd, m_discVV)),
	m_discVV(new GuiBoardVV(BOARD_ROW_LEN_DEFAULT, BOARD_COL_LEN_DEFAULT))
{
	m_actionList[EN_GUI_ACTION_COM_START_COMMP] = std::bind(&GuiFsm::ComStartComp, this, std::placeholders::_1);
	m_actionList[EN_GUI_ACTION_GAME_START] = std::bind(&GuiFsm::GameStart, this, std::placeholders::_1);
	m_actionList[EN_GUI_ACTION_GAME_START_COMP] = std::bind(&GuiFsm::GameStartComp, this, std::placeholders::_1);
	m_actionList[EN_GUI_ACTION_PUT_DISC] = std::bind(&GuiFsm::PutDisc, this, std::placeholders::_1);
	m_actionList[EN_GUI_ACTION_PUT_DISC_COMP] = std::bind(&GuiFsm::PutDiscComp, this, std::placeholders::_1);
	m_actionList[EN_GUI_ACTION_PASS_TURN] = std::bind(&GuiFsm::PassTurn, this, std::placeholders::_1);
	m_actionList[EN_GUI_ACTION_GAME_END_COMP] = std::bind(&GuiFsm::GameEndComp, this, std::placeholders::_1);

	m_stateList[EN_GUI_STATE_INIT] = new GuiStateInit(m_discVV);
	m_stateList[EN_GUI_STATE_COM_COMP] = new GuiStateComComp(m_discVV);
	m_stateList[EN_GUI_STATE_START_WAIT] = new GuiStateStartWait(m_discVV);
	m_stateList[EN_GUI_STATE_IN_GAME_IDLE] = new GuiStateInGameIdle(m_discVV);
	m_stateList[EN_GUI_STATE_IN_GAME_BUSY] = new GuiStateInGameBusy(m_discVV);
	m_stateList[EN_GUI_STATE_GAME_RESULT] = new GuiStateGameResult(m_discVV);

	if (!m_pcMainWnd->InitInstance())
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "ERROR!!");
	}
}

void GuiFsm::DoAction(ST_ACT_MSG msg)
{
	m_actionList[msg.enAction](msg);
	m_currentState = GUI_STATE_ACTION_TABLE[msg.enAction][m_currentState];
}

void GuiFsm::ComStartComp(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->ComStartComp(m_pcCom, m_pcMainWnd);
}

void GuiFsm::GameStart(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->GameStart(m_pcCom, m_pcMainWnd);
}

void GuiFsm::GameStartComp(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->GameStartComp(m_pcCom, m_pcMainWnd);
}

void GuiFsm::PutDisc(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->PutDisc(m_pcCom, m_pcMainWnd, *static_cast<DISC_POS*>(msg.p1));
}

void GuiFsm::PutDiscComp(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->PutDiscComp(m_pcCom, m_pcMainWnd, *static_cast<bool*>(msg.p1), *static_cast<DISC*>(msg.p2));
}

void GuiFsm::PassTurn(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->PassTurn(m_pcCom, m_pcMainWnd, *static_cast<DISC*>(msg.p1));
}

void GuiFsm::GameEndComp(ST_ACT_MSG msg)
{
	m_stateList[m_currentState]->GameEndComp(m_pcCom, m_pcMainWnd);
}