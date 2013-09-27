/*
 * producer.cpp
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


int main(int argc, char **argv)
{
    srand (time(NULL));
    Argv arguments;

    arguments.setArgv(argv);

    int waitTime;
    int numberOfConsumers;
    int numberOfProducers;
    int producerId;
    std::string queuePath;
    char key;
    std::string keyStr;


    producerId = StringUtils::StringToNumber<int>(arguments.getArg(1));
    queuePath = arguments.getArg(2);
    keyStr = arguments.getArg(3);
    key = keyStr[0];
    numberOfConsumers = StringUtils::StringToNumber<int>(arguments.getArg(4));
    numberOfProducers = StringUtils::StringToNumber<int>(arguments.getArg(5));

    MessageQueue<Message> queue(queuePath, key);
    Message dato;
    int counter=0;

    std::cout << "Producer"<< producerId<<" started"<<std::endl;

    while(true)
    {
        waitTime = rand() % 5 + 1;
        sleep(waitTime);

        dato.id = counter * numberOfProducers + producerId;
        counter++;

        std::cout << "Producer:"<<producerId<< " produced:" <<dato.id<<std::endl;

        for(int j=1; j<=numberOfConsumers;j++)
        {
            dato.mtype = j;
            if(queue.put(dato)<0)
                exit(0);
        }
    }

    return 0;
}


