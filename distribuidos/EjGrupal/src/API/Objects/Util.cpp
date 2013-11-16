#include "Util.h"
#include <stdlib.h>
#include <time.h>

Util & Util::getInstance() {
	/* Variable instantiated in the first use, destroyed 
	 *  automatically in the end of the program
	 */
    
    static Util m_Instance;
	return m_Instance;
}

long Util::generateRandomNumber(long minValue, long maxValue) {
    srand( time(NULL) + getpid() );
     return minValue + rand() % ( (abs(maxValue - minValue) + 1) );
}

Util::Util(const Util& orig) {
}

Util::~Util() {
}

