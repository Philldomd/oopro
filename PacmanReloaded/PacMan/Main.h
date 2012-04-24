
#ifndef MAIN_H
#define MAIN_H


#include "d3dApp.h"
#include <vector>
#include "SceneManager.h"

using namespace std;

class Main : public D3DApp
{
public:
	Main(HINSTANCE hInstance);
	~Main();
	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);


private:
	 
	SceneManager * g_sceneManager;

};

#endif 