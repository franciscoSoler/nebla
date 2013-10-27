#ifndef ICONFIGFILEPARSER_H
#define	ICONFIGFILEPARSER_H

#include <string>

class IConfigFileParser {

public:
    virtual void parse() = 0;
    virtual int getConfigFileParam(std::string param, const int defaultValue) const = 0;
    virtual std::string getConfigFileParam(std::string param, std::string defaultValue) const = 0;
    virtual void setConfigFileParam(std::string param, const int value) = 0;
    virtual void setConfigFileParam(std::string param, std::string value) = 0;
    virtual ~IConfigFileParser() {};
};

#endif	/* ICONFIGFILEPARSER_H */

