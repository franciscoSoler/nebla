#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

class LockFile {

private:

	struct flock fl;
	int fd;
	char nombre[255];

public:

	LockFile (const char* nombre);
	virtual ~LockFile();

	int takeLock ();
	int releaseLock ();
	int writeToFile( const char* buffer, int buffsize);
};

#endif /* LOCKFILE_H_ */
