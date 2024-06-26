#pragma once

#include "GuiCom.h"
#include "GuiMainWnd.h"

#include <vector>

enum EN_GUI_STATE
{
	EN_GUI_STATE_INIT = 0,
	EN_GUI_STATE_COM_COMP,
	EN_GUI_STATE_START_WAIT,
	EN_GUI_STATE_IN_GAME_IDLE,
	EN_GUI_STATE_IN_GAME_BUSY,
	EN_GUI_STATE_GAME_RESULT,
	EN_GUI_STATE_NUM
};

class GuiState
{
protected:
	unsigned char m_ucGameRow;
	unsigned char m_ucGameCol;
	static DISC s_currentTurn;

	void UpdateBoard(const DISC* penBoard);
	void DrawBoard(GuiMainWnd*& pcMainWnd);
	void GameStart_Sub(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd);
	void GameEnd_Sub(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd);
	void ProceedNextTurn(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol);
public:
	GuiState(GuiBoardVV*& vv);
	GuiBoardVV*& m_discVV;
	BOARD_SIZE UpdateBoardSize(GuiMainWnd*& pcMainWnd);
	virtual void ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) {};
	virtual void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) {};
	virtual void GameStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) {};
	virtual void PutDisc(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC_POS enDiscPos) {};
	virtual void PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol) {};
	virtual void PassTurn(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC enDiscPos) {};
	virtual void GameEnd(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) {};
};

