#ifndef UTIL_H
#define	UTIL_H

#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <list>

class Util {
public:
    
    static Util & getInstance(); 
    static long generateRandomNumber(long minValue, long maxValue);
    static void createProcess(std::string processName, 
    int amountOfProcesses = 1, int parameterOffset = 0);
    static void createProcess(std::string processName,
                             std::string arg1, std::string arg2);
    virtual ~Util();
    
private:
    Util() {};
    Util(const Util& orig);
    void operator = (Util const &);
    
private:
    static char buffer[255];
};

#endif	/* UTIL_H */

