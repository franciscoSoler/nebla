#ifndef PIPE_H_
#define PIPE_H_

#include "CommunicationChannel.h"
#include <iostream>

namespace pipemsg
{
    /* Read "n" bytes from a descriptor. */
    // From Unix_Network_Programming_-_Vol._II examples
    size_t readn(int fd, void *vptr, size_t n);
}

/**
 * Base class for pipes
 */
class AbstractPipe:public CommunicationChannel
{
public:
    typedef enum{READ=0,WRITE}PipeTypeEnum;

protected:

    /**
     * type of pipe (READ, WRITE)
     */
    PipeTypeEnum m_PipeType;

public:
    /**
     * Constructor
     * @param type type of pipe (READ, WRITE)
     */
    AbstractPipe(PipeTypeEnum type=READ);

    /**
     * Destructor
     */
    virtual ~AbstractPipe();

    /**
     * @brief Informs if the pipe is open.
     * @return bool true if the pipe is open, false otherwise.
     */
    bool isOpen();

    /**
     * @brief Set type of pipe
     * @param type type of pipe (READ, WRITE)
     */
    void setPipeType(PipeTypeEnum type);

    /**
     * @brief Creates the pipe.
     * @return bool true in case of success, false otherwise.
     *
     * Creates the pipe. but it does not prepare the pipe
     * for read/write operations
     * It must be implemented on every child class
     */
    virtual bool create()=0;

    /**
     * @brief Cleans up after using the pipe.
     * @return bool true in case of success, false otherwise.
     */
    virtual bool cleanup();

    /**
     * @brief Opens the pipe for communication.
     * @return bool true in case of success, false otherwise.
     *
     * Opens the pipe for communication.
     * It must be called on every process that uses the pipe
     * before using read() or write()
     * It must be reimplemented in every child class.
     */
    virtual bool open()=0;

    /**
     * @brief Closes the pipe for communication
     * @return bool true in case of success, false otherwise.
     */
    bool close();

    /**
     * @brief Receives an array of bytes from the pipe.
     * @param(in) buf array where the received bytes will be stored.
     *              It must be allocated externally.
     * @param(in) size amount of bytes to be received.
     * @param(out) buf_read number of bytes received, it can be different from
     *                 the parameter size in case of error on the pipe.
     *                 If NULL, does nothing.
     * @return bool true in case of success, false otherwise.
     *
     * Receives an array of bytes from the pipe.
     * It blocks until the specified amount of bytes is received.
     */
    bool receive(char *buf, int size, int *buf_read = NULL);

    /**
     * @brief Writes an array of bytes into the pipe.
     * @param(in) buf array of bytes to be sent
     * @param(in) size amount of bytes to be sent.
     * @param(out) buf_written number of bytes sent,it can be different from
     *                the parameter size in case of error on the pipe.
     * @return bool true in case of success, false otherwise.
     */
    bool send(char *buf, int size, int * buf_written = NULL);

    /**
     *
     */
    bool hasReadableData();

};

#endif //  PIPE_H_
