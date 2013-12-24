#include "ConfigFileParser.h"
#include "EqualLineProcessor.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>


ConfigFileParser::ConfigFileParser(std::string configFilePath) {
    // Check if the file exists
    std::ifstream file(configFilePath.c_str());
    if (!file.is_open() ) {
        throw "Error: The configuration file does not exists.";
    }
    
    configFilePath_ = configFilePath;
    file.close();
    
    // Create the Filters and LineProcessors
    lineProcessors_.push_back( new EqualLineProcessor() );
}

void ConfigFileParser::parse() {
    std::ifstream file(configFilePath_.c_str());
    std::string line;
   
    while (! file. eof()) {
        getline(file, line);
        
        // First, remove the comment 
        removeComments(line);
        
        if (! line.empty()) {
            std::pair<std::string, std::string> result = (*lineProcessors_.begin())->processLine(line);
            if ( (! result.first.empty()) && (! result.second.empty()) ) {
                dictionary_.insert(result);
            }
        }
        
    }
    
    file.close();
}

void ConfigFileParser::removeComments(std::string& line) const {
    // In this version, the parser removes the contents (contenido?) of 
    // the line from the first # 
    unsigned int pos = line.find("#");
    if (pos != std::string::npos) {
        line = line.substr(0, pos);
    }
    
}

int ConfigFileParser::getConfigFileParam(std::string param, const int defaultValue) const {
    std::transform(param.begin(), param.end(), param.begin(), ::tolower);
    std::map<std::string, std::string>::const_iterator it = dictionary_.find( param );
    
    if (it != dictionary_.end()) {
        std::stringstream ss;
        ss << it->second;
        int value;
        ss >> value;
        return value;
    }
    else {
        return defaultValue;
    }
    
    
    
}

std::string ConfigFileParser::getConfigFileParam(std::string param, std::string defaultValue) const {
    std::transform(param.begin(), param.end(), param.begin(), ::tolower);
    std::map<std::string, std::string>::const_iterator it = dictionary_.find(param);
    
    if (it != dictionary_.end()) {
        return it->second;
    }
    else {
        return defaultValue;
    }
}

std::list<std::string> ConfigFileParser::getParamStringList(std::string param) const {
    std::transform(param.begin(), param.end(), param.begin(), ::tolower);
    std::list<std::string> listOfStrings;
    std::map<std::string, std::string>::const_iterator it;

    for (it = dictionary_.begin(); it != dictionary_.end(); ++it) {
        std::string key = it->first;
        key = key.substr(0, param.size());

        if (key == param) {
            listOfStrings.push_back( it->second );
        }
    }

    return listOfStrings;
}

std::list<int> ConfigFileParser::getParamIntList(std::string param) const {
    std::list<std::string> stringList = this->getParamStringList( param );
    std::list<std::string>::iterator it;
    std::list<int> intList;

    for (it = stringList.begin(); it != stringList.end(); ++it) {
        int value;
        // TODO: Check that the string is a int!!
        std::istringstream( *it ) >> value;
        intList.push_back( value );
    }

    return intList;
}

void ConfigFileParser::setConfigFileParam(std::string param, const int value) {
    // TODO
}

void ConfigFileParser::setConfigFileParam(std::string param, std::string value) {
    // TODO
}

ConfigFileParser::~ConfigFileParser() {
    std::list<ILineProcessor::ptrLineProcessor>::iterator it = lineProcessors_.begin(); 
    
    for (; it != lineProcessors_.end(); ++it) {
        delete *it;
    }
}

