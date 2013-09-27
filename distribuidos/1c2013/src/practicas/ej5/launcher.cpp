/*
 * launcher.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: m
 */
#include <assert.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "MessageQueue.h"
#include "SharedMemory.h"
#include "Semaphore.h"
#include "Message.h"
#include "Argv.h"
#include "ProcessAux.h"
#include "EventHandler.h"
#include "SignalHandler.h"

class SIGINT_Handler : public EventHandler
{
    private:
		bool & m_doRun;
    public:

        SIGINT_Handler (bool &doRun): m_doRun(doRun)
        {
        }

        virtual int handleSignal ( int signum )
        {
            assert ( signum == SIGINT );

            m_doRun = false;

            return 0;
        }

};

int main(int argc, char **argv)
{
	srand(time(NULL));
    char key = 'a';
    int bufferSize = 5;
    std::string semPath = argv[0];
    std::stringstream keyStr;

    keyStr<<key;

    std::string producerPath = "./producer";
    std::string consumerPath = "./consumer";

    SharedMemory<int> buffer(bufferSize);
    SharedMemory<int> counter;
    SharedMemory<int> inPtr;
    SharedMemory<int> outPtr;

    Semaphore mutex( semPath,1, 1);
    Semaphore notFull( semPath,2, 0);
    Semaphore notEmpty( semPath,3, 0);

    mutex.init();
    notFull.init();
    notEmpty.init();

    buffer.create(semPath, 'a');
    counter.create(semPath, 'b');
    inPtr.create(semPath, 'c');
    outPtr.create(semPath, 'd');

    inPtr.write(0);
    outPtr.write(0);
    counter.write(0);

    pid_t pid=fork();

    if(pid <0)
    {
    	std::cout << "Error"<<std::endl;
    	exit(1);
    }

    //Producer
    if(pid == 0)
    {
    	std::cout <<"Arranca productor"<<std::endl;
    	int number = 0;

    	bool doRun=true;

    	SIGINT_Handler handler(doRun);
    	SignalHandler :: getInstance()->registerHandler ( SIGINT,&handler );

    	while(doRun)
    	{
        	int local_count;

        	std::cout << "Producido:"<<number<<std::endl;

        	usleep(rand()%150000+50000);

			if(counter.read() == bufferSize)
				notFull.dec();

			buffer.write(number,inPtr.read());

			mutex.dec();

			counter.write(counter.read() + 1);
			local_count = counter.read();

			mutex.inc();

			if(local_count == 1)
				notEmpty.inc();

			inPtr.write((inPtr.read()+1)%bufferSize);

			number++;
    	}

        buffer.release();
        counter.release();
        inPtr.release();
        outPtr.release();

    	std::cout <<"Termina productor"<<std::endl;
    }
    //Consumer
    else
    {
    	int local_count =0;
    	int value;

    	std::cout <<"Arranca consumidor"<<std::endl;
    	while(true)
    	{
    		if(local_count == 0)
    			notEmpty.dec();

    		value = buffer.read(outPtr.read());

    		mutex.dec();
    		local_count = counter.read()-1;
    		counter.write(local_count);
    		mutex.inc();

    		if(local_count == bufferSize -1)
    			notFull.inc();

    		outPtr.write((outPtr.read() +1) %bufferSize);

    		std::cout << "Consumido:"<<value<<std::endl;

    		if(value == 99)
    		{
    			std::cout << "end"<<std::endl;
    			 kill(pid,SIGINT);
    			break;
    		}
    	}

        wait();

        buffer.release();
        counter.release();
        inPtr.release();
        outPtr.release();

        mutex.destroy();
        notFull.destroy();
        notEmpty.destroy();

        std::cout <<"Termina consumidor"<<std::endl;
    }



    return 0;
}
