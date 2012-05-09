#include "HIDObservable.h"
#include "Observer.h"

HIDObservable::HIDObservable(){}
HIDObservable::~HIDObservable(){}

void HIDObservable::broadcastLeftClick( POINT p_mousePosition )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastLeftClick( p_mousePosition );
	}
}
void HIDObservable::broadcastRightClick( POINT p_mousePosition )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastRightClick( p_mousePosition );
	}
}

void HIDObservable::broadcastMousePos( POINT p_mousePosition )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastMousePos( p_mousePosition );
	}
}

void HIDObservable::broadcastKeyPress( USHORT p_key )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastKeyPress( p_key );
	}
}

void HIDObservable::addSubscriber( Observer* p_observer )
{
	m_subscribers.push_back( p_observer );
}

void HIDObservable::removeObserver( Observer* p_observer )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		if (p_observer->compair( m_subscribers.at(i) ))
		{
			m_subscribers.erase( m_subscribers.begin() + i );
			break;
		}
	}
	m_subscribers.shrink_to_fit();
}