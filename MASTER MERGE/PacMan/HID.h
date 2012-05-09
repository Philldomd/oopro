#ifndef HID_H
#define HID_H

#include "stdafx.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "HIDObservable.h"

class HID
{
public:
	HID( HWND p_hwnd );
	~HID();

	void update( UINT message, LPARAM lParam );
	HIDObservable*	getObservable();
private:
	HIDObservable	m_observable;
	Mouse			m_mouse;
	Keyboard		m_keyboard;
};

#endif

// http://www.toymaker.info/Games/html/raw_input.html