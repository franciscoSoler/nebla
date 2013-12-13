#include "CommObject.h"
#include <ConfigFileParser/ConfigFileParser.h>
#include "middlewareCommon.h"
#include <Exceptions/Exception.h>
#include <memory>
#include <sstream>
#include <Logger/Logger.h>

namespace COMM {

CommObject::CommObject(std::string name)
    : commName_(name),
      commId_(0) {}

CommObject::~CommObject() {
}

std::string CommObject::getCommName() const {
    return commName_;
}

void CommObject::setCommName(std::string name) {
    commName_ = name;
}

std::string CommObject::createKey(const char* commType,
                                  const char* fileName, int id) {
    std::stringstream ss;
    ss << id;

    std::string key;
    key += commType;
    key += "-";
    key += fileName;
    key += "-";
    key += ss.str();

    return key;
}

int CommObject::findCommId(std::string key) {
    std::auto_ptr<IConfigFileParser> cfg(
    new ConfigFileParser( COMM_OBJECTS_CONFIG_FILE ) );

    cfg->parse();
    int commId = cfg->getConfigFileParam(key, -1);

    if (commId == -1) {
        throw Exception( this->commName_ +  ": - No existe ID asociado al CommObject" );
    }

    return commId;

}

}
