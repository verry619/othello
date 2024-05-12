#pragma once

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
public:
	virtual void ComStartComp(void) {};
	virtual void GameStart(void) {};
};

