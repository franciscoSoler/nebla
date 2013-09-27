/* 
 * File:   Logger.h
 * Author: distribuidos
 *
 * Created on April 11, 2013, 7:16 PM
 */

#ifndef LOGGER_H
#define	LOGGER_H
#include <string>
class Logger {
public:
    Logger(std::string etiqueta);
    Logger();
    void setEtiqueta(std::string etiqueta);
    void error(std::string mensaje);
    void warning(std::string mensaje);
    void log(std::string mensaje);
    
    virtual ~Logger();
private:
    std::string getTime();
    std::string etiqueta;
  
    

};

#endif	/* LOGGER_H */

