#pragma once

#include "GuiState.h"

#include <functional>

enum EN_GUI_ACTION
{
	EN_GUI_ACTION_COM_START_COMMP = 0,
	EN_GUI_ACTION_GAME_START,
	EN_GUI_ACTION_GAME_START_COMP,
	EN_GUI_ACTION_PUT_DISC,
	EN_GUI_ACTION_PUT_DISC_COMP,
	EN_GUI_ACTION_GAME_END_COMP,
	EN_GUI_ACTION_NUM
};

class GuiFsm
{
private:
	EN_GUI_STATE m_currentState;
	GuiState* m_stateList[EN_GUI_STATE_NUM];
	std::function<void(void)> m_actionList[EN_GUI_ACTION_NUM];

	void ComStartComp(void);
	void GameStart(void);

public:
	GuiFsm(void);
	void DoAction(EN_GUI_ACTION enAction);
};

