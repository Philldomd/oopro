#include "Main.h"

Main::Main(ID3D10Device* pDevice,ID3DX10Font* pFont, HWND* phWnd)
{
	mHWnd = phWnd;
	mDevice = pDevice;
	mCamera = NULL;
	mTerrain = NULL;

	updateClientBounds();
}
Main::~Main()
{
	mCamera->~Camera();
	mCamera = NULL;
	mTerrain->~Terrain();
	mTerrain = NULL;

	fx::DestroyAll();
}
HRESULT Main::initialize()
{
	//Init Effect Files
	fx::InitAll(mDevice);

	//Init raw input
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
	Rid[0].dwFlags = RIDEV_INPUTSINK;   
	Rid[0].hwndTarget = *mHWnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	//Create and init Camera
	mCamera = new Camera(D3DXVECTOR3( 0, 200, 0 ));
	float aspect = float(mClientW)/mClientH;
	mCamera->createProjectionMatrix(0.25f*PI, aspect, 0.5f, 1000.0f);

	//Create and init Terrain
	mTerrain = new Terrain(mDevice, 512, 512);
	HR(mTerrain->init());
	
	return S_OK;
}

void Main::onResize()
{
	updateClientBounds();
	float aspect = float(mClientW)/mClientH;
	mCamera->createProjectionMatrix(0.25f*PI, aspect, 0.5f, 1000.0f);
}

void Main::updateInput(float pDt)
{
	if(GetAsyncKeyState('A') & 0x8000)
	{
		mCamera->strafe( -pDt );
	}
	if(GetAsyncKeyState('D') & 0x8000)
	{
		mCamera->strafe( pDt );
	}
	if(GetAsyncKeyState('W') & 0x8000)
	{
		mCamera->walk( pDt );
	}
	if(GetAsyncKeyState('S') & 0x8000)
	{
		mCamera->walk( -pDt );
	}

	if(GetAsyncKeyState('R') & 0x8000)
	{
		mCamera->setY( pDt * 100 );
	}
	else if(GetAsyncKeyState('F') & 0x8000)
	{
		mCamera->setY( -pDt * 100 );
	}
	else{}

	if(GetAsyncKeyState('T') & 0x8000)
	{
		mCamera->setCameraFree(true);
	}
	else if(GetAsyncKeyState('Y') & 0x8000)
	{
		mCamera->setCameraFree(false);
	}
	else{}

	mCamera->updateCameraPos();
	mCamera->updateViewMatrix();
}
void Main::update(float pDt)
{
	
}
void Main::render(wstring pFrameStats)
{
	D3DXMATRIX view, proj;
	D3DXVECTOR3 cameraPos;
	cameraPos = mCamera->getPosition();
	view = mCamera->getViewMatrix();
	proj = mCamera->getProjectionMatrix();

	mTerrain->prepToRender(view * proj, cameraPos);
	mTerrain->render(view , proj);
}

void Main::handleInput( UINT message, WPARAM wParam, LPARAM lParam, float dt )
{
	UINT dwSize;
	RAWINPUT* raw;
	float radianScaleWidth, radianScaleHeight;
	POINT pt;

	switch (message) 
	{
	case WM_INPUT:
		dwSize = 40;
		static BYTE lpb[40];

		GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, 
			lpb, &dwSize, sizeof(RAWINPUTHEADER) );

		raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE && !mCamera->isCameraFree()) 
		{
			radianScaleWidth = (2*PI) / mClientW;
			radianScaleHeight = (PI/3) / mClientH;

			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;

			//Rotate Camera
			mCamera->updatePitch( yPosRelative * radianScaleHeight);
			mCamera->updateYaw( xPosRelative * radianScaleWidth);

			//Center mouse
			pt.x = mClientW/2;
			pt.y = mClientH/2;

			ClientToScreen(*mHWnd, &pt);
			SetCursorPos(pt.x, pt.y);
		}
		break;
	default:
		break;
	}
}

void Main::updateClientBounds()
{
	RECT rc;
	GetClientRect( *mHWnd, &rc );
	mClientW = rc.right - rc.left;
	mClientH = rc.bottom - rc.top;
}

D3DXVECTOR3 Main::getCameraPos()
{
	if(mCamera)
		return mCamera->getPosition();
	else
		return D3DXVECTOR3(0,0,0);
}