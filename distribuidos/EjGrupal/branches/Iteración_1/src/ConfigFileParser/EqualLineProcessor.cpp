#include "EqualLineProcessor.h"
#include <algorithm>
#include <string>

EqualLineProcessor::EqualLineProcessor() {
}

void EqualLineProcessor::addToChain(ILineProcessor::ptrLineProcessor lineProcessor) {
    nextLineProcessor_ = lineProcessor;
}

std::pair<std::string, std::string> EqualLineProcessor::processLine(std::string line) {
    
    // First, remove the whitespaces
    line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());  
        
    if (numberOfEqualsInLine(line) == 1) {
        return pairOfLineWithOneEqual(line);
    }
    else if (numberOfEqualsInLine(line) > 2) {
        // Cut the string in the second equal
        line = line.substr(0, line.find("="));
        
        // Then process the line like a 'one equal' one.
        return pairOfLineWithOneEqual(line);
    }
    else {
        std::pair<std::string, std::string> pair("", "");
        return pair;
    }
}

int EqualLineProcessor::numberOfEqualsInLine(std::string line) {
    int counter = 0;
    std::string::iterator it = line.begin();
    for (; it != line.end(); ++it) {
        counter += (*it == '=') ? 1 : 0;
    }
    
    return counter;
}

std::pair<std::string, std::string> EqualLineProcessor::pairOfLineWithOneEqual(std::string line) {
    unsigned int pos = line.find("=");
    std::pair<std::string, std::string> pair("", "");
    
    if (pos != 0 && pos != std::string::npos) {
        // The line is valid
        
        if (pos - 1 == 0) {
            pair.first = line[0];
        }
        else {
            pair.first = line.substr(0, pos);    
        }
        
        if (line.size()-1 == pos +1) {
            pair.second = line[pos+1];
        }
        else {
            pair.second = line.substr(pos+1, line.size()-1);
        }
    }

    // Lowercase the pair
    std::transform(pair.first.begin(), pair.first.end(), pair.first.begin(), ::tolower);
    // std::transform(pair.second.begin(), pair.second.end(), pair.second.begin(), ::tolower);
    return pair;
}
     
ILineProcessor::ptrLineProcessor EqualLineProcessor::getNextLineProcessor() {
    return nextLineProcessor_;    
}


EqualLineProcessor::~EqualLineProcessor() {
}

