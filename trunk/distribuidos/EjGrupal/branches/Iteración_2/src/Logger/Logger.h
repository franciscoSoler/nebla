#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include <string>

				
class Logger {

public:
    typedef enum {	TRACE = 0,
                    IMPORTANT,
                    DEBUG,
                    COMM,
                    ERROR

    } LogLevelEnum;
    
	static Logger & getInstance(); 
	static void logMessage(const LogLevelEnum i_Level, std::string i_Msg);
	/* Creates a new log with the name speficied. If the log already exists
	 * it is destroyed.
 	 */
	static void createLog(const std::string i_LogName = "");
	/*
	 *
	 */
	static void setProcessInformation(const std::string i_ProcessInformation);

private:
	/* Had to make private the default constructor, the 
	 * copy constructor and the operator =
	 */
	Logger() {}
	Logger(Logger const &);
	void operator = (Logger const &);
	static void writeMessageInLog(const std::string, std::ios_base::openmode);	
    static std::string currentDateTime();

private:
	static std::string m_LogName;
	static std::string m_ProcessInf;
	static std::string m_BackgroundColor[];
	static std::string m_ForegroundColor[];
	
};

#endif // _LOGGER_H_
