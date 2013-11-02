#ifndef IPCOBJECT_H_
#define IPCOBJECT_H_

#include <string>

namespace IPC {

class IPCObject {
protected:
	std::string IPCName_;

public:
	IPCObject(std::string IPCName);
	std::string getIPCName() const;
	void setIPCName(std::string IPCName);
};

} /* namespace IPC */
#endif /* IPCOBJECT_H_ */
