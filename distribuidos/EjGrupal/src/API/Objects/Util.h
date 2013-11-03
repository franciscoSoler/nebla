#ifndef UTIL_H
#define	UTIL_H

#include <unistd.h>
#include <sys/types.h>

class Util {
public:
    
    static Util & getInstance(); 
    static long generateRandomNumber(long minValue, long maxValue);
    virtual ~Util();
    
private:
    Util() {};
    Util(const Util& orig);
    void operator = (Util const &);

};

#endif	/* UTIL_H */

