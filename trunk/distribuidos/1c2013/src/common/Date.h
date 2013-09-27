/*
 * Date.h
 *
 */

#ifndef DATE_H_
#define DATE_H_

#include "Object.h"
#include <time.h>
#include <iostream>

class Date: public Object
{
public:
	Date();
	Date(const Date &orig);
	virtual ~Date();

	void setDate(time_t date);

	static Date now();

	std::string toString();
private:
	time_t m_Time;
};

#endif /* DATE_H_ */
