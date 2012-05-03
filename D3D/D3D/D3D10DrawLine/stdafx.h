#ifndef _STDAFX__H
#define _STDAFX__H

#include <windows.h>
#include <D3D10.h>
#include <D3DX10.h>
#include <DxErr.h>
#include <string>
#include <vector>

const float         CAMERA_FOVX = 90.0f;
const float         CAMERA_OFFSET = 50.0f;
const float         CAMERA_ZFAR = 1000.0f;
const float         CAMERA_ZNEAR = 0.1f;
const float         CAMERA_ZOOM_MIN = 1.5f;
const float         CAMERA_ZOOM_MAX = 5.0f;
const D3DXVECTOR3   CAMERA_ACCELERATION(20.0f, 20.0f, 20.0f);
const D3DXVECTOR3   CAMERA_VELOCITY(1.0f, 1.0f, 1.0f);

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
#define PI (3.14159265358979323846f)

using namespace std;

#endif
