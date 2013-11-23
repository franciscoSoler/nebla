#include "Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>
#include "LockFile.h"

std::string Logger::m_ProcessInf = "Generic Process N°X:";
std::string Logger::m_LogName = "Log.txt";
std::string Logger::m_BackgroundColor[] = {"\033[1;0m", "\033[1;34m", "\033[1;35m", "\033[1;33m", "\033[1;31m"};
std::string Logger::m_ForegroundColor[] = {"\033[1;0m", "\033[1;0m", "\033[1;0m", "\033[1;0m", "\033[1;43m"};

Logger& Logger::getInstance() {
	/* Variable instantiated in the first use, destroyed 
	 *  automatically in the end of the program
	 */
		static Logger m_Instance;
        return m_Instance;
}

void Logger::createLog(const std::string i_LogName) {
	if (! i_LogName.empty()) {
		m_LogName = i_LogName;
	}

	Logger::getInstance().writeMessageInLog(
	"LogFile created succesfully\n\n", std::ios::out | std::ios::trunc);
}		

void Logger::logMessage(LogLevelEnum i_Level, std::string i_Msg) {
	LockFile file = LockFile(m_LogName.c_str());

	std::stringstream ss;
    ss << currentDateTime();
	ss << m_ForegroundColor[i_Level] << m_BackgroundColor[i_Level]
	<< m_ProcessInf << " " << i_Msg << "\033[1;0m" << std::endl;

	file.takeLock();
	file.writeToFile(ss.str().c_str(), ss.str().length()); 
	file.releaseLock();
}

void Logger::writeMessageInLog(std::string i_LogMsg, std::ios_base::openmode i_OpenMode) {
	std::ofstream logFile; 
	logFile.open(m_LogName.c_str(), i_OpenMode);

	if (logFile.good())
	{
		logFile << i_LogMsg;
	}		

	logFile.close();
}

void Logger::setProcessInformation(const std::string i_ProcessInformation) {
	m_ProcessInf = i_ProcessInformation;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string Logger::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "[%Y-%m-%d]-[%X] - ", &tstruct);
    return buf;
}
