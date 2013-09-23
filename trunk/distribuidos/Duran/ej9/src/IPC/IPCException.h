#ifndef IPC_EXCEPTION_H_
#define IPC_EXCEPTION_H_

class IPCException {
private:
	char description[255];

public:
	IPCException(char *className, char *error);
	virtual ~IPCException();
	char* getDescription();
};

#endif /* IPC_EXCEPTION_H_ */
