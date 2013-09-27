/*
 * consumer.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: m
 */

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "MessageQueue.h"
#include "Message.h"
#include "Argv.h"
#include "StringUtils.h"


int main( int argc, char **argv )
{
    srand ( time( NULL ) );
    Argv arguments;

    arguments.setArgv( argv );

    int waitTime;
    int numberOfConsumers;
    int numberOfProducers;
    int consumerId;
    std::string queuePath;
    char key;
    std::string keyStr;


    consumerId = StringUtils::StringToNumber<int>( arguments.getArg( 1 ) );
    queuePath = arguments.getArg( 2 );
    keyStr = arguments.getArg( 3 );
    key = keyStr[0];
    numberOfConsumers = StringUtils::StringToNumber<int>( arguments.getArg( 4 ) );

    MessageQueue<Message> queue( queuePath, key );
    Message dato;
    int counter=0;

    std::cout << "Consumer"<< consumerId<<" started"<<std::endl;

    while(true)
    {
        waitTime = rand() % 5 + 1;
        if(queue.get( consumerId, &dato )<0)
            exit(0);

        std::cout << "Consumer:" << dato.mtype << " consumed:" << dato.id << std::endl;
    }

    return 0;
}
