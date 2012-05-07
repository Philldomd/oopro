#include "HID.h"


HID::HID( HWND p_hwnd )
{
	m_keyboard		= Keyboard( p_hwnd );
	m_mouse			= Mouse( p_hwnd );
}

HID::~HID()
{
	m_mouse.~Mouse();
	m_mouse = NULL;
	m_keyboard.~Keyboard();
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
				if ( m_mouse.click(raw, RI_MOUSE_LEFT_BUTTON_DOWN) )
				{
					m_observable.broadcastLeftClick(m_mouse.getPosition());
				}

				if( m_mouse.click(raw, RI_MOUSE_RIGHT_BUTTON_DOWN) )
				{
					m_observable.broadcastRightClick(m_mouse.getPosition());
				}

				m_observable.broadcastMousePos(m_mouse.getPosition());
			}

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				if(raw->data.keyboard.Flags == 0)
					m_observable.broadcastKeyPress( m_keyboard.keyDown(raw) );
			}

			//delete[] lpb;
			break;
		default:
			
			break;
	}
}

HIDObservable* HID::getObservable()
{
	return &m_observable;
}