#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#ifndef MOUSE_H
#define	MOUSE_H

#include "stdafx.h"

class Mouse
{
public:
	Mouse();
	Mouse( HWND p_hwnd );
	~Mouse();
	
	bool			click( RAWINPUT* raw, USHORT  p_flag );
	POINT			getPosition();
	
private:
	int				m_x;
	int				m_y;
	HWND			m_hwnd;
};

#endif