//=============================================================================
// Init Direct3D.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//=============================================================================

#include "d3dApp.h"
#include "Main.h"
 
class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	Main* mMain;
	ID3D10BlendState* blendState;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	

	InitDirect3DApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
: D3DApp(hInstance) 
{
	
	mMain = NULL;
}

InitDirect3DApp::~InitDirect3DApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();
}

void InitDirect3DApp::initApp()
{
	D3DApp::initApp();

	blendState = NULL;
	D3D10_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D10_BLEND_DESC));
	desc.AlphaToCoverageEnable = TRUE;
	desc.BlendEnable[0] = TRUE;
	desc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	desc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	desc.BlendOp = D3D10_BLEND_OP_ADD;
	desc.SrcBlendAlpha = D3D10_BLEND_SRC_ALPHA;
	desc.DestBlendAlpha = D3D10_BLEND_INV_SRC_ALPHA;
	desc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	desc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	md3dDevice->CreateBlendState(&desc, &blendState);

	//Init Main
	mMain = new Main(md3dDevice, mFont,&mhMainWnd);

	HR(mMain->initialize());
}

void InitDirect3DApp::onResize()
{
	if(mMain)
		mMain->onResize();
	D3DApp::onResize();
}

void InitDirect3DApp::updateScene(float dt)
{
	mMain->updateInput(dt);
	mMain->update(dt);

	D3DApp::updateScene(dt);
}

void InitDirect3DApp::drawScene()
{
	D3DApp::drawScene();

	//Render everything
	mMain->render(mFrameStats);

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {20, 5, 0, 0};
	//Add Camera position to mFrameStats
	D3DXVECTOR3 cameraPos = mMain->getCameraPos();
	wstring sCameraPos, information;

	wostringstream out;   
	out.precision(6);
	out << "\nCamera Pos: "	<< cameraPos.x << ", "
		<< cameraPos.y << ", "
		<< cameraPos.z;
	sCameraPos = out.str();

	information.append(mFrameStats);
	information.append(sCameraPos);

	mFont->DrawTextW(NULL, (LPCWSTR)information.c_str(), -1, &R, DT_NOCLIP, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	mSwapChain->Present(0, 0);
}

LRESULT InitDirect3DApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Update the controller message loop!
	if(mMain)
		mMain->handleInput( msg, wParam, lParam, mTimer.getDeltaTime() );

	return D3DApp::msgProc(msg, wParam, lParam);
}