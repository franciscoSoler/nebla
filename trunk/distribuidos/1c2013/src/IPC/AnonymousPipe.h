#ifndef ANONYMOUS_PIPE_H_
#define ANONYMOUS_PIPE_H_

#include <iostream>
#include "AbstractPipe.h"

/**
 * Encapsulates an anonymous pipe.
 * This sort of pipe must only be used between a father and a
 * child process.
 *
 * For pipes that can be used from unrelated processes, @see NamedPipe
 * @see man 2 pipe
 */
class AnonymousPipe:public AbstractPipe
{
private:
    /**
     * Pipe file descriptors result of a pipe()
     * call
     * pipe[0] READ  end
     * pipe[1] WRITE end
     */
    int m_PipeEndsFd[2];

public:
    /**
     * Constructor
     */
    AnonymousPipe();

    /**
     * Destructor
     */
    virtual ~AnonymousPipe();

    /**
     * @brief Set the pipe's file descriptors.
     * @param fds file descriptors to be used for the pipe.
     * @see create()
     *
     * Set the pipe's file descriptors from an array
     * of 2 file descriptors.
     * The file descriptors should be opened previously
     * for the process using the pipe, by calling
     * create()
     * The pipe is in charge of administrating the fds
     * once they've been set
     * If invalid file descriptors have been assigned,
     * the pipe will throw an exception while doing
     * read() or write()
     */
    void setPipeEndsFd(int *fds);

    /**
     * Get the pipe's file descriptors from an internal
     * of 2 file descriptors.
     * It should be called in order to pass the pipe's
     * fds to a child process.
     * The file descriptors should be opened previously
     * for the process using the pipe, by calling
     * create()
     * @param fds file descriptors to be used for the pipe.
     */
    void getPipeEndsFd(int *fds);

    /**
     * Create the file descriptors for both ends of the pipe.
     * Once created one process must use the write end
     * and the other the read end in order to transfer information
     * It uses the pipe() syscall.
     * It must be called only once, before creating a child
     * process.
     * @return bool true in case of success, false otherwise.
     * @see man 2 pipe
     */
    virtual bool create();

    /**
     * @brief Opens the pipe for comunication.
     * @return bool true in case of success, false otherwise.
     *
     * Opens the pipe for comunication.
     * It must be called on every process that uses the pipe
     * before using read() or write()
     */
    virtual bool open();

    /**
     * Cleans up after using the pipe.
     * @return bool true in case of success, false otherwise.
     */
    virtual bool cleanup();
};

#endif // ANONYMOUSPIPE_H_
