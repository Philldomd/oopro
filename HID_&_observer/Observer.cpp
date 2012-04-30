#include "Observer.h"

int Observer::getIndex()
{
	return m_index;
}

bool Observer::compair( Observer* p_observer )
{
	return m_index == p_observer->getIndex(); 
}