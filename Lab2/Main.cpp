#include "stdafx.h"
#include "OBJLoader.h"
#include "Mesh.h"
#include "GameTimer.h"
#include "Ground.h"
#include "DrawTex2D.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE				g_hInst					= NULL;  
HWND					g_hWnd					= NULL;
D3D10_DRIVER_TYPE       g_driverType			= D3D10_DRIVER_TYPE_NULL;
ID3D10Device*           g_d3dDevice			= NULL;
IDXGISwapChain*         g_pSwapChain			= NULL;
ID3D10RenderTargetView* g_pRenderTargetView		= NULL;
ID3D10Texture2D*        g_pDepthStencil			= NULL;
ID3D10DepthStencilView* g_pDepthStencilView		= NULL;
D3D10_VIEWPORT vp;


//camera data
D3DXVECTOR3				g_vCameraPos			= D3DXVECTOR3(0,0,-100);
D3DXMATRIX				g_mView;
D3DXMATRIX				g_mProj;
D3DXMATRIX				g_mRotY;
D3DXMATRIX				g_mRotX;
D3DXMATRIX				g_mRotZ;
D3DXMATRIX mLightView;
D3DXMATRIX mLightVolume;

float					g_Angle					= 0;

//Mesh
Mesh* g_obj = NULL;

//OBJLoader
OBJLoader* objLoader = NULL;

//Ground
Ground* g_Ground = NULL;

//ShadowMap Texture
DrawTex2D* g_shadow = NULL;

//Shadowmap Shader
Shader* g_ShadowShader = NULL;

//Timer
GameTimer* g_timer = NULL;
std::wstring mFrameStats;
ID3DX10Font* mFont = NULL;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT             InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT				InitDevice();
HRESULT				Render();
void				Update(float deltaTime);
void				CleanupDevice();

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;;

	RECT rc;
	GetClientRect( g_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	D3D10_DRIVER_TYPE driverTypes[] = 
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D10CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, 
			D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_d3dDevice );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED(hr) )
		return hr;

	// Create a render target view
	ID3D10Texture2D* pBackBuffer;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		return hr;

	hr = g_d3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
	pBackBuffer->Release();
	if( FAILED(hr) )
		return hr;

	// Create depth stencil texture
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_d3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
	if( FAILED(hr) )
		return hr;

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_d3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if( FAILED(hr) )
		return hr;


	g_d3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

	// Setup the viewport
	
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_d3dDevice->RSSetViewports( 1, &vp );

	//Create Timer
	g_timer = new GameTimer();

	//TEST
	objLoader = new OBJLoader();

	//Create OBJ
	g_obj = new Mesh();

	g_timer->start();
	g_timer->reset();

	//Load obj
	objLoader->LoadOBJ("bth.obj", g_obj);

	g_timer->stop();

	float time = g_timer->getGameTime();

	//init OBJ
	if(FAILED(g_obj->Initialize(g_d3dDevice)))
	{
		return E_FAIL;
	}
	D3DXMatrixIdentity(&g_mRotX);
	D3DXMatrixIdentity(&g_mRotY);
	D3DXMatrixIdentity(&g_mRotZ);

	//Create Ground
	g_Ground = new Ground();

	if(FAILED(g_Ground->Initialize(g_d3dDevice)))
	{
		return E_FAIL;
	}

	//Create Shadowmap

	g_shadow = new DrawTex2D();
	if(FAILED(g_shadow->Initialize(g_d3dDevice)))
	{
		return E_FAIL;
	}

	// Define our vertex data layout for shadowmap
	const D3D10_INPUT_ELEMENT_DESC ShadowLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Create Shader
	g_ShadowShader = new Shader();
	if(FAILED(g_ShadowShader->Init(g_d3dDevice, "BuildShadowMap.fx", ShadowLayout, sizeof(ShadowLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "DrawLine",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	D3DXMatrixLookAtLH(&mLightView, &D3DXVECTOR3(0,100,0),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(-1, 0, 0));

	D3DXMatrixOrthoLH(&mLightVolume, 200, 200, 1.0f, 151.0f);

	//Create Tex font
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height = 24;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy((wchar_t *)fontDesc.FaceName, L"Times New Roman");
	D3DX10CreateFontIndirect(g_d3dDevice, &fontDesc, &mFont);

	return S_OK;
}

void Update(float deltaTime)
{
	D3DXMatrixRotationX(&g_mRotX, g_Angle);
	D3DXMatrixRotationY(&g_mRotY, g_Angle);
	D3DXMatrixRotationZ(&g_mRotZ, g_Angle);

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (g_timer->getGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << L"FPS: " << fps << L"\n" 
			 << "Milliseconds: Per Frame: " << mspf << "\nPress R to change resolution";
		mFrameStats = outs.str();
		
		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
}

HRESULT Render()
{
	// Render the result
	D3DXMATRIX mWorldViewProj;

	D3DXMATRIX mRotation;

	//RECT rc;
	float width = 1024;
	float height = 768;

	D3DXMatrixLookAtLH(&g_mView, &g_vCameraPos, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
	D3DXMatrixPerspectiveLH(&g_mProj, (float)D3DX_PI * 0.6f, (float)(width / height), 1.0f, 1000.0f);

	mRotation = g_mRotX * g_mRotY * g_mRotZ;
	mWorldViewProj =  g_mView * g_mProj;

	static float ClearColor[4] = { 1.0f,1.0f, 1.0f, 0.0f };
	

	//Create shadow map
	//***************************************************************************
			g_d3dDevice->OMSetRenderTargets(0,0, g_shadow->getRenderTD());
			g_d3dDevice->RSSetViewports(1, g_shadow->getViewport());
			g_d3dDevice->ClearDepthStencilView(g_shadow->getRenderTD(), D3D10_CLEAR_DEPTH, 1.0f, 0);
			
			//render Ground 
			D3DXMATRIX groundWVP = mLightView * mLightVolume;
			g_Ground->RenderSpecificShader(g_ShadowShader, groundWVP);

			//render Object
			D3DXMATRIX objWVP = mRotation * mLightView * mLightVolume;
			g_obj->RenderSpecificShader(g_ShadowShader , objWVP);

	//Create Final scene
	//***************************************************************************
			//set render targets and viewports
			g_d3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );
			g_d3dDevice->RSSetViewports( 1, &vp );
			g_d3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor);
			g_d3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );

			mWorldViewProj =  g_mView * g_mProj;

			//render Ground
			g_Ground->SetShadowMap(g_shadow->getDepthRs());
			g_Ground->PrepareToRender(mWorldViewProj, groundWVP, 
									D3DXVECTOR2((float)g_shadow->getTexSizeX(),(float)g_shadow->getTexSizeY()));
			g_Ground->Render();

			mWorldViewProj = mRotation * g_mView * g_mProj;

			//render Object
			g_obj->PrepareToRender(mWorldViewProj, mRotation);
			g_obj->Render();

			const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
			RECT R = {300, 5, 0, 0};
			mFont->DrawTextW(0, (LPCWSTR)mFrameStats.data(), -1, &R, DT_NOCLIP, BLACK);
		
	//render shadow map billboard
	//***************************************************************************
			g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
			g_ShadowShader->SetResource("shadowMap",g_shadow->getDepthRs());
			g_ShadowShader->SetTechniqueByName("RenderBillboard");

			g_ShadowShader->Apply(0);
			//g_d3dDevice->DrawIndexed(1, 0, 0);
			g_d3dDevice->Draw(1,0);

			//release shadow map
			g_ShadowShader->SetResource("shadowMap", 0);
			g_ShadowShader->GetTechniqueByName("RenderBillboard")->GetPassByIndex(0)->Apply(0);
			//Release Ground tex
			g_Ground->SetGroundTexNULL();

	if(FAILED(g_pSwapChain->Present( 0, 0 )))
		return E_FAIL;

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return 0;

	if( FAILED( InitDevice() ) )
		return 0;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Main message loop
	MSG msg = {0};
	g_timer->reset();

	while(WM_QUIT != msg.message)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
			g_timer->tick();
			//render
			Update(dt);
			Render();

			prevTimeStamp = currTimeStamp;
		}
	}

	CleanupDevice();

	return (int) msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "D3DLines";
	wcex.hIconSm        = 0;
	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	// Create window
	g_hInst = hInstance; 
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	
	if(!(g_hWnd = CreateWindow(	"D3DLines",
							"Default",
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							rc.right - rc.left,
							rc.bottom - rc.top,
							NULL,
							NULL,
							hInstance,
							NULL)))
	{
		return E_FAIL;
	}

	ShowWindow( g_hWnd, nCmdShow );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			g_timer->stop();
		}
		else
		{
			g_timer->start();
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:

		switch(wParam)
		{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_RIGHT:
				g_Angle += PI * 0.05;
				if(g_Angle > 2 * PI)
				{
					g_Angle -= 2* PI;
				}
				break;
			case VK_LEFT:
				g_Angle -= PI * 0.05;
				if(g_Angle < 0)
				{
					g_Angle += 2* PI;
				}
				break;
			case 'R':
				g_shadow->toggleSMAPSize();
				break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	if( g_d3dDevice ) g_d3dDevice->ClearState();

	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_d3dDevice);
	g_ShadowShader->~Shader();
	g_shadow->~DrawTex2D();
	g_obj->~Mesh();
	objLoader->~OBJLoader();
}