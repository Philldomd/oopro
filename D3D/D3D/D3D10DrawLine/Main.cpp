#include "Heightmap.h"
#include "ParticleSys.h"
#include "Shader.h"
#include "Minimap.h"
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
Camera					g_camera;
D3DXMATRIX				g_mProj;
D3DXMATRIX				g_mView;
D3DXVECTOR3				direction;
POINT					oldMousePos;
D3DXMATRIX mLightView,mLightVolume,g_mLightWVP;
//buffer data
D3DXVECTOR3 tempSpeed;
ID3D10InputLayout*		g_pVertexLayout;
Buffer*					g_pVB;
Buffer*					g_pIB;
Shader*					g_pMiniShader, *g_pLightShader;
HeightMap*				g_pHeightMap;
D3D10_VIEWPORT vp;
int						numberOfPoints;
int						numIndices;
int						mapHeight = 1024, mapWidth = 1024;
//shader variables
ID3D10Effect*			g_pEffect;
ID3D10EffectTechnique*	g_pTechRenderLine;
Minimap*				m_miniMap;

//Particle system
PSystem*		mRain;
PSystem*		mFire;
PSystem*		mFire1;
PSystem*		mFire2;
float		mGameTimer;
struct LineVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 texCoord;
};
LineVertex* mesh;
struct Light
{
	D3DXVECTOR4 pos;
	D3DXVECTOR3 col;
	float Ld; //Diffuse light
};
void Keyboard_Input()
{
	D3DXVECTOR3 forwardSpeed = D3DXVECTOR3(0,0,0);
	int rotate = 0;
	if((GetKeyState(VK_UP) & 0x80) || (GetKeyState('W') & 0x80))
	{
		tempSpeed = forwardSpeed + D3DXVECTOR3(g_camera.getTarget());
		g_camera.lookAt(tempSpeed);
	}
	if((GetKeyState(VK_DOWN) & 0x80) || (GetKeyState('S') & 0x80))
	{
		g_camera.rotate(0,0);
		g_camera.lookAt(g_camera.getTarget());
	}
	if((GetKeyState(0xCB) & 0x80) || (GetKeyState('A') & 0x80))
	{
		rotate = 60.0f;
		g_camera.rotate(rotate,0);
	}
	if((GetKeyState(0xCD) & 0x80) || (GetKeyState('D') & 0x80))
	{
		rotate = -60.0f;
		g_camera.rotate(rotate,0);
	}
	else
	{
//		g_camera.rotate(0,0);
	}
	//g_camera.lookAt(D3DXVECTOR3(30,15,45));
}
void updateCamera(float dt);
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
	mRain = new PSystem();
	mRain->setEmitPos(D3DXVECTOR3(0,255,0));
	mFire = new PSystem();
	mFire1 = new PSystem();
	mFire2 = new PSystem();	
	mFire->setEmitPos(D3DXVECTOR3(-63,88,258));
	mFire1->setEmitPos(D3DXVECTOR3(115,110,-92));
	mFire2->setEmitPos(D3DXVECTOR3(75,174,222));
	g_pMiniShader = new Shader();
	g_pLightShader = new Shader();
	m_miniMap = new Minimap();
	HRESULT hr = S_OK;;
	mesh = new LineVertex[mapWidth*mapHeight];
	RECT rc;
	GetClientRect( g_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	g_camera = Camera();
	g_camera.perspective(60,(float)(width/height),1.0f,2048.0f);
	g_camera.lookAt(D3DXVECTOR3(0,15,60), D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,1,0));
	UINT createDeviceFlags = 0;
#ifdef _DEBUGdad
	createDeviceaFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
	
	D3D10_DRIVER_TYPE driverTypes[] = 
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount							= 1;
	sd.BufferDesc.Width						= width;
	sd.BufferDesc.Height					= height;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= g_hWnd;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.Windowed								= TRUE;

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
	descDepth.Width					= width;
	descDepth.Height				= height;
	descDepth.MipLevels				= 1;
	descDepth.ArraySize				= 1;
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count		= 1;
	descDepth.SampleDesc.Quality	= 0;
	descDepth.Usage					= D3D10_USAGE_DEFAULT;
	descDepth.BindFlags				= D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags		= 0;
	descDepth.MiscFlags				= 0;
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
	if( FAILED(hr) )
		return hr;

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format				= descDepth.Format;
	descDSV.ViewDimension		= D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice	= 0;
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if( FAILED(hr) )
		return hr;
	

	g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0; 

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// Setup the viewport
	
	vp.Width		= width;
	vp.Height		= height;
	vp.MinDepth		= 0.0f;
	vp.MaxDepth		= 1.0f;
	vp.TopLeftX		= 0;
	vp.TopLeftY		= 0;
	g_pd3dDevice->RSSetViewports( 1, &vp );

	//D3D10_VIEWPORT vpMini;
	//vpMini.Width		= width;
	//vpMini.Height		= height;
	//vpMini.MinDepth		= 0.0f;
	//vpMini.MaxDepth		= 1.0f;
	//vpMini.TopLeftX		= 0;
	//vpMini.TopLeftY		= 0;
	//g_pd3dDevice->RSSetViewports( 2, &vpMini );


	//Create vertex buffer to hold maxAmount of particles
	g_pHeightMap = new HeightMap();
	
	numberOfPoints = mapHeight*mapWidth;
	g_pHeightMap->loadRAW(mapHeight,mapWidth,"HeightMapTest.raw", 1.0f, -20);
	float invTwoDX = 1.0f / (2.0f * 1.0f);
	float invTwoDZ = 1.0f / (2.0f * 1.0f);
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			DWORD index = i * mapWidth + j;
			float y = g_pHeightMap->GetY(i,j);
			mesh[index].pos = D3DXVECTOR3(i-(mapHeight/2), y, j-(mapWidth/2));
			mesh[index].texCoord.x = (mesh[index].pos.x + (0.1f*mapWidth)) / 128;
			mesh[index].texCoord.y = (mesh[index].pos.z - (0.1f*mapHeight)) / 128;
			if(i >= 2 && i < mapHeight -1 && j >= 2 && i < mapWidth -1)
			{
				float t = g_pHeightMap->GetY(i - 1, j);
				float b = g_pHeightMap->GetY(i + 1, j);
				float l = g_pHeightMap->GetY(i, j - 1);
				float r = g_pHeightMap->GetY(i, j + 1);

				D3DXVECTOR3 tanZ(0.0f, (t-b)*invTwoDZ, 1.0f);
				D3DXVECTOR3 tanX(1.0f, (r-l)*invTwoDX, 0.0f);

				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &tanZ, &tanX);
				D3DXVec3Normalize(&N, &N);

				mesh[i*mapHeight+j].norm = N;
			}			
			
		}
	}

	BUFFER_INIT_DESC bd;
	bd.Usage			= BUFFER_CPU_WRITE;
	bd.ElementSize		= sizeof( LineVertex );
	bd.NumElements		= numberOfPoints;
	bd.Type				= VERTEX_BUFFER;
	bd.InitData			= mesh;
	g_pVB = new Buffer();
	if( FAILED( g_pVB->Init(g_pd3dDevice, bd ) ) )
		MessageBox( 0, "Unable to create Vertex Buffer", "VB Error", 0 );
	SAFE_DELETE_ARRAY(mesh);
	numIndices = (mapWidth * 2) * (mapHeight - 1) + (mapHeight - 2);
	int* indices = new int[numIndices];
	int index = 0;
	for (int z = 0; z < mapHeight -1; z++)
	{
		//Even rows move left to right, odd rows right to left;
		if ( z % 2 == 0 )
		{
			int x;
			for ( x = 0; x < mapWidth; x++ )
			{
				indices[index++] = x + (z * mapWidth);
				indices[index++] = x + (z * mapWidth) + mapWidth;
			}
			// Insert degenerate vertex if this isn't the last row
			if ( z != mapHeight - 2 )
			{
				indices[index++] = --x + (z * mapWidth);
			}
		}
		else
		{
			//Odd row
			int x;
			for ( x = mapWidth - 1; x >=0; x-- )
			{
				indices[index++] = x + (z * mapWidth);
				indices[index++] = x + (z * mapWidth) + mapWidth;
			}
			if (z != mapHeight - 2 )
			{
				indices[index++] = ++x + (z * mapWidth);
			}
		}
	}
	
	BUFFER_INIT_DESC indexDesc;
	indexDesc.Usage = BUFFER_DEFAULT;
	indexDesc.ElementSize = sizeof( int );
	indexDesc.NumElements = numIndices;
	indexDesc.Type = INDEX_BUFFER;
	indexDesc.InitData = indices;
	g_pIB = new Buffer();
	if( FAILED( g_pIB->Init(g_pd3dDevice, indexDesc ) ) )
		MessageBox( 0, "Unable to create Vertex Buffer", "IB Error", 0 );
	SAFE_DELETE_ARRAY(indices);

	//Init shader effect
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
	ID3DBlob* compilationErrors = 0;
	hr = D3DX10CreateEffectFromFile(	"Line.fx",
											NULL,
											NULL,
											"fx_4_0",
											dwShaderFlags,
											0,
											g_pd3dDevice,
											NULL,
											NULL,
											&g_pEffect,
											NULL,
											NULL);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0,(char*) compilationErrors->GetBufferPointer(),0,0);
			compilationErrors->Release();
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr,
			"D3DX10CreateEffectFromFile", true);
	}

	g_pTechRenderLine = g_pEffect->GetTechniqueByName("DrawLine");

	// Define our vertex data layout
	const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	D3D10_PASS_DESC PassDesc;
	g_pTechRenderLine->GetPassByIndex(0)->GetDesc(&PassDesc);

	//load texture
	ID3D10ShaderResourceView *pSRView = NULL;
	D3DX10CreateShaderResourceViewFromFile(g_pd3dDevice,"grass.jpg",NULL,NULL,&pSRView,NULL);

	g_pEffect->GetVariableByName("GRASS")->AsShaderResource()->SetResource(pSRView);

	pSRView->Release();
	
	//Create Input Layout (== Vertex Declaration)
	
	m_miniMap->initialize(g_pd3dDevice);
	m_miniMap->CreateTex();

	g_pd3dDevice->CreateInputLayout(lineVertexLayout,
		sizeof(lineVertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&g_pVertexLayout );
	const D3D10_INPUT_ELEMENT_DESC minimapLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	if(FAILED( g_pMiniShader->Init(g_pd3dDevice, 
		"miniMap.fx", 
		minimapLayout, 
		sizeof(minimapLayout)/ sizeof(D3D10_INPUT_ELEMENT_DESC),
		"RB", 
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	const D3D10_INPUT_ELEMENT_DESC shadowInput[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	if( FAILED( g_pLightShader->Init(g_pd3dDevice, 
		"ShadowMap.fx", 
		shadowInput, 
		sizeof(shadowInput) / sizeof(D3D10_INPUT_ELEMENT_DESC), 
		"RenderShadowMap",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}
	mRain->init(g_pd3dDevice, g_pEffect, 100000,"rain.fx");
	mFire->init(g_pd3dDevice, g_pEffect, 128, "fire.fx");
	mFire1->init(g_pd3dDevice, g_pEffect, 128, "fire.fx");
	mFire2->init(g_pd3dDevice, g_pEffect, 128, "fire.fx");

	

	D3DXMatrixLookAtLH(&mLightView, &D3DXVECTOR3(0,500,0),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(-1, 0, 0));
	D3DXMatrixOrthoLH(&mLightVolume, 200, 200, 1.0f, 1024.0f);

	g_mLightWVP = mLightView * mLightVolume;
	return S_OK;
}



void Update(float deltaTime)
{
	//camera
	mGameTimer += deltaTime; 	
	updateCamera(deltaTime);
	//g_camera.lookAt(D3DXVECTOR3(0, g_pHeightMap->getHeight(g_camera.getPosition().x, g_camera.getPosition().z)+5,0));
	mRain->update(deltaTime, mGameTimer);
	mFire->update(deltaTime, mGameTimer);
	mFire1->update(deltaTime, mGameTimer);
	mFire2->update(deltaTime, mGameTimer);
}

HRESULT Render()
{
	// Render the result
	D3DXMATRIX mWorldViewProj;
	D3DXMATRIX mEyeWorldView;
	D3DXMATRIX normalMatrix;

	

	//light
	Light light;

	light.pos = D3DXVECTOR4(300,500,0,0);
	light.col = D3DXVECTOR3(1,1,1);
	light.Ld = 1.0f;
	D3DXVec4Transform(&(light.pos),&(light.pos),&g_mView);

	//RECT rc;
	float width = 1024;
	float height = 768;
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pd3dDevice->RSSetViewports(1, &vp);
	g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pd3dDevice->ClearDepthStencilView(g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
	
	mWorldViewProj = g_camera.getViewMatrix() * g_camera.getProjectionMatrix();
	mEyeWorldView = g_camera.getViewMatrix();
	//-------------------------------
	ID3D10RenderTargetView* lol;
	lol = m_miniMap->getRTV();
	//g_pd3dDevice->OMSetRenderTargets(1,&lol, g_pDepthStencilView);
	//g_pd3dDevice->RSSetViewports(1, m_miniMap->getVP());


	//g_pd3dDevice->ClearRenderTargetView(lol, ClearColor);
	//g_pd3dDevice->ClearDepthStencilView(g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);



	g_pMiniShader->SetTechniqueByName("RB");
	//g_pEffect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix()->SetMatrix( (float*)&g_mLightWVP );
	g_pEffect->GetVariableByName("g_mWorldViewProjection")->AsMatrix()->SetMatrix(g_mLightWVP);
	//Render 
	g_pd3dDevice->IASetInputLayout( g_pVertexLayout );
	g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//g_pMiniShader->Apply(0);

	g_pEffect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix()->SetMatrix( (float*)&g_mLightWVP );
	g_pEffect->GetVariableByName( "LPos")->AsVector()->SetFloatVector((float*)&(light.pos));
	g_pEffect->GetVariableByName( "LCol")->AsVector()->SetFloatVector((float*)&(light.col));
	g_pEffect->GetVariableByName( "Ld")->AsScalar()->SetFloat(light.Ld);
	g_pEffect->GetVariableByName( "mEyeWorldView" )->AsMatrix()->SetMatrix( (float*)&mEyeWorldView );
	
	D3D10_TECHNIQUE_DESC techDesc;
	g_pTechRenderLine->GetDesc( &techDesc );

	g_pVB->Apply(0);
	g_pIB->Apply(0);
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		g_pTechRenderLine->GetPassByIndex( p )->Apply(0);
		g_pd3dDevice->DrawIndexed(numIndices, 0,0);
	}
	//g_pd3dDevice->OMSetRenderTargets(0,0,NULL);
	//----------------------------------

	
	

	float DetNM = D3DXMatrixDeterminant(&normalMatrix);
	D3DXMatrixInverse(&normalMatrix, &DetNM, &normalMatrix);
	D3DXMatrixTranspose(&normalMatrix, &normalMatrix);
	// Set variables
	g_pEffect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix()->SetMatrix( (float*)&mWorldViewProj );
	g_pEffect->GetVariableByName( "LPos")->AsVector()->SetFloatVector((float*)&(light.pos));
	g_pEffect->GetVariableByName( "LCol")->AsVector()->SetFloatVector((float*)&(light.col));
	g_pEffect->GetVariableByName( "Ld")->AsScalar()->SetFloat(light.Ld);
	g_pEffect->GetVariableByName( "mEyeWorldView" )->AsMatrix()->SetMatrix( (float*)&mEyeWorldView );
	// Set Input Assembler params

	g_pd3dDevice->IASetInputLayout( g_pVertexLayout );
	g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	g_pVB->Apply(0);
	g_pIB->Apply(0);
	
	//clear render target
	g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

	//clear depth info
	g_pd3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );

	// Render line using the technique g_pRenderTextured
	
	g_pTechRenderLine->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		g_pTechRenderLine->GetPassByIndex( p )->Apply(0);
		g_pd3dDevice->DrawIndexed(numIndices,0,0);
	}
	//mRain->setEyePos(g_camera.getPosition());
	//mRain->draw(g_camera);
	//mFire->setEyePos(g_camera.getPosition());
	//mFire->draw(g_camera);
	//mFire1->setEyePos(g_camera.getPosition());
	//mFire1->draw(g_camera);
	//mFire2->setEyePos(g_camera.getPosition());
	//mFire2->draw(g_camera);

	//--------------------------------------
	g_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	g_pMiniShader->SetResource("miniMap",m_miniMap->getSRV());
	g_pMiniShader->SetTechniqueByName("RB");

	g_pMiniShader->Apply(0);

	//g_d3dDevice->DrawIndexed(1, 0, 0);
	g_pd3dDevice->Draw(1,0);

	//release shadow map
	g_pMiniShader->SetResource("miniMap", 0);
	g_pMiniShader->GetTechniqueByName("RB")->GetPassByIndex(0)->Apply(0);


	//--------------------------------------
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
	POINT mousePos;
	
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
		}
		break;
	case WM_LBUTTONDOWN:
		if(wParam & MK_LBUTTON)
		{
			SetCapture(hWnd);
			oldMousePos.x = LOWORD(lParam);
			oldMousePos.y = HIWORD(lParam);

		}
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;

	case WM_MOUSEMOVE:
		if( wParam & MK_LBUTTON )
		{
			mousePos.x = (int)LOWORD(lParam);
			mousePos.y = (int)HIWORD(lParam);

			float dx = mousePos.x - oldMousePos.x;
			float dy = mousePos.y - oldMousePos.y;

			/*g_camera.pitch(dy * 0.0087f);
			g_camera.rotateY(dx * 0.0087f);*/
			

			oldMousePos = mousePos;
		}
		return 0 ;
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
void updateCamera(float dt)
{	
	Keyboard_Input();
	g_camera.update(dt);
}
