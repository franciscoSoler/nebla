/*
 * Date.cpp
 *
 */

#include "Date.h"
#include <sstream>

using namespace std;

Date::Date()
{
	m_Time = UNDEFINED;
}

Date::~Date()
{
}

Date::Date(const Date & orig)
{
	m_Time = orig.m_Time;
}

std::string Date::toString()
{
    struct tm *tmStruct;
    std::stringstream retStr;

    tmStruct=localtime(&m_Time);

    retStr << tmStruct->tm_mday << "/"
           << (tmStruct->tm_mon+1)<< "/"
           << (tmStruct->tm_year+1900) << " "
           << tmStruct->tm_hour << ":"
           << tmStruct->tm_min << ":"
           << tmStruct->tm_sec;

    return retStr.str();
}

void Date::setDate(time_t date)
{
	m_Time = date;
}

Date Date::now()
{
	Date retDate;
	retDate.setDate(time(NULL));
	return retDate;
}
