/*
 * Exception.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: eze
 */

#include "Exception.h"

Exception::Exception() {
	error_description_ = "";
}

Exception::Exception(std::string error_description) {
	error_description_ = error_description;
}

Exception::~Exception() {
}

std::string Exception::get_error_description() const {
	return error_description_;
}

void Exception::set_error_description(std::string error_description) {
	error_description_ = error_description;
}

const char* Exception::what() const throw() {
        return error_description_.c_str();
}