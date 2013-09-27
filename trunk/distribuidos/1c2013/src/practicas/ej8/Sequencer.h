/*
 * Sequencer.h
 *
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "Object.h"
#include "Semaphore.h"

class Sequencer: public Object {
public:
	Sequencer(std::string name, int key,int procNum);
	virtual ~Sequencer();

	void init();

	void destroy();

	bool begin(int pos);
	bool end(int pos);

	Semaphore m_Sem;
};

#endif /* SEQUENCER_H_ */
