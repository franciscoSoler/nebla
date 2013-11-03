/* 
 * File:   Parser.h
 * Author: fede
 *
 * Created on October 27, 2013, 8:12 PM
 */

#ifndef PARSER_H
#define	PARSER_H

#include <iostream>
#include <fstream>
#include <cstring>

using std::ifstream;
using std::cout;
using std::endl;
using std::string;

const int MAX_CHARS_LINEA = 512;
const int MAX_TOKENS_LINEA = 20;
const char* const DELIMITADOR = ",";

class Parser
{
    private:
	char bufferLinea[MAX_CHARS_LINEA];
	int nroTokenLinea;
	
    public:
	Parser() { }
	~Parser() { }
	
	bool obtenerLineaSiguiente(ifstream& stream);
	string obtenerProximoValor();
};

#endif	/* PARSER_H */

