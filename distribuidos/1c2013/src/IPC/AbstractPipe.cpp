#include "AbstractPipe.h"

#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>

using namespace std;

namespace pipemsg
{
    /* Read "n" bytes from a descriptor. */
    // From Unix_Network_Programming_-_Vol._II examples
    size_t readn(int fd, void *vptr, size_t n)
    {
        size_t  nleft;
        ssize_t nread;
        char    *ptr;

        ptr = static_cast<char *>(vptr);
        nleft = n;
        while (nleft > 0)
        {
            if ( (nread = read(fd, ptr, nleft)) < 0)
            {
                if (errno == EINTR)
                    nread = 0; /* and call read() again */
                else
                    return(-1);
            }
            else if (nread == 0)
                break; /* EOF */

            nleft -= nread;
            ptr   += nread;
        }
        return(n - nleft); /* return >= 0 */
    }
}


AbstractPipe::AbstractPipe(PipeTypeEnum type)
{
    m_Fd = 0;
    setPipeType(type);
}

AbstractPipe::~AbstractPipe()
{

}

void AbstractPipe::setPipeType(PipeTypeEnum type)
{
    m_PipeType = type;
}

bool AbstractPipe::cleanup()
{
    return true;
}

bool AbstractPipe::close()
{
    if(isOpen())
    {
        ::close(m_Fd);
        m_Fd = 0;
    }

    return true;
}

bool AbstractPipe::receive(char *buf, int size, int *buf_num_read)
{
    int numread;

    if(size <= 0)
    {
//        cout << "Size to receive must be bigger than 0" <<endl;
        return false;
    }

    if(!isOpen())
    {
//        cout << "Pipe must be open" <<endl;
        return false;
    }

    // Read from the pipe
    numread = pipemsg::readn(m_Fd, buf, size);

    if(buf_num_read != NULL)
        *buf_num_read = numread;

    if(numread <= 0 && isOpen())
    {
//        cout << "Error while reading" <<endl;
        return false;
    }

    return true;
}

bool AbstractPipe::isOpen()
{
    return m_Fd > 0;
}

bool AbstractPipe::send(char *buf, int size, int * buf_written)
{
    int numwriten;

    if(size <= 0)
    {
//        cout << "Size to receive must be bigger than 0" <<endl;
        return false;
    }

    if(!isOpen())
    {
//        cout << "Pipe must be open" <<endl;
        return false;
    }

    numwriten = ::write(m_Fd, buf, size);

    if(buf_written != NULL)
        *buf_written = numwriten;

    // Write to the pipe
    if(numwriten <= 0 && isOpen())
    {
//        cout << "Error while writing" <<endl;
        return false;
    }

    return true;
}

/**
 *
 */
bool AbstractPipe::hasReadableData()
{
    bool retVal=true;

    if(m_PipeType == READ)
    {
        fd_set rfds;
        struct timespec tv;
        int ret_sel;

        // Watch the pipe fd to see when it has input.
        FD_ZERO(&rfds);
        FD_SET(m_Fd, &rfds);

        //Set the timeout
        tv.tv_sec = 0;
        tv.tv_nsec = 1;

        ret_sel = pselect(m_Fd+1, &rfds, NULL, NULL, &tv, NULL);

        if (ret_sel == -1)
            throw new std::exception;
        else
        {
            if (ret_sel)
            	retVal = true;
            else
            	retVal = false;
        }
    }
    else
    	throw new std::exception;

    return retVal;
}
