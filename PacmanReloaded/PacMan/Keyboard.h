#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD     ((USHORT) 0x06)
#endif

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdafx.h"

class Keyboard
{
public:
	Keyboard();
	Keyboard(HWND p_hwnd);
	~Keyboard();

	USHORT keyDown( RAWINPUT* raw );
};

#endif

// http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx