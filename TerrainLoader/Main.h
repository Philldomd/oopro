#ifndef MAIN__H
#define MAIN__H

#include "d3dUtil.h"
#include "Camera.h"
#include "Terrain.h"

using namespace std;

//Definitions used by Raw input
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

class Main
{
public:
	Main(ID3D10Device* pDevice, ID3DX10Font* pFont, HWND* phWnd);
	~Main();

	HRESULT initialize();
	void update(float pDt);
	void updateInput(float pDt);
	void render(wstring pFrameStats);
	void onResize();
	D3DXVECTOR3 getCameraPos();

	void handleInput(	UINT message, WPARAM wParam, 
						LPARAM lParam, float dt );

private:
	void updateClientBounds();


	UINT mClientW, mClientH;
	Camera* mCamera;
	Terrain* mTerrain;
	HWND* mHWnd;
	ID3D10Device* mDevice;
};
#endif