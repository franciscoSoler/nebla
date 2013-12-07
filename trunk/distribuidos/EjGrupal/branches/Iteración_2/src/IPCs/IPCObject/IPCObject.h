#ifndef IPCOBJECT_H_
#define IPCOBJECT_H_

#include <string>

namespace IPC {

class IPCObject {


public:
	IPCObject(std::string IPCName);
        virtual ~IPCObject();
	std::string getIPCName() const;
	void setIPCName(std::string IPCName);

protected:
    std::string IPCName_;
};

} /* namespace IPC */
#endif /* IPCOBJECT_H_ */
