#include "GamePlayer.h"

GamePlayer::GamePlayer(DISC enDiscCol)
	:m_enMyDiscCol(enDiscCol),
	m_enState(GAME_PLAYER_STATE::WAITING_TURN),
	m_unThreadId(0)
{
	(void)_beginthreadex(NULL, 0, &GamePlayer::executeLauncher, this, 0, &m_unThreadId);
}

unsigned int __stdcall GamePlayer::executeLauncher(void* args)
{
    reinterpret_cast<GamePlayer*>(args)->ThreadProc();
    return 0;
}

void GamePlayer::ThreadProc(void)
{
    MSG msg;

    while (1)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            OnRcvMsg(
                static_cast<OTHELLO_MSG_ID>(msg.message),
                static_cast<WORD>(msg.wParam),
                static_cast<DWORD>(msg.lParam));
        }
    }

    _endthreadex(0);
    return;
}

void GamePlayer::OnRcvMsg(OTHELLO_MSG_ID msg, WORD param1, DWORD param2)
{
    switch (msg)
    {
    case OTHELLO_MSG_ID::GAME_START:
        break;
    case OTHELLO_MSG_ID::GAME_QUIT:
        break;
    case OTHELLO_MSG_ID::GAME_END:
        break;
    case OTHELLO_MSG_ID::PASS_TURN:
        break;
    case OTHELLO_MSG_ID::PUT_DISC:
        break;
    default:
        break;
    }

    m_enState = GAME_PLAYER_STATE::WAITING_TURN;
}

void GamePlayer::PlayNextTurn(const DISC* penBoard)
{
	m_enState = GAME_PLAYER_STATE::MY_TURN;
}

DISC GamePlayer::GetDiscCol(void)
{
	return m_enMyDiscCol;
}