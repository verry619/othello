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
	ID_NUM
};

static const std::map<OTHELLO_LOG_ID, std::string> s_MapLogIdToStr =
{
	{OTHELLO_LOG_ID::GAME_START,	std::string("GAME_START")},
	{OTHELLO_LOG_ID::GAME_QUIT,		std::string("GAME_QUIT")},
	{OTHELLO_LOG_ID::GAME_END,		std::string("GAME_END")},
	{OTHELLO_LOG_ID::PUT_DISC,		std::string("PUT_DISC")},
	{OTHELLO_LOG_ID::PASS_TURN,		std::string("PASS_TURN")},
	{OTHELLO_LOG_ID::ID_NUM,		std::string("ID_NUM")},
};

static const std::map<DISC, char> s_MapDiscToChar =
{
	{DISC::NONE,			'N'},
	{DISC::BLACK,			'B'},
	{DISC::WHITE,			'W'},
	{DISC::OUT_OF_BOARD,	'O'},
};

typedef struct OTHELLO_LOG_PARAM_ {
	unsigned int p1;
	unsigned int p2;
	unsigned int p3;
	unsigned int p4;
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
		const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc);
	void WriteDevLog(const OTHELLO_LOG_ID enId,
		const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc,
		const std::string& strMsg);
	void WriteDevLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
		const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc);
	void WriteDevLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
		const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc,
		const std::string& strMsg);
	void WriteGameLog(const DISC_MOVE& enDiscMove, const BOARD_INFO& enBoardInfo);

	void CreateNewDevLogFile(void);
};