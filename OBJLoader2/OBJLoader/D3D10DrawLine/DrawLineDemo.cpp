//--------------------------------------------------------------------------------------
// File: DrawLineDemo.cpp
//
// Example: Draw line using D3D10
//
// Copyright (c) Stefan Petersson 2008. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Buffer.h"
#include "OBJLoader.h"
#include "Shader.h"
#include "CreateTex2D.h"
#include "GameTimer.h"
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE				g_hInst					= NULL;  
HWND					g_hWnd					= NULL;
D3D10_DRIVER_TYPE       g_driverType			= D3D10_DRIVER_TYPE_NULL;
ID3D10Device*           g_pd3dDevice			= NULL;
IDXGISwapChain*         g_pSwapChain			= NULL;
ID3D10RenderTargetView* g_pRenderTargetView		= NULL;
ID3D10Texture2D*        g_pDepthStencil			= NULL;
ID3D10DepthStencilView* g_pDepthStencilView		= NULL;

//camera data
D3DXVECTOR3				g_vCameraPos			= D3DXVECTOR3(0,100,-200);
D3DXMATRIX				g_mView;
D3DXMATRIX				g_mProj;
D3DXMATRIX				lightView, lightProj;

//buffer data
ID3D10InputLayout*		g_pVertexLayout;
Buffer*					g_pVB;
Buffer*					g_pVBMap;

//Shaders
Shader*					g_pShader;
Shader*					g_pShaderMap;
Shader*					g_pShadowShader;

//Shadows
CreateTex*				mShadowTex;


float					rot;
D3DXMATRIX				rotationMy;
D3DXMATRIX				rotationMx;
//shader variables
ID3D10Effect*			g_pEffect;
ID3D10Effect*			g_pEffectMap;
ID3D10EffectTechnique*	g_pTechRenderLine;
D3D10_VIEWPORT vp;

GameTimer mTimer;
std::wstring mFrameStats;
ID3DX10Font* mFont = NULL;
RECT R;

Loader*					g_pLoader;
int						numVertex;
Material				mtl;
struct LineVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texCoord;
	D3DXVECTOR3 norm;
};
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
	
	//INIT Timer
	mTimer = GameTimer();
	mTimer.reset();
	mTimer.start();

	rot = 10;
	g_pLoader = new Loader();
	g_pShader = new Shader();
	g_pShadowShader = new Shader();
	g_pShaderMap = new Shader();
	mShadowTex = new CreateTex();
	HRESULT hr = S_OK;;
	g_pVB = new Buffer();
	g_pVBMap = new Buffer();
	RECT rc;
	GetClientRect( g_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
	g_pLoader->loadFile("bth.obj");
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
			D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice );
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

	hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
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
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
	if( FAILED(hr) )
		return hr;

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if( FAILED(hr) )
		return hr;


	g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

	// Setup the viewport
	
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pd3dDevice->RSSetViewports( 1, &vp );
	numVertex = g_pLoader->GetVertBuffLength();
	LineVertex* pData = new LineVertex[numVertex];
	
	for (int i = 0; i < g_pLoader->GetVertBuffLength(); i++)
	{
		pData[i].pos = g_pLoader->GetVertexPos(i);
		pData[i].norm = g_pLoader->GetVertexNorm(i);
		pData[i].texCoord = g_pLoader->GetVertexTexCoord(i);
	}
	//Create vertex buffer to hold maxAmount of particles
	BUFFER_INIT_DESC bd;
	bd.Usage = BUFFER_DEFAULT;
	bd.ElementSize = sizeof( Vertex );
	bd.Type = VERTEX_BUFFER;
	bd.NumElements = numVertex;
	bd.InitData = pData;

	if( FAILED( g_pVB->Init(g_pd3dDevice, bd) ) )
		MessageBox( 0, "Unable to create Vertex Buffer", "VB Error", 0 );

	//Init shader effect
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
	ID3D10Blob* comperror = 0;
	
	// Define our vertex data layout
	const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D10_INPUT_PER_VERTEX_DATA, 0}		
	};


	if(FAILED(g_pShader->Init(g_pd3dDevice, 
		"Line.fx", 
		lineVertexLayout, 
		sizeof(lineVertexLayout)/ sizeof(D3D10_INPUT_ELEMENT_DESC), 
		"DrawLine", 
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	mtl = g_pLoader->GetMaterialAt(0);

	ID3D10ShaderResourceView *pSRView = NULL;
	D3DX10CreateShaderResourceViewFromFile(g_pd3dDevice,mtl.map_Kd,NULL,NULL,&pSRView,NULL);
	g_pShader->SetResource("DDSPIC",pSRView);
	

	
	LineVertex* pDataMap = new LineVertex[4];

	pDataMap[0].pos = D3DXVECTOR3(-200,-100,-200);
	pDataMap[0].texCoord = D3DXVECTOR2(0,0);
	pDataMap[0].norm = D3DXVECTOR3(0,1,0);
	pDataMap[1].pos = D3DXVECTOR3(-200,-100,200);
	pDataMap[1].texCoord = D3DXVECTOR2(1,0);
	pDataMap[1].norm = D3DXVECTOR3(0,1,0);
	pDataMap[2].pos = D3DXVECTOR3(200,-100,-200);
	pDataMap[2].texCoord = D3DXVECTOR2(0,1);
	pDataMap[2].norm = D3DXVECTOR3(0,1,0);
	pDataMap[3].pos = D3DXVECTOR3(200,-100,200);
	pDataMap[3].texCoord = D3DXVECTOR2(1,1);
	pDataMap[3].norm = D3DXVECTOR3(0,1,0);

	BUFFER_INIT_DESC bdMap;
	bdMap.Usage = BUFFER_DEFAULT;
	bdMap.ElementSize = sizeof( Vertex );
	bdMap.Type = VERTEX_BUFFER;
	bdMap.NumElements = 4;
	bdMap.InitData = pDataMap;

	if( FAILED( g_pVBMap->Init(g_pd3dDevice, bdMap) ) )
		MessageBox( 0, "Unable to create Vertex Buffer", "VB Error", 0 );

	if(FAILED(g_pShaderMap->Init(g_pd3dDevice, 
		"Map.fx", 
		lineVertexLayout, 
		sizeof(lineVertexLayout)/ sizeof(D3D10_INPUT_ELEMENT_DESC), 
		"DrawLine", 
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	ID3D10ShaderResourceView *pSRViewMap = NULL;
	D3DX10CreateShaderResourceViewFromFile(g_pd3dDevice,"grass.png",NULL,NULL,&pSRViewMap,NULL);
	g_pShaderMap->SetResource("GRASS", pSRViewMap);

	mShadowTex->Initilize(g_pd3dDevice, 0.5f,0.5f);
	mShadowTex->CreateShadowTex();


	const D3D10_INPUT_ELEMENT_DESC shadowInput[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	if( FAILED( g_pShadowShader->Init(g_pd3dDevice, 
		"ShadowMap.fx", 
		shadowInput, 
		sizeof(shadowInput) / sizeof(D3D10_INPUT_ELEMENT_DESC), 
		"RenderShadowMap",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	D3DXMatrixLookAtLH(&lightView, &D3DXVECTOR3(0,200,0), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(-1,0,0));
	D3DXMatrixOrthoLH(&lightProj, 200, 200, 1, 300);


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
	D3DX10CreateFontIndirect(g_pd3dDevice, &fontDesc, &mFont);

	//Text output position
	R.left = 300;
	R.top = 5;
	R.right = 0;
	R.bottom = 0;

	return S_OK;
}

void Update(float deltaTime)
{
	rot += deltaTime;
	D3DXMatrixRotationY(&rotationMy, rot);
	D3DXMatrixRotationX(&rotationMx, rot);

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.getGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << L"FPS: " << fps << L"\n" 
			 << "Milliseconds: Per Frame: " << mspf;
		mFrameStats = outs.str();
		
		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
}

HRESULT Render()
{
	
	D3DXMATRIX mWorldViewProj,mWorldViewProjNoRot, rotation, lightWVP;
	D3DXMATRIX mWorld;

	D3DXMatrixIdentity(&mWorld);
	//RECT rc;

	D3DXMatrixLookAtLH(&g_mView, &g_vCameraPos, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
	D3DXMatrixPerspectiveLH(&g_mProj, (float)D3DX_PI * 0.6f, (float)(1024 / 768), 1.0f, 1000.0f);
	rotation = rotationMx * rotationMy;
	mWorld = rotation * mWorld;

	lightWVP =  lightView * lightProj;

	D3DXMATRIX lightWVPr = rotation * lightView * lightProj;

	mWorldViewProjNoRot = g_mView * g_mProj;
	mWorldViewProj = rotation * g_mView * g_mProj;

	//Create shadowMap
	//******************************************************************
	g_pd3dDevice->OMSetRenderTargets(0,0,mShadowTex->getSDepthV());
	g_pd3dDevice->RSSetViewports(1, mShadowTex->getViewPort());
	g_pd3dDevice->ClearDepthStencilView(mShadowTex->getSDepthV(), D3D10_CLEAR_DEPTH, 1.0f, 0);

	//Render 
	g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g_pShadowShader->Apply(0);
	D3D10_TECHNIQUE_DESC techDesc;
	g_pShadowShader->SetMatrix("gLightWVP", lightWVPr);
	g_pShadowShader->GetTechnique()->GetDesc( &techDesc );

	g_pVB->Apply(0);
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		g_pShadowShader->Apply(0);
		g_pd3dDevice->Draw(numVertex, 0);
	}
	g_pd3dDevice->OMSetRenderTargets(0,0,NULL);

	//Render Final Scene
	//******************************************************************
	static float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pd3dDevice->RSSetViewports(1, &vp);
	g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pd3dDevice->ClearDepthStencilView(g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);


	// Set variables//Bind Shadow Map Texture and Set Shadow Mab Bias
	g_pShader->SetMatrix("g_mWorldViewProjection", mWorldViewProj);
	g_pShader->SetMatrix("mNormalMatrix", mWorld);
	g_pShaderMap->SetMatrix("g_mWorldViewProjection", mWorldViewProjNoRot);
	g_pShaderMap->SetMatrix("gLightWVP", lightWVP);
	g_pShaderMap->SetResource("SHADOW", mShadowTex->getSResourceV());
	


	

	// Set Input Assembler params
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pd3dDevice->IASetInputLayout( g_pVertexLayout );
	g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	g_pVB->Apply(0);
	
	g_pTechRenderLine = g_pShader->GetTechniqueByName("DrawLine");
	//clear render target
	g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

	//clear depth info
	g_pd3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );

	// Render line using the technique g_pRenderTextured
	//D3D10_TECHNIQUE_DESC techDesc;
	g_pTechRenderLine->GetDesc( &techDesc );

	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		g_pShader->Apply(0);
		g_pd3dDevice->Draw(numVertex, 0);
		
	}
	g_pTechRenderLine = g_pShaderMap->GetTechniqueByName("DrawLine");
	g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	g_pVBMap->Apply(0);
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		g_pShaderMap->Apply(0);
		g_pd3dDevice->Draw(4, 0);
	}
	mFont->DrawTextW(0, (LPCWSTR)mFrameStats.data(), -1, &R, DT_NOCLIP, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
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

	mTimer.reset();

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

			mTimer.tick();

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
							"Direct3D 10 : Line Drawing",
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
			case VK_ADD:
				mShadowTex->Initilize(g_pd3dDevice, 2, 2);
				mShadowTex->CreateShadowTex();
				break;
			case VK_SUBTRACT:
				mShadowTex->Initilize(g_pd3dDevice, 0.5f, 0.5f);
				mShadowTex->CreateShadowTex();
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
	if( g_pd3dDevice ) g_pd3dDevice->ClearState();

	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pd3dDevice);
}