/*
 * Sequencer.cpp
 *
 */
#include "Sequencer.h"

Sequencer::Sequencer(std::string name, int key,int procNum):
m_Sem(name,key,0,procNum)
{

}

Sequencer::~Sequencer()
{
}

void Sequencer::init()
{
	m_Sem.init();
	m_Sem.inc(0);
}

void Sequencer::destroy()
{
	m_Sem.destroy();
}

bool Sequencer::begin(int pos)
{
//	std::cout << "Dec: "<< pos <<std::endl;
	return m_Sem.dec(pos);
}

bool Sequencer::end(int pos)
{
//	std::cout << "Inc: "<< (pos+1)%m_Sem.semAmmount() <<std::endl;
	return m_Sem.inc((pos+1)%m_Sem.semAmmount());
}
