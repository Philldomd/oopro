#include "Mouse.h"

Mouse::Mouse(){}

Mouse::Mouse( HWND p_hwnd )
{
	RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;   
    Rid[0].hwndTarget = p_hwnd;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
	m_hwnd = p_hwnd;
	ShowCursor(true);
}

Mouse::~Mouse(){}

bool Mouse::click(RAWINPUT* raw, USHORT p_flag)
{
	return p_flag == raw->data.mouse.usButtonFlags;
}

POINT Mouse::getPosition()
{
	// The OS does not move the cursor according to the X and Y from the RAW bits.
	// GetCursorPos() polls Windows for the cursor position.
	POINT tempPos;
	GetCursorPos(&tempPos);
	ScreenToClient(m_hwnd, &tempPos);

	return tempPos;
}