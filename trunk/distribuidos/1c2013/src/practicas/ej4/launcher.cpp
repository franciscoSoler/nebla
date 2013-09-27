/*
 * launcher.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: m
 */
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "MessageQueue.h"
#include "Message.h"
#include "Argv.h"
#include "ProcessAux.h"

int main(int argc, char **argv)
{
    std::string queuePath = "./launcher";
    char key = 'a';
    std::stringstream keyStr;

    keyStr<<key;

    std::string producerPath = "./producer";
    std::string consumerPath = "./consumer";

    int numberOfConsumers= 3;
    int numberOfProducers= 5;
    std::stringstream numberOfConsumersStr;
    std::stringstream numberOfProducersStr;

    numberOfConsumersStr << numberOfConsumers;
    numberOfProducersStr << numberOfProducers;

    MessageQueue<Message> queue(queuePath, key);
    queue.destroy();

	Argv producerArguments;

	producerArguments.pushArg(queuePath);
	producerArguments.pushArg(keyStr.str());
	producerArguments.pushArg(numberOfConsumersStr.str());
	producerArguments.pushArg(numberOfProducersStr.str());
	ProcessAux::launchProcess(producerPath,numberOfProducers,producerArguments);

	Argv consumerArguments;

	consumerArguments.pushArg(queuePath);
	consumerArguments.pushArg(keyStr.str());
	consumerArguments.pushArg(numberOfConsumersStr.str());
	consumerArguments.pushArg(numberOfProducersStr.str());
	ProcessAux::launchProcess(consumerPath,numberOfProducers,consumerArguments);

    wait();
    return 0;
}
