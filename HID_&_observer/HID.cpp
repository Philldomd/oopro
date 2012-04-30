#include "HID.h"
#include "Keyboard.h"
#include "Mouse.h"

HID::HID( HWND p_hwnd )
{
	m_keyboard		= new Keyboard( p_hwnd );
	m_mouse			= new Mouse( p_hwnd );
}

HID::~HID()
{
	m_mouse->~Mouse();
	m_mouse = NULL;
	m_keyboard->~Keyboard();
	m_keyboard = NULL;
}

void HID::update( UINT message, LPARAM lParam )
{
	UINT dwSize;
	RAWINPUT* raw;

	switch (message) 
	{
		case WM_INPUT:
			dwSize = 40;
			static BYTE lpb[40];
    
			GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, 
							lpb, &dwSize, sizeof(RAWINPUTHEADER) );
    
			raw = (RAWINPUT*)lpb;
    
			if (raw->header.dwType == RIM_TYPEMOUSE) 
			{
				m_mouse->updateMousePos(lParam);

				if ( m_mouse->click(raw, RI_MOUSE_LEFT_BUTTON_DOWN) )
				{
					// Send left click event to observers
				}

				if( m_mouse->click(raw, RI_MOUSE_RIGHT_BUTTON_DOWN) )
				{
					// Send right click event to observers.
				}
			}

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				// If någon typ av input. broadcast.
			}

			delete[] lpb;
			break;
		default:
			
			break;
	}
}