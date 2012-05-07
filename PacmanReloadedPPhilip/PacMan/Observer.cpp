#include "Observer.h"

Observer::Observer(Scene* p_scene)
{
	m_scene = p_scene;
}

int Observer::getIndex()
{
	return m_index;
}

bool Observer::compair( Observer* p_observer )
{
	return m_index == p_observer->getIndex(); 
}

void Observer::broadcastLeftClick( POINT p_mousePosition )
{
	m_scene->leftMouseClick(p_mousePosition);
}

void Observer::broadcastRightClick( POINT p_mousePosition )
{
	m_scene->rightMouseClick(p_mousePosition);
}

void Observer::broadcastMousePos( POINT p_mousePosition )
{
	m_scene->mouseMove( p_mousePosition );
}

void Observer::broadcastKeyPress( USHORT p_key )
{ 
	m_scene->keyEvent(p_key);
}