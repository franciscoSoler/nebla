/*
 * terminator.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: m
 */




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

int main(int argc, char **argv)
{
    std::string queuePath = argv[0];
    char key = 'a';

    MessageQueue<Message> queue("./launcher", key);
    queue.destroy();

    return 0;
}
