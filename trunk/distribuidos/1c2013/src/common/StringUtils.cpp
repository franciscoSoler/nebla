/*
 * StringUtils.cpp
 *
 *  Created on: 01/05/2012
 *      Author: alex
 */

#include "StringUtils.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


using namespace std;

std::vector<std::string> StringUtils::splitString(std::string sentence, char delim)
{
	vector<string> tokens;

    std::stringstream ss(sentence);
    std::string item;

    //Pido el texto hasta cada delimitador y lo inserto en el
    //vector de tokens
    while(std::getline(ss, item, delim))
    	tokens.push_back(item);

	return tokens;
}

