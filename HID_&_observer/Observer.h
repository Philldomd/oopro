#ifndef OBSERVER_H
#define OBSERVER_H

#include "stdafx.h"

class Observer
{
public:
	virtual void	broadcastLeftClick( POINT p_mousePosition );
	virtual void	broadcastRightClick( POINT p_mousePosition );
	virtual void	broadcastKeyPress( USHORT p_key );

	int				getIndex();
	bool			compair( Observer* p_observer );
private:
	int				m_index;
};

#endif