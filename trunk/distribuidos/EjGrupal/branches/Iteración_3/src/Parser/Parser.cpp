/* 
 * File:   Parser.cpp
 * Author: fede
 * 
 * Created on October 28, 2013, 12:03 AM
 */

#include "Parser.h"

bool Parser::obtenerLineaSiguiente(ifstream& stream)
{
    if(stream.eof())
        return false;
    stream.getline(bufferLinea, MAX_CHARS_LINEA);
    this->nroTokenLinea = 0;
    return true;
}

string Parser::obtenerProximoValor()
{    
    char* valor = NULL;
    if(nroTokenLinea == 0)
        valor = strtok(bufferLinea, DELIMITADOR);
    else
        valor = strtok(0, DELIMITADOR);
    
    this->nroTokenLinea++;
    
    if( valor == NULL )
        return "";
    return string(valor);
}
