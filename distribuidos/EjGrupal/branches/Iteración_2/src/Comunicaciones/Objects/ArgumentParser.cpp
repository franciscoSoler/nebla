#include "ArgumentParser.h"
#include <stdio.h>
#include <string.h>

ArgumentParser::ArgumentParser(int argc, char** argv) 
        : argc_(argc),
          argv_(argv) {}


int ArgumentParser::parseArgument(int index, int & argument) {
    if (isIndexValid(index)) {
        sscanf(argv_[index], "%d", &argument);
    }
    else {
        return -1;
    }
    return 0; 
}

int ArgumentParser::parseArgument(int index, long & argument) {
    if (isIndexValid(index)) {
        sscanf(argv_[index], "%ld", &argument);
    }
    else {
        return -1;
    }
    return 0; 
}

int ArgumentParser::parseArgument(int index, char argument[], 
                                   int argumentSize) {
    if (isIndexValid(index)) {
        if ( strlen(argv_[index]) > static_cast<size_t>(argumentSize) ) {
            return -1;
        }
        else {
            strcpy(argument, argv_[index]);
        }
    }
    else {
        return -1;
    }
    return 0; 
}
