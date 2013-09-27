#ifndef NAMED_PIPE_H_
#define NAMED_PIPE_H_

#include <iostream>
#include "AbstractPipe.h"
/**
 * Encapsulates named pipes
 * @see man 3 mkfifo
 */
class NamedPipe:public AbstractPipe
{
private:
    /**
     * Directory in which the queue will be created
     */
    std::string m_PipeDir;

    /**
     * Name of the pipe
     */
    std::string m_PipeName;

    /**
     * Path to the pipe in the filesystem
     */
    std::string m_PipePath;

public:
    /**
     * Constructor
     */
    NamedPipe(std::string pipePath,PipeTypeEnum type=READ);

    /**
     * Destructor
     */
    virtual ~NamedPipe();


    /**
     * Creates the named pipe in the file system
     * @see man 3 mkfifo
     * @return bool true in case of success, false otherwise.
     */
    virtual bool create();

    /**
     * Opens the pipe for comunication.
     * It must be called on every process that uses the pipe
     * before using read() or write().
     * @return bool true in case of success, false otherwise.
     */
    virtual bool open();

    /**
     * Cleans up after using the pipe.
     * It deletes the named pipe with pipe_name from the filesystem.
     * @return bool true in case of success, false otherwise.
     */
    virtual bool cleanup();

    /**
     *
     */
    std::string getPipeName();
};

#endif //#ifndef _NAMED_PIPE_H_
