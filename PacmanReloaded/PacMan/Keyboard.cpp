#include "Keyboard.h"

Keyboard::Keyboard(){}

Keyboard::Keyboard( HWND p_hwnd )
{
	RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_KEYBOARD; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;   
    Rid[0].hwndTarget = p_hwnd;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

Keyboard::~Keyboard(){}

USHORT Keyboard::keyDown( RAWINPUT* raw )
{
	USHORT keyCode = raw->data.keyboard.VKey;

	return keyCode;
}