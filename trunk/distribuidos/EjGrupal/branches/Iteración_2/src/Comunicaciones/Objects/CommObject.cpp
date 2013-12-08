#include "CommObject.h"
#include <ConfigFileParser/ConfigFileParser.h>
#include "middlewareCommon.h"
#include <Exceptions/Exception.h>
#include <memory>
#include <Logger/Logger.h>

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

void CommObject::findCommId(std::string key) {
    std::auto_ptr<IConfigFileParser> cfg(
    new ConfigFileParser( COMM_OBJECTS_CONFIG_FILE ) );

    cfg->parse();
    commId_ = cfg->getConfigFileParam(key, -1);

    if (commId_ == -1) {
        throw Exception( "No existe ID asociado al CommObject" );
    }

}


