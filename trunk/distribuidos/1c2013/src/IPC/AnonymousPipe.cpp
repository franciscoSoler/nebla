#include "AnonymousPipe.h"
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

AnonymousPipe::AnonymousPipe()
{

}

AnonymousPipe::~AnonymousPipe()
{

}

void AnonymousPipe::setPipeEndsFd(int *fds)
{
    m_PipeEndsFd[0] = fds[0];
    m_PipeEndsFd[1] = fds[1];
}

void AnonymousPipe::getPipeEndsFd(int *fds)
{
    fds[0] = m_PipeEndsFd[0];
    fds[1] = m_PipeEndsFd[1];
}

bool AnonymousPipe::create()
{
    bool ret_val = true;

    if (pipe(m_PipeEndsFd) == -1)
    {
//        cout <<"error while creating the pipe"<<endl;
        ret_val = false;
    }

    return ret_val;
}

bool AnonymousPipe::cleanup()
{
    return true;
}

bool AnonymousPipe::open()
{
    bool ret_val = true;

    if(ret_val)
    {
        //Set the fd of the type of end,
        //and close the unused fd
        if(m_PipeType == READ)
        {
            m_Fd = m_PipeEndsFd[0];
            ::close(m_PipeEndsFd[1]);
        }
        else
        {
            m_Fd = m_PipeEndsFd[1];
            ::close(m_PipeEndsFd[0]);
        }
    }
    return ret_val;
}
