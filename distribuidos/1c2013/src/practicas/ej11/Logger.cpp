/* 
 * File:   Logger.cpp
 * Author: distribuidos
 * 
 * Created on April 11, 2013, 7:16 PM
 */

#include "Logger.h"
#include <sstream>
#include <unistd.h>
#include <cstdio>

Logger::Logger(std::string etiqueta) {
    this->etiqueta=etiqueta;
}
Logger::Logger() {
    this->etiqueta="process";
}

Logger::~Logger() {
}
void Logger::setEtiqueta(std::string etiqueta){
    this->etiqueta=etiqueta;
}
std::string Logger::getTime(){

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[50];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 50, "%d/%m/%Y %H:%M:%S - ", timeinfo);

	return std::string(buffer);
}

void Logger::log(std::string mensaje){
    std::stringstream entrada; 
    entrada.str("");
    entrada<<getTime()<<"["<<etiqueta<<":"<<getpid()<<"] - "<<mensaje<<"\n";
    write(fileno(stdout),entrada.str().c_str(),entrada.str().size());
    
}
void Logger::error(std::string mensaje){
    std::stringstream entrada; 
    entrada.str("");
    entrada<<getTime()<<"[ERROR]"<<"["<<etiqueta<<":"<<getpid()<<"] - "<<mensaje<<"\n";
    write(fileno(stdout),entrada.str().c_str(),entrada.str().size());
    
}
void Logger::warning(std::string mensaje){
    std::stringstream entrada; 
    entrada.str("");
    entrada<<getTime()<<"[WARNING]"<<"["<<etiqueta<<":"<<getpid()<<"] - "<<mensaje<<"\n";
    write(fileno(stdout),entrada.str().c_str(),entrada.str().size());
    
}