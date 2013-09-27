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
    std::string queuePath = "./launcher";
    int key = 1;
    int processerAmmount = 3;

	Sequencer sequencer(queuePath,key,processerAmmount);

	sequencer.destroy();

    return 0;
}
