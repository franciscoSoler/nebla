/*
 * SorteadorEntero.cpp
 *
 *  Created on: 30/09/2012
 *      Author: fernando
 */

#include "SorteadorEntero.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

SorteadorEntero::SorteadorEntero() {
	// TODO Auto-generated constructor stub

}

SorteadorEntero::~SorteadorEntero() {
	// TODO Auto-generated destructor stub
}

int SorteadorEntero::sortear(int desde,int hasta){

	  srand ( time(NULL) * getpid());


	  return  rand() % (hasta-desde+1) + desde;
}
