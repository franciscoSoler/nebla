#ifndef CONFIGFILEPARSER_H
#define	CONFIGFILEPARSER_H

#include <string>
#include <map>
#include <list>
#include "IConfigFileParser.h"
#include "ILineProcessor.h"


class ConfigFileParser : public IConfigFileParser {
public:
    ConfigFileParser(std::string configFilePath = "dist.conf");
    void parse();
    int getConfigFileParam(std::string param, const int defaultValue) const;
    std::string getConfigFileParam(std::string param, std::string defaultValue) const;
    void setConfigFileParam(std::string param, const int value);
    void setConfigFileParam(std::string param, std::string value);
    virtual ~ConfigFileParser();
    
private:
    ConfigFileParser(const ConfigFileParser& orig);
    void removeComments(std::string& line) const;
    
    std::string configFilePath_;
    std::map<std::string, std::string> dictionary_;
    std::list< ILineProcessor::ptrLineProcessor > lineProcessors_;

};

#endif	/* CONFIGFILEPARSER_H */

