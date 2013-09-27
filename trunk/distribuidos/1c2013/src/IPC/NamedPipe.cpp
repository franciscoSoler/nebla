#include "NamedPipe.h"

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

NamedPipe::NamedPipe(std::string pipePath,PipeTypeEnum type):AbstractPipe(type)
{
    m_PipePath = pipePath;
}

NamedPipe::~NamedPipe()
{
}

bool NamedPipe::create()
{
    int mkfifo_result;
    bool ret_val = true;

    mkfifo_result = mkfifo(m_PipePath.c_str(), 0666);

    if ((mkfifo_result == -1) && (errno != EEXIST))
    {
//        cout << "Error creating the named pipe"<<endl;
        ret_val = false;
    }

    return ret_val;
}

bool NamedPipe::cleanup()
{
    remove( m_PipePath.c_str());
    return true;
}

bool NamedPipe::open()
{
    bool ret_val = true;

    if(ret_val)
    {
        // Open the pipe for writing
        if(m_Fd == 0)
        {
            if(m_PipeType == WRITE)
                m_Fd = ::open(m_PipePath.c_str(), O_WRONLY|O_APPEND);
            else
                m_Fd = ::open(m_PipePath.c_str(), O_RDONLY|O_APPEND);

            if(m_Fd < 0 )
            {
//                cout << "Error while opening the pipe for writing"<<endl;
                m_Fd = 0;
                ret_val = false;
            }
        }
    }

    return ret_val;
}
