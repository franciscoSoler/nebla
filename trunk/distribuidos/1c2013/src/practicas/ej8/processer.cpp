/*
 * proceser.cpp
 *
 */
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <assert.h>
#include "Argv.h"
#include "StringUtils.h"
#include "Sequencer.h"

int main( int argc, char **argv )
{
    srand ( time( NULL ) );
    Argv arguments;

    arguments.setArgv( argv );

    std::string processerName[] = { "Embotelladora", "Tapadora", "Etiquetadora"};
    int waitTime;
    int numberOfProcessers;
    int processerId;
    std::string queuePath;
    char key;

    processerId = StringUtils::StringToNumber<int>( arguments.getArg( 1 ) );
    queuePath = arguments.getArg( 2 );
    key = StringUtils::StringToNumber<int>(arguments.getArg( 3 ));
    numberOfProcessers = StringUtils::StringToNumber<int>( arguments.getArg( 4 ) );

    Sequencer sequencer(queuePath,key,numberOfProcessers);

    std::cout << processerName[processerId]<<" started"<<std::endl;

    std::cout <<"Queue path " << queuePath <<" key "<<key<<std::endl;

    while(true)
    {
        waitTime = rand() % 500000 + 500000;

        if(!sequencer.begin(processerId))
        	break;

		std::cout << processerName[processerId]<<" working"  << std::endl;

		usleep(waitTime);

        if(!sequencer.end(processerId))
        	break;
    }

    std::cout << processerName[processerId]<<" ended"<<std::endl;
    return 0;
}
