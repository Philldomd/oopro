#ifndef MENUOBSERVER_H
#define MENUOBSERVER_H

#include "stdafx.h"

class Observer;

class MenuObserver : public Observer
{
public:
	MenuObserver();
	~MenuObserver();

	void	broadcastLeftClick( POINT p_mousePosition );
	void	broadcastRightClick( POINT p_mousePosition );
	void	broadcastKeyPress( USHORT p_key );
};

#endif