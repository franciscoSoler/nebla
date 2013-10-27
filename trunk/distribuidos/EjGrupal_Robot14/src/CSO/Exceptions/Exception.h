#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>

class Exception {
private:
	std::string error_description_;

public:
	Exception();
	explicit Exception(std::string error_description);
	virtual ~Exception();
	std::string get_error_description() const;
	void set_error_description(std::string error_description);
};

#endif /* EXCEPTION_H_ */
