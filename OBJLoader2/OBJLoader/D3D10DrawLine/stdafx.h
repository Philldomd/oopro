#ifndef _STDAFX__H
#define _STDAFX__H

#include <windows.h>
#include <D3D10.h>
#include <D3DX10.h>
#include <DxErr.h>

#include <string>
#include <vector>

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define PI (3.14159265358979323846f)

#endif
