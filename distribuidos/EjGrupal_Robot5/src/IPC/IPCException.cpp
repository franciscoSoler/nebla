/* 
 * File:   IPCException.cpp
 * Author: knoppix
 * 
 * Created on October 31, 2013, 9:43 PM
 */

#include "IPCException.h"

#include <string.h>

IPCException::IPCException(const char *className, const char *error) {
    strcpy(description, className);
    strcat(description, ": ");
    strcat(description, error);
}

const char* IPCException::what() const throw () {
    return description;
}