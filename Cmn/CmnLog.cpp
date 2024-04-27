#include "pch.h"
#include "CmnLog.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <ctime>

#define DEF_FILEPATH_LOG "./LOG"
#define DEF_FILENAME_DEVLOG "DEV.txt"
#define DEF_FILEPATH_GAMELOG "./LOG/GAME"
#define DEF_FILENAME_GAMELOG "GAME.txt"

void CmnLog::WriteDevLog(const OTHELLO_LOG_ID enId,
	const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc)
{
	OTHELLO_LOG_PARAM p = { 0 };
	WriteDevLog(enId, p, strFile, ulLine, strFunc, "");
}
void CmnLog::WriteDevLog(const OTHELLO_LOG_ID enId,
	const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc,
	const std::string& strMsg)
{
	OTHELLO_LOG_PARAM p = { 0 };
	WriteDevLog(enId, p, strFile, ulLine, strFunc, strMsg);
}
void CmnLog::WriteDevLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
	const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc)
{
	WriteDevLog(enId, enParam, strFile, ulLine, strFunc, "");
}

void CmnLog::WriteDevLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
	const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc,
	const std::string& strMsg)
{
	if (!(std::filesystem::exists(DEF_FILEPATH_LOG))) {
		std::filesystem::create_directory(DEF_FILEPATH_LOG);

	}

	const std::chrono::system_clock::time_point tp_now
		= std::chrono::system_clock::now();
	const std::time_t tt_now
		= std::chrono::system_clock::to_time_t(tp_now);
	std::tm ptm_now_local;
	if (0 != localtime_s(&ptm_now_local, &tt_now))
	{
		std::cerr << "ERROR!!! localtime_s()";
		return;
	}

	static std::string filepath = "";
	if ("" == filepath)
	{
		std::stringstream s;
		s << ptm_now_local.tm_year + 1900;
		s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_mon + 1;
		s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_mday;
		s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_hour;
		s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_min;
		s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_sec;
		filepath = DEF_FILEPATH_LOG + std::string("/") + s.str() + "_" + DEF_FILENAME_DEVLOG;
	}

	const uint64_t ms_now
		= std::chrono::duration_cast<std::chrono::milliseconds>(tp_now.time_since_epoch()).count();

	std::stringstream s;
	s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_hour << ":";
	s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_min << ":";
	s << std::setw(2) << std::setfill('0') << ptm_now_local.tm_sec << ".";
	s << std::setfill('0') << std::right << std::setw(3) << (ms_now % 1000) << ",";

	s << strFile << "(" << ulLine << "):" << strFunc << "()" <<
		",id=" << s_MapLogIdToStr.at(enId) <<
		",p1=" << enParam.p1 <<
		",p2=" << enParam.p2 <<
		",p3=" << enParam.p3 <<
		",p4=" << enParam.p4 <<
		"," << strMsg <<
		std::endl;

	std::cout << s.str();

	std::ofstream f(filepath, std::ios::app);

	while (!f)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		f.open(filepath, std::ios::app);
	}

	f << s.str();

	f.close();
}

void CmnLog::WriteGameLog(const OTHELLO_LOG_ID enId, const OTHELLO_LOG_PARAM enParam,
	const std::string& strFile, const unsigned long& ulLine, const std::string& strFunc)
{

}