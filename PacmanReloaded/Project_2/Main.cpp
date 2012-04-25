#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	

	Main theApp(hInstance);

	theApp.initApp();

	return theApp.run();
}


Main::Main(HINSTANCE hInstance)
: D3DApp(hInstance)
{
}


Main::~Main(void)
{
	if(md3dDevice)
		md3dDevice->ClearState();
}


void Main::initApp()
{
	D3DApp::initApp();
}
void Main::onResize()
{
	D3DApp::onResize();
}
void Main::updateScene(float p_deltaTime)
{
	D3DApp::updateScene(p_deltaTime);
}
void Main::drawScene()
{
	D3DApp::drawScene();
}

LRESULT Main::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return D3DApp::msgProc(msg, wParam, lParam);
}