#include "Main.h"
#include "Level.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	Main theApp(hInstance);

	theApp.initApp();

	return theApp.run();
}


Main::Main(HINSTANCE hInstance)
: D3DApp(hInstance){}


Main::~Main(void)
{
	if(md3dDevice)
		md3dDevice->ClearState();
	g_sceneManager->removeCurrentScenes();
	g_sceneManager->~SceneManager();
	g_sceneManager = NULL;
	g_hid->~HID();
	g_hid = NULL;
}


void Main::initApp()
{
	g_sceneManager = NULL;

	D3DApp::initApp();

	g_hid = new HID(mhMainWnd);

	g_sceneManager = SceneManager::getInstance();
	g_sceneManager->initSceneManager(md3dDevice);

	onResize();

	Scene* tempScene = new Level();
	g_sceneManager->addScene(tempScene);
	g_hid->getObservable()->addSubscriber(new Observer(tempScene));
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

	g_sceneManager->drawScene(D3DApp::getDSV(), D3DApp::getRTV(), D3DApp::getVP());

	//Draw information about things(ONLY FOR DEBUGGING)
	RECT R = {5, 5, 0, 0};
	mFont->DrawTextW(0, (LPCWSTR)mFrameStats.c_str() , -1, &R, DT_NOCLIP, WHITE);

	mSwapChain->Present(0,0);
}

LRESULT Main::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_hid->update(msg, lParam);
	return D3DApp::msgProc(msg, wParam, lParam);
}