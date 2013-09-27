/*
 * CommunicationChannel.h
 *
 *  Created on: 06/05/2012
 *      Author: alex
 */

#ifndef COMMUNICATIONCHANNEL_H_
#define COMMUNICATIONCHANNEL_H_

#include "../common/Object.h"

class CommunicationChannel: public Object
{
protected:
    /**
     * Pipe file descriptor, to be used
     * for every read/write operation
     */
    int m_Fd;

public:
	CommunicationChannel();
	virtual ~CommunicationChannel();

	int getFd();
};

#endif /* COMMUNICATIONCHANNEL_H_ */
