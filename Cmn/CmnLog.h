#pragma once

#include <string>
#include <map>

#include "Cmn.h"

#define WRITE_DEV_LOG(enId,enParam,...) CmnLog::getInstance().WriteDevLog(enId,enParam,__FILE__,__LINE__,__func__,##__VA_ARGS__)
#define WRITE_DEV_LOG_NOPARAM(enId,...) CmnLog::getInstance().WriteDevLog(enId,__FILE__,__LINE__,__func__,##__VA_ARGS__)

enum class OTHELLO_LOG_ID {
	NONE,
	GAME_START,
	GAME_QUIT,
	GAME_END,
	PUT_DISC,
	PASS_TURN,
	GAME_RESULT,
	ID_NUM
};

static const std::map<OTHELLO_LOG_ID, const char*> s_MapLogIdToStr =
{
	{OTHELLO_LOG_ID::NONE,			"NONE"},
	{OTHELLO_LOG_ID::GAME_START,	"GAME_START"},
	{OTHELLO_LOG_ID::GAME_QUIT,		"GAME_QUIT"},
	{OTHELLO_LOG_ID::GAME_END,		"GAME_END"},
	{OTHELLO_LOG_ID::PUT_DISC,		"PUT_DISC"},
	{OTHELLO_LOG_ID::PASS_TURN,		"PASS_TURN"},
	{OTHELLO_LOG_ID::GAME_RESULT,	"GAME_RESULT"},
	{OTHELLO_LOG_ID::ID_NUM,		"ID_NUM"},
};

static const std::map<DISC, char> s_MapDiscToChar =
{
	{DISC::NONE,			'N'},
	{DISC::BLACK,			'B'},
	{DISC::WHITE,			'W'},
	{DISC::OUT_OF_BOARD,	'O'},
};

typedef struct OTHELLO_LOG_PARAM_ {
	unsigned short p1;
	unsigned short p2;
	unsigned short p3;
	unsigned short p4;
}OTHELLO_LOG_PARAM;

class CmnLog final
{
private:
	std::string m_filepath_dev;
	std::string m_filepath_game;

	CmnLog();
	~CmnLog() = default;

	void MyGetCurrentTime(time_t& tt_now, std::tm& tm_now_local, uint64_t& ms_now);

public:
	CmnLog(const CmnLog&) = delete;
	CmnLog& operator=(const CmnLog&) = delete;
	CmnLog(CmnLog&&) = delete;
	CmnLog& operator=(CmnLog&&) = delete;

	static CmnLog& getInstance() {
		static CmnLog inst;
		return inst;
	}

	void WriteDevLog(const OTHELLO_LOG_ID enId,
		const char* strFile, const unsigned long& ulLine, const char* strFunc);
	void WriteDevLog(const OTHELLO_LOG_ID enId,
		const char* strFile, const unsigned long& ulLine, const char* strFunc,
		const char* strMsg);
	void WriteDevLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
		const char* strFile, const unsigned long& ulLine, const char* strFunc);
	void WriteDevLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
		const char* strFile, const unsigned long& ulLine, const char* strFunc,
		const char* strMsg);
	void WriteGameLog(const DISC_MOVE& enDiscMove, const BOARD_INFO& enBoardInfo);

	void CreateNewDevLogFile(void);
};