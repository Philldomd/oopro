#ifndef OBSERVER_H
#define OBSERVER_H

#include "stdafx.h"
#include "Scene.h"

class Observer
{
public:

	Observer(Scene* p_scene);
	
	void		broadcastLeftClick( POINT p_mousePosition );
	void		broadcastRightClick( POINT p_mousePosition );
	void		broadcastMousePos( POINT p_mousePosition );
	void		broadcastKeyPress( USHORT p_key );

	int			getIndex();
	bool		compair( Observer* p_observer );
private:
	int			m_index;
protected:
	Scene*		m_scene;
};

#endif