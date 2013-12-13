#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <exception>

class Exception : public std::exception {
private:
	std::string error_description_;

public:
	Exception();
	explicit Exception(std::string error_description);
        virtual ~Exception() throw();
        
	std::string get_error_description() const;
	void set_error_description(std::string error_description);
        
        const char* what() const throw();
};

#endif /* EXCEPTION_H_ */
