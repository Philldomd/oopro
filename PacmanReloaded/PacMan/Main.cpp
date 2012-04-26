#include "Main.h"
#include "MainMenuScene.h"

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
	g_sceneManager = NULL;

	D3DApp::initApp();

	g_sceneManager = SceneManager::getInstance();
	g_sceneManager->initSceneManager(md3dDevice);

	onResize();

	g_sceneManager->addScene(new MainMenuScene(md3dDevice));

}
void Main::onResize()
{
	D3DApp::onResize();
	g_sceneManager->recalculateSpriteBatch();
}
void Main::updateScene(float p_deltaTime)
{
	D3DApp::updateScene(p_deltaTime);

	g_sceneManager->updateScene(p_deltaTime);
}
void Main::drawScene()
{
	D3DApp::drawScene();

	g_sceneManager->drawScene();

	//Draw information about things(ONLY FOR DEBUGGING)
	RECT R = {5, 5, 0, 0};
	mFont->DrawTextW(0, (LPCWSTR)mFrameStats.c_str() , -1, &R, DT_NOCLIP, WHITE);

	mSwapChain->Present(0,0);
}

LRESULT Main::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return D3DApp::msgProc(msg, wParam, lParam);
}