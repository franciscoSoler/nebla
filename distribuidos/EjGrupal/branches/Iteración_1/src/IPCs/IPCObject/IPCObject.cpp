#include "IPCObject.h"

namespace IPC {

IPCObject::IPCObject(std::string IPCName) : IPCName_(IPCName) {}

IPCObject::~IPCObject() {
}

std::string IPCObject::getIPCName() const {
	return IPCName_;
}

void IPCObject::setIPCName(std::string IPCName) {
	IPCName_ = IPCName;
}

} /* namespace IPC */
