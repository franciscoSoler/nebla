/*
 * launcher.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: m
 */
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "MessageQueue.h"
#include "Message.h"
#include "Semaphore.h"
#include "Argv.h"
#include "ProcessAux.h"
#include "Sequencer.h"

int main(int argc, char **argv)
{
    std::string queuePath = argv[0];
    int key = 1;
    bool doRun=true;

    int processerAmmount = 3;

    Argv producerArguments;

	producerArguments.pushArg(queuePath);
	producerArguments.pushArg(key);
	producerArguments.pushArg(processerAmmount);

	Sequencer sequencer(queuePath,key,processerAmmount);

	sequencer.init();

	ProcessAux::launchProcess("./processer",processerAmmount,producerArguments);

    int status;
    wait(&status);
    return 0;
}
