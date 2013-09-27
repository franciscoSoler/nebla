#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "../common/Object.h"

class EventHandler: public Object
{
public:
	virtual int handleSignal ( int signum ) = 0;
};

#endif /* EVENTHANDLER_H_ */
