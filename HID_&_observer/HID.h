#ifndef HID_H
#define HID_H

#include "stdafx.h"

class HIDObservable;
class Mouse;
class Keyboard;

class HID
{
public:
	HID( HWND p_hwnd );
	~HID();

	void update( UINT message, LPARAM lParam );
private:
	HIDObservable	m_observable;
	Mouse			m_mouse;
	Keyboard		m_keyboard;
};

#endif

// http://www.toymaker.info/Games/html/raw_input.html