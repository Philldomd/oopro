#include "Observer.h"
#include "MenuObserver.h"

MenuObserver::MenuObserver(){}
MenuObserver::~MenuObserver(){}

void MenuObserver::broadcastLeftClick( POINT p_mousePosition )
{ }

void MenuObserver::broadcastRightClick( POINT p_mousePosition )
{ }

void MenuObserver::broadcastKeyPress( USHORT p_key )
{ }