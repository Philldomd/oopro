//-----------------------------------------------------------------------------
// Copyright (c) 2006-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------
//
// This is the third demo in the Direct3D camera demo series and it builds
// on the previous demo (http://www.dhpoware.com/downloads/D3DCamera2.zip).
//
// In this demo the third person camera model is implemented. The third person
// camera model is also sometimes referred to as the chase camera model. The
// third person camera is different to the first person and flight simulation
// camera models in that the user does not explicitly control the third person
// camera.
//
// A third person camera is attached to an object in the game world. The third
// person camera is typically positioned at some distance behind and above this
// object. As the user moves and rotates this object about the game world, the
// third person camera will follow this object and reposition itself so that it
// is always oriented behind and above this object.
//
// The third person camera model in this demo does not implement any dampening
// or lag when the camera repositions itself in response to the target object
// being moved and rotated. This results in the camera snapping to its new
// position and orientation. To minimize this behavior time based movement and
// rotations are used. A future demo will address this problem.
//
//-----------------------------------------------------------------------------

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#if defined(_DEBUG)
#define D3D_DEBUG_INFO
#endif

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>
#include <stdexcept>
#include <string>

#if defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "entity3d.h"
#include "input.h"
#include "third_person_camera.h"

//-----------------------------------------------------------------------------
// Macros.
//-----------------------------------------------------------------------------

#define SAFE_RELEASE(x) if ((x) != 0) { (x)->Release(); (x) = 0; }

//-----------------------------------------------------------------------------
// Constants.
//-----------------------------------------------------------------------------

#if !defined(CLEARTYPE_QUALITY)
#define CLEARTYPE_QUALITY 5
#endif

#define APP_TITLE "D3D Third Person Camera Demo 1"

const float BALL_FORWARD_SPEED = 60.0f;
const float BALL_HEADING_SPEED = 60.0f;
const float BALL_ROLLING_SPEED = 140.0f;

const float FLOOR_WIDTH = 1024.0f;
const float FLOOR_HEIGHT = 1024.0f;
const float FLOOR_TILE_U = 4.0f;
const float FLOOR_TILE_V = 4.0f;

const float CAMERA_FOVX = 80.0f;
const float CAMERA_ZFAR = FLOOR_WIDTH * 2.0f;
const float CAMERA_ZNEAR = 1.0f;

const float LIGHT_RADIUS = max(FLOOR_WIDTH, FLOOR_HEIGHT);
const float LIGHT_POS[3] = {0.0f, LIGHT_RADIUS * 0.5f, 0.0f};
const float LIGHT_DIR[3] = {0.0f, -1.0f, 0.0f};
const float LIGHT_SPOT_INNER_CONE = D3DXToRadian(30.0f);
const float LIGHT_SPOT_OUTER_CONE = D3DXToRadian(100.0f);

//-----------------------------------------------------------------------------
// Types.
//-----------------------------------------------------------------------------

struct Vertex
{
    float pos[3];
    float texCoord[2];
    float normal[3];
};

struct Light
{
    float dir[3];
    float pos[3];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float spotInnerCone;
    float spotOuterCone;
    float radius;
};

struct Material
{
    float ambient[4];
    float diffuse[4];
    float emissive[4];
    float specular[4];
    float shininess;
};

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

HWND                         g_hWnd;
HINSTANCE                    g_hInstance;
D3DPRESENT_PARAMETERS        g_params;
IDirect3D9                  *g_pDirect3D;
IDirect3DDevice9            *g_pDevice;
ID3DXFont                   *g_pFont;
ID3DXEffect                 *g_pEffect;
ID3DXMesh                   *g_pBallMesh;
IDirect3DTexture9           *g_pNullTexture;
IDirect3DTexture9           *g_pFloorTexture;
IDirect3DTexture9           *g_pBallTexture;
IDirect3DVertexDeclaration9 *g_pVertexDecl;
IDirect3DVertexBuffer9      *g_pVertexBuffer;
bool                         g_enableVerticalSync;
bool                         g_isFullScreen;
bool                         g_hasFocus;
bool                         g_displayHelp;
bool                         g_disableColorMapTexture;
DWORD                        g_msaaSamples;
DWORD                        g_maxAnisotrophy;
int                          g_framesPerSecond;
int                          g_windowWidth;
int                          g_windowHeight;
float                        g_globalAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float                        g_ballRadius;
ThirdPersonCamera            g_camera;
Entity3D                     g_ball;

Light g_light =
{
    LIGHT_DIR[0], LIGHT_DIR[1], LIGHT_DIR[2],   // dir
    LIGHT_POS[0], LIGHT_POS[1], LIGHT_POS[2],   // pos
    1.0f, 1.0f, 1.0f, 1.0f,                     // ambient
    1.0f, 1.0f, 1.0f, 1.0f,                     // diffuse
    1.0f, 1.0f, 1.0f, 1.0f,                     // specular
    LIGHT_SPOT_INNER_CONE,                      // spotInnerCone
    LIGHT_SPOT_OUTER_CONE,                      // spotOuterCone
    LIGHT_RADIUS                                // radius
};

Material g_material =
{
    0.2f, 0.2f, 0.2f, 1.0f,                     // ambient
    0.8f, 0.8f, 0.8f, 1.0f,                     // diffuse
    0.0f, 0.0f, 0.0f, 1.0f,                     // emissive
    0.0f, 0.0f, 0.0f, 1.0f,                     // specular
    0.0f                                        // shininess
};

D3DVERTEXELEMENT9 g_vertexElements[] =
{
    {0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    {0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
    D3DDECL_END()
};

//-----------------------------------------------------------------------------
// Function Prototypes.
//-----------------------------------------------------------------------------

void    ChooseBestMSAAMode(D3DFORMAT backBufferFmt, D3DFORMAT depthStencilFmt,
                           BOOL windowed, D3DMULTISAMPLE_TYPE &type,
                           DWORD &qualityLevels, DWORD &samplesPerPixel);
void    Cleanup();
void    CleanupApp();
float   ClipBallToFloor(const Entity3D &ball, float forwardSpeed, float elapsedTimeSec);
HWND    CreateAppWindow(const WNDCLASSEX &wcl, const char *pszTitle);
bool    CreateNullTexture(int width, int height, LPDIRECT3DTEXTURE9 &pTexture);
bool    DeviceIsValid();
float   GetElapsedTimeInSeconds();
bool    Init();
void    InitApp();
bool    InitD3D();
void    InitFloor(float width, float height, float uTile, float vTile);
bool    InitFont(const char *pszFont, int ptSize, LPD3DXFONT &pFont);
bool    LoadShader(const char *pszFilename, LPD3DXEFFECT &pEffect);
void    Log(const char *pszMessage);
bool    MSAAModeSupported(D3DMULTISAMPLE_TYPE type, D3DFORMAT backBufferFmt,
                          D3DFORMAT depthStencilFmt, BOOL windowed,
                          DWORD &qualityLevels);
void    ProcessUserInput();
void    RenderBall();
void    RenderFloor();
void    RenderFrame();
void    RenderText();
bool    ResetDevice();
void    SetProcessorAffinity();
void    ToggleFullScreen();
void    UpdateBall(float elapsedTimeSec);
void    UpdateFrame(float elapsedTimeSec);
void    UpdateFrameRate(float elapsedTimeSec);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

    MSG msg = {0};
    WNDCLASSEX wcl = {0};

    wcl.cbSize = sizeof(wcl);
    wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wcl.lpfnWndProc = WindowProc;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hInstance = g_hInstance = hInstance;
    wcl.hIcon = LoadIcon(0, IDI_APPLICATION);
    wcl.hCursor = LoadCursor(0, IDC_ARROW);
    wcl.hbrBackground = 0;
    wcl.lpszMenuName = 0;
    wcl.lpszClassName = "D3D9WindowClass";
    wcl.hIconSm = 0;

    if (!RegisterClassEx(&wcl))
        return 0;

    g_hWnd = CreateAppWindow(wcl, APP_TITLE);

    if (g_hWnd)
    {
        SetProcessorAffinity();

        if (Init())
        {
            ShowWindow(g_hWnd, nShowCmd);
            UpdateWindow(g_hWnd);

            while (true)
            {
                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                        break;

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (msg.message == WM_QUIT)
                    break;

                if (g_hasFocus)
                {
                    UpdateFrame(GetElapsedTimeInSeconds());

                    if (DeviceIsValid())
                        RenderFrame();
                }
                else
                {
                    WaitMessage();
                }
            }
        }

        Cleanup();
        UnregisterClass(wcl.lpszClassName, hInstance);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
        switch (wParam)
        {
        default:
            break;

        case WA_ACTIVE:
        case WA_CLICKACTIVE:
            g_hasFocus = true;
            break;

        case WA_INACTIVE:
            if (g_isFullScreen)
                ShowWindow(hWnd, SW_MINIMIZE);
            g_hasFocus = false;
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        g_windowWidth = static_cast<int>(LOWORD(lParam));
        g_windowHeight = static_cast<int>(HIWORD(lParam));
        break;

    default:
        Keyboard::instance().handleMsg(hWnd, msg, wParam, lParam);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ChooseBestMSAAMode(D3DFORMAT backBufferFmt, D3DFORMAT depthStencilFmt,
                        BOOL windowed, D3DMULTISAMPLE_TYPE &type,
                        DWORD &qualityLevels, DWORD &samplesPerPixel)
{
    bool supported = false;

    struct MSAAMode
    {
        D3DMULTISAMPLE_TYPE type;
        DWORD samples;
    }
    multsamplingTypes[15] =
    {
        { D3DMULTISAMPLE_16_SAMPLES,  16 },
        { D3DMULTISAMPLE_15_SAMPLES,  15 },
        { D3DMULTISAMPLE_14_SAMPLES,  14 },
        { D3DMULTISAMPLE_13_SAMPLES,  13 },
        { D3DMULTISAMPLE_12_SAMPLES,  12 },
        { D3DMULTISAMPLE_11_SAMPLES,  11 },
        { D3DMULTISAMPLE_10_SAMPLES,  10 },
        { D3DMULTISAMPLE_9_SAMPLES,   9  },
        { D3DMULTISAMPLE_8_SAMPLES,   8  },
        { D3DMULTISAMPLE_7_SAMPLES,   7  },
        { D3DMULTISAMPLE_6_SAMPLES,   6  },
        { D3DMULTISAMPLE_5_SAMPLES,   5  },
        { D3DMULTISAMPLE_4_SAMPLES,   4  },
        { D3DMULTISAMPLE_3_SAMPLES,   3  },
        { D3DMULTISAMPLE_2_SAMPLES,   2  }
    };

    for (int i = 0; i < 15; ++i)
    {
        type = multsamplingTypes[i].type;

        supported = MSAAModeSupported(type, backBufferFmt, depthStencilFmt,
                        windowed, qualityLevels);

        if (supported)
        {
            samplesPerPixel = multsamplingTypes[i].samples;
            return;
        }
    }

    type = D3DMULTISAMPLE_NONE;
    qualityLevels = 0;
    samplesPerPixel = 1;
}

void Cleanup()
{
    CleanupApp();

    SAFE_RELEASE(g_pNullTexture);
    SAFE_RELEASE(g_pFont);
    SAFE_RELEASE(g_pDevice);
    SAFE_RELEASE(g_pDirect3D);
}

void CleanupApp()
{
    SAFE_RELEASE(g_pBallTexture);
    SAFE_RELEASE(g_pFloorTexture);
    SAFE_RELEASE(g_pEffect);
    SAFE_RELEASE(g_pBallMesh);
    SAFE_RELEASE(g_pVertexDecl);
    SAFE_RELEASE(g_pVertexBuffer);
}

float ClipBallToFloor(const Entity3D &ball, float forwardSpeed, float elapsedTimeSec)
{
    // Perform very simple collision detection to prevent the ball from
    // moving beyond the edges of the floor. Notice that we are predicting
    // whether the ball will move beyond the edges of the floor based on the
    // ball's current forward velocity and the amount of time that has elapsed.

    float floorBoundaryX = FLOOR_WIDTH * 0.5f - g_ballRadius;
    float floorBoundaryZ = FLOOR_HEIGHT * 0.5f - g_ballRadius;
    float velocity = forwardSpeed * elapsedTimeSec;
    D3DXVECTOR3 newBallPos = ball.getPosition() + ball.getForwardVector() * velocity;

    if (newBallPos.z > -floorBoundaryZ && newBallPos.z < floorBoundaryZ)
    {
        if (newBallPos.x > -floorBoundaryX && newBallPos.x < floorBoundaryX)
            return forwardSpeed; // ball will still be within floor's bounds
    }

    return 0.0f; // ball will be outside of floor's bounds...so stop the ball
}

HWND CreateAppWindow(const WNDCLASSEX &wcl, const char *pszTitle)
{
    // Create a window that is centered on the desktop. It's exactly 1/4 the
    // size of the desktop. Don't allow it to be resized.

    DWORD wndExStyle = WS_EX_OVERLAPPEDWINDOW;
    DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                     WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    HWND hWnd = CreateWindowEx(wndExStyle, wcl.lpszClassName, pszTitle,
                    wndStyle, 0, 0, 0, 0, 0, 0, wcl.hInstance, 0);

    if (hWnd)
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int halfScreenWidth = screenWidth / 2;
        int halfScreenHeight = screenHeight / 2;
        int left = (screenWidth - halfScreenWidth) / 2;
        int top = (screenHeight - halfScreenHeight) / 2;
        RECT rc = {0};

        SetRect(&rc, left, top, left + halfScreenWidth, top + halfScreenHeight);
        AdjustWindowRectEx(&rc, wndStyle, FALSE, wndExStyle);
        MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

        GetClientRect(hWnd, &rc);
        g_windowWidth = rc.right - rc.left;
        g_windowHeight = rc.bottom - rc.top;
    }

    return hWnd;
}

bool CreateNullTexture(int width, int height, LPDIRECT3DTEXTURE9 &pTexture)
{
    // Create an empty white texture. This texture is applied to geometry
    // that doesn't have any texture maps. This trick allows the same shader to
    // be used to draw the geometry with and without textures applied.

    HRESULT hr = D3DXCreateTexture(g_pDevice, width, height, 0, 0,
                    D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture);

    if (FAILED(hr))
        return false;

    LPDIRECT3DSURFACE9 pSurface = 0;

    if (SUCCEEDED(pTexture->GetSurfaceLevel(0, &pSurface)))
    {
        D3DLOCKED_RECT rcLock = {0};

        if (SUCCEEDED(pSurface->LockRect(&rcLock, 0, 0)))
        {
            BYTE *pPixels = static_cast<BYTE*>(rcLock.pBits);
            int widthInBytes = width * 4;

            if (widthInBytes == rcLock.Pitch)
            {
                memset(pPixels, 0xff, widthInBytes * height);
            }
            else
            {
                for (int y = 0; y < height; ++y)
                    memset(&pPixels[y * rcLock.Pitch], 0xff, rcLock.Pitch);
            }

            pSurface->UnlockRect();
            pSurface->Release();
            return true;
        }

        pSurface->Release();
    }

    pTexture->Release();
    return false;
}

bool DeviceIsValid()
{
    HRESULT hr = g_pDevice->TestCooperativeLevel();

    if (FAILED(hr))
    {
        if (hr == D3DERR_DEVICENOTRESET)
            return ResetDevice();
    }

    return true;
}

float GetElapsedTimeInSeconds()
{
    // Returns the elapsed time (in seconds) since the last time this function
    // was called. This elaborate setup is to guard against large spikes in
    // the time returned by QueryPerformanceCounter().

    static const int MAX_SAMPLE_COUNT = 50;

    static float frameTimes[MAX_SAMPLE_COUNT];
    static float timeScale = 0.0f;
    static float actualElapsedTimeSec = 0.0f;
    static INT64 freq = 0;
    static INT64 lastTime = 0;
    static int sampleCount = 0;
    static bool initialized = false;

    INT64 time = 0;
    float elapsedTimeSec = 0.0f;

    if (!initialized)
    {
        initialized = true;
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&lastTime));
        timeScale = 1.0f / freq;
    }

    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
    elapsedTimeSec = (time - lastTime) * timeScale;
    lastTime = time;

    if (fabsf(elapsedTimeSec - actualElapsedTimeSec) < 1.0f)
    {
        memmove(&frameTimes[1], frameTimes, sizeof(frameTimes) - sizeof(frameTimes[0]));
        frameTimes[0] = elapsedTimeSec;

        if (sampleCount < MAX_SAMPLE_COUNT)
            ++sampleCount;
    }

    actualElapsedTimeSec = 0.0f;

    for (int i = 0; i < sampleCount; ++i)
        actualElapsedTimeSec += frameTimes[i];

    if (sampleCount > 0)
        actualElapsedTimeSec /= sampleCount;

    return actualElapsedTimeSec;
}

bool Init()
{
    if (!InitD3D())
    {
        Log("Direct3D initialization failed!");
        return false;
    }

    try
    {
        InitApp();
        return true;
    }
    catch (const std::exception &e)
    {
        std::ostringstream msg;

        msg << "Application initialization failed!" << std::endl << std::endl;
        msg << e.what();

        Log(msg.str().c_str());
        return false;
    }
}

void InitApp()
{
    // Setup fonts.

    if (!InitFont("Arial", 10, g_pFont))
        throw std::runtime_error("Failed to create font.");

    // Setup textures.

    if (!CreateNullTexture(2, 2, g_pNullTexture))
        throw std::runtime_error("Failed to create null texture.");

    if (FAILED(D3DXCreateTextureFromFile(g_pDevice, "ball_decal_map.dds", &g_pBallTexture)))
        throw std::runtime_error("Failed to load texture: ball_decal_map.dds.");

    if (FAILED(D3DXCreateTextureFromFile(g_pDevice, "floor_decal_map.dds", &g_pFloorTexture)))
        throw std::runtime_error("Failed to load texture: floor_decal_map.dds.");

    // Setup shaders.

    if (!LoadShader("blinn_phong.fx", g_pEffect))
        throw std::runtime_error("Failed to load shader: blinn_phong.fx.");

    // Setup the floor geometry.

    InitFloor(FLOOR_WIDTH, FLOOR_HEIGHT, FLOOR_TILE_U, FLOOR_TILE_V);

    // Setup the ball mesh.

    if (FAILED(D3DXLoadMeshFromX("ball.x", D3DXMESH_MANAGED, g_pDevice, 0, 0, 0, 0, &g_pBallMesh)))
        throw std::runtime_error("Failed to load mesh: ball.x.");

    D3DXVECTOR3 *pVertices = 0;

    if (FAILED(g_pBallMesh->LockVertexBuffer(0, reinterpret_cast<LPVOID*>(&pVertices))))
        throw std::runtime_error("Failed to lock ball mesh vertex buffer.");

    D3DXVECTOR3 center;

    D3DXComputeBoundingSphere(pVertices, g_pBallMesh->GetNumVertices(),
        g_pBallMesh->GetNumBytesPerVertex(), &center, &g_ballRadius);

    g_pBallMesh->UnlockVertexBuffer();

    // Initialize the ball.

    g_ball.constrainToWorldYAxis(true);
    g_ball.setPosition(0.0f, 1.0f + g_ballRadius, 0.0f);

    // Setup camera.

    g_camera.perspective(CAMERA_FOVX,
        static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight),
        CAMERA_ZNEAR, CAMERA_ZFAR);

    g_camera.lookAt(D3DXVECTOR3(0.0f, g_ballRadius * 3.0f, -g_ballRadius * 7.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}

bool InitD3D()
{
    HRESULT hr = 0;
    D3DDISPLAYMODE desktop = {0};

    g_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!g_pDirect3D)
        return false;

    // Just use the current desktop display mode.
    hr = g_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &desktop);

    if (FAILED(hr))
    {
        g_pDirect3D->Release();
        g_pDirect3D = 0;
        return false;
    }

    // Setup Direct3D for windowed rendering.
    g_params.BackBufferWidth = 0;
    g_params.BackBufferHeight = 0;
    g_params.BackBufferFormat = desktop.Format;
    g_params.BackBufferCount = 1;
    g_params.hDeviceWindow = g_hWnd;
    g_params.Windowed = TRUE;
    g_params.EnableAutoDepthStencil = TRUE;
    g_params.AutoDepthStencilFormat = D3DFMT_D24S8;
    g_params.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    g_params.FullScreen_RefreshRateInHz = 0;

    if (g_enableVerticalSync)
        g_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    else
        g_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Swap effect must be D3DSWAPEFFECT_DISCARD for multi-sampling support.
    g_params.SwapEffect = D3DSWAPEFFECT_DISCARD;

    // Select the highest quality multi-sample anti-aliasing (MSAA) mode.
    ChooseBestMSAAMode(g_params.BackBufferFormat, g_params.AutoDepthStencilFormat,
        g_params.Windowed, g_params.MultiSampleType, g_params.MultiSampleQuality,
        g_msaaSamples);

    // Most modern video cards should have no problems creating pure devices.
    // Note that by creating a pure device we lose the ability to debug vertex
    // and pixel shaders.
    hr = g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
            &g_params, &g_pDevice);

    if (FAILED(hr))
    {
        // Fall back to software vertex processing for less capable hardware.
        // Note that in order to debug vertex shaders we must use a software
        // vertex processing device.
        hr = g_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_params, &g_pDevice);
    }

    if (FAILED(hr))
    {
        g_pDirect3D->Release();
        g_pDirect3D = 0;
        return false;
    }

    D3DCAPS9 caps;

    // Prefer anisotropic texture filtering if it's supported.
    if (SUCCEEDED(g_pDevice->GetDeviceCaps(&caps)))
    {
        if (caps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY)
            g_maxAnisotrophy = caps.MaxAnisotropy;
        else
            g_maxAnisotrophy = 1;
    }

    return true;
}

void InitFloor(float width, float height, float uTile, float vTile)
{
    HRESULT hr = 0;

    hr = g_pDevice->CreateVertexDeclaration(g_vertexElements, &g_pVertexDecl);

    if (FAILED(hr))
        throw std::runtime_error("Failed to create floor vertex declaration.");

    hr = g_pDevice->CreateVertexBuffer(sizeof(Vertex) * 4, 0, 0,
        D3DPOOL_MANAGED, &g_pVertexBuffer, 0);

    if (FAILED(hr))
        throw std::runtime_error("Failed to create floor vertex buffer.");

    Vertex *pVertices = 0;

    hr = g_pVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    if (FAILED(hr))
        throw std::runtime_error("Failed to lock floor vertex buffer.");

    pVertices[0].pos[0] = -width * 0.5f;
    pVertices[0].pos[1] = 0.0f;
    pVertices[0].pos[2] = height * 0.5f;
    pVertices[0].texCoord[0] = 0.0f;
    pVertices[0].texCoord[1] = 0.0f;
    pVertices[0].normal[0] = 0.0f;
    pVertices[0].normal[1] = 1.0f;
    pVertices[0].normal[2] = 0.0f;

    pVertices[1].pos[0] = width * 0.5f;
    pVertices[1].pos[1] = 0.0f;
    pVertices[1].pos[2] = height * 0.5f;
    pVertices[1].texCoord[0] = uTile;
    pVertices[1].texCoord[1] = 0.0f;
    pVertices[1].normal[0] = 0.0f;
    pVertices[1].normal[1] = 1.0f;
    pVertices[1].normal[2] = 0.0f;

    pVertices[2].pos[0] = -width * 0.5f;
    pVertices[2].pos[1] = 0.0f;
    pVertices[2].pos[2] = -height * 0.5f;
    pVertices[2].texCoord[0] = 0.0f;
    pVertices[2].texCoord[1] = vTile;
    pVertices[2].normal[0] = 0.0f;
    pVertices[2].normal[1] = 1.0f;
    pVertices[2].normal[2] = 0.0f;

    pVertices[3].pos[0] = width * 0.5f;
    pVertices[3].pos[1] = 0.0f;
    pVertices[3].pos[2] = -height * 0.5f;
    pVertices[3].texCoord[0] = uTile;
    pVertices[3].texCoord[1] = vTile;
    pVertices[3].normal[0] = 0.0f;
    pVertices[3].normal[1] = 1.0f;
    pVertices[3].normal[2] = 0.0f;

    g_pVertexBuffer->Unlock();
}

bool InitFont(const char *pszFont, int ptSize, LPD3DXFONT &pFont)
{
    static DWORD dwQuality = 0;

    // Prefer ClearType font quality if available.

    if (!dwQuality)
    {
        DWORD dwVersion = GetVersion();
        DWORD dwMajorVersion = static_cast<DWORD>((LOBYTE(LOWORD(dwVersion))));
        DWORD dwMinorVersion = static_cast<DWORD>((HIBYTE(LOWORD(dwVersion))));

        // Windows XP and higher will support ClearType quality fonts.
        if (dwMajorVersion >= 6 || (dwMajorVersion == 5 && dwMinorVersion == 1))
            dwQuality = CLEARTYPE_QUALITY;
        else
            dwQuality = ANTIALIASED_QUALITY;
    }

    int logPixelsY = 0;

    // Convert from font point size to pixel size.

    if (HDC hDC = GetDC((0)))
    {
        logPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
        ReleaseDC(0, hDC);
    }

    int fontCharHeight = -ptSize * logPixelsY / 72;

    // Now create the font. Prefer anti-aliased text.

    HRESULT hr = D3DXCreateFont(
        g_pDevice,
        fontCharHeight,                 // height
        0,                              // width
        FW_BOLD,                        // weight
        1,                              // mipmap levels
        FALSE,                          // italic
        DEFAULT_CHARSET,                // char set
        OUT_DEFAULT_PRECIS,             // output precision
        dwQuality,                      // quality
        DEFAULT_PITCH | FF_DONTCARE,    // pitch and family
        pszFont,                        // font name
        &pFont);

    return SUCCEEDED(hr) ? true : false;
}

bool LoadShader(const char *pszFilename, LPD3DXEFFECT &pEffect)
{
    ID3DXBuffer *pCompilationErrors = 0;
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER;

    // Both vertex and pixel shaders can be debugged. To enable shader
    // debugging add the following flag to the dwShaderFlags variable:
    //      dwShaderFlags |= D3DXSHADER_DEBUG;
    //
    // Vertex shaders can be debugged with either the REF device or a device
    // created for software vertex processing (i.e., the IDirect3DDevice9
    // object must be created with the D3DCREATE_SOFTWARE_VERTEXPROCESSING
    // behavior). Pixel shaders can be debugged only using the REF device.
    //
    // To enable vertex shader debugging add the following flag to the
    // dwShaderFlags variable:
    //     dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    //
    // To enable pixel shader debugging add the following flag to the
    // dwShaderFlags variable:
    //     dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;

    HRESULT hr = D3DXCreateEffectFromFile(g_pDevice, pszFilename, 0, 0,
                    dwShaderFlags, 0, &pEffect, &pCompilationErrors);

    if (FAILED(hr))
    {
        if (pCompilationErrors)
        {
            std::string compilationErrors(static_cast<const char *>(
                            pCompilationErrors->GetBufferPointer()));

            pCompilationErrors->Release();
            throw std::runtime_error(compilationErrors);
        }
    }

    if (pCompilationErrors)
        pCompilationErrors->Release();

    return pEffect != 0;
}

void Log(const char *pszMessage)
{
    MessageBox(0, pszMessage, "Error", MB_ICONSTOP);
}

bool MSAAModeSupported(D3DMULTISAMPLE_TYPE type, D3DFORMAT backBufferFmt,
                       D3DFORMAT depthStencilFmt, BOOL windowed,
                       DWORD &qualityLevels)
{
    DWORD backBufferQualityLevels = 0;
    DWORD depthStencilQualityLevels = 0;

    HRESULT hr = g_pDirect3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL, backBufferFmt, windowed, type,
                    &backBufferQualityLevels);

    if (SUCCEEDED(hr))
    {
        hr = g_pDirect3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL, depthStencilFmt, windowed, type,
                &depthStencilQualityLevels);

        if (SUCCEEDED(hr))
        {
            if (backBufferQualityLevels == depthStencilQualityLevels)
            {
                // The valid range is between zero and one less than the level
                // returned by IDirect3D9::CheckDeviceMultiSampleType().

                if (backBufferQualityLevels > 0)
                    qualityLevels = backBufferQualityLevels - 1;
                else
                    qualityLevels = backBufferQualityLevels;

                return true;
            }
        }
    }

    return false;
}

void ProcessUserInput()
{
    Keyboard &keyboard = Keyboard::instance();

    if (keyboard.keyPressed(Keyboard::KEY_ESCAPE))
        PostMessage(g_hWnd, WM_CLOSE, 0, 0);

    if (keyboard.keyDown(Keyboard::KEY_LALT) || keyboard.keyDown(Keyboard::KEY_RALT))
    {
        if (keyboard.keyPressed(Keyboard::KEY_ENTER))
            ToggleFullScreen();
    }

    if (keyboard.keyPressed(Keyboard::KEY_H))
        g_displayHelp = !g_displayHelp;

    if (keyboard.keyPressed(Keyboard::KEY_T))
        g_disableColorMapTexture = !g_disableColorMapTexture;
}

void RenderBall()
{
    UINT totalPasses;
    D3DXHANDLE hTechnique;
    const D3DXMATRIX &worldMatrix = g_ball.getWorldMatrix();
    const D3DXMATRIX &viewMatrix = g_camera.getViewMatrix();
    const D3DXMATRIX &projMatrix = g_camera.getProjectionMatrix();
    D3DXMATRIX worldViewProjMatrix = worldMatrix * viewMatrix * projMatrix;

    // Since the world matrix here is a rigid-body transformation (i.e., only
    // rotations and translations are used) we can simply use the world matrix
    // to transform the mesh's normals instead of computing the normal matrix.
    // The normal matrix is the transpose of the inverse of the world matrix.
    //
    // The reason why we don't have to compute the normal matrix is because:
    // (a) Rotations are usually orthogonal. This means that the inverse of an
    //     orthogonal rotation matrix is the same as taking the transpose of
    //     the orthogonal rotation matrix.
    // (b) Translations don't affect the direction of normals.

    g_pEffect->SetMatrix("worldMatrix", &worldMatrix);
    g_pEffect->SetMatrix("worldInverseTransposeMatrix", &worldMatrix);
    g_pEffect->SetMatrix("worldViewProjectionMatrix", &worldViewProjMatrix);
    
    g_pEffect->SetValue("cameraPos", &g_camera.getPosition(), sizeof(D3DXVECTOR3));
    g_pEffect->SetValue("globalAmbient", &g_globalAmbient, sizeof(g_globalAmbient));

    // A directional light that's shining directly at the ball as it moves.
    g_pEffect->SetValue("light.dir", g_camera.getViewDirection(), sizeof(D3DXVECTOR3));
    g_pEffect->SetValue("light.ambient", g_light.ambient, sizeof(g_light.ambient));
    g_pEffect->SetValue("light.diffuse", g_light.diffuse, sizeof(g_light.diffuse));
    g_pEffect->SetValue("light.specular", g_light.specular, sizeof(g_light.specular));

    g_pEffect->SetValue("material.ambient", g_material.ambient, sizeof(g_material.ambient));
    g_pEffect->SetValue("material.diffuse", g_material.diffuse, sizeof(g_material.diffuse));
    g_pEffect->SetValue("material.emissive", g_material.emissive, sizeof(g_material.emissive));
    g_pEffect->SetValue("material.specular", g_material.specular, sizeof(g_material.specular));
    g_pEffect->SetFloat("material.shininess", g_material.shininess);

    if (g_disableColorMapTexture)
        g_pEffect->SetTexture("colorMapTexture", g_pNullTexture);
    else
        g_pEffect->SetTexture("colorMapTexture", g_pBallTexture);

    hTechnique = g_pEffect->GetTechniqueByName("PerPixelDirectionalLighting");

    if (SUCCEEDED(g_pEffect->SetTechnique(hTechnique)))
    {
        if (SUCCEEDED(g_pEffect->Begin(&totalPasses, 0)))
        {
            for (UINT i = 0; i < totalPasses; ++i)
            {
                if (SUCCEEDED(g_pEffect->BeginPass(i)))
                {
                    g_pBallMesh->DrawSubset(0);
                    g_pEffect->EndPass();
                }
            }

            g_pEffect->End();
        }
    }
}

void RenderFloor()
{
    UINT totalPasses;
    D3DXHANDLE hTechnique;
    D3DXMATRIX identityMatrix;
    D3DXMATRIX viewProjMatrix;

    D3DXMatrixIdentity(&identityMatrix);
    viewProjMatrix = g_camera.getViewMatrix() * g_camera.getProjectionMatrix();

    g_pEffect->SetMatrix("worldMatrix", &identityMatrix);
    g_pEffect->SetMatrix("worldInverseTransposeMatrix", &identityMatrix);
    g_pEffect->SetMatrix("worldViewProjectionMatrix", &viewProjMatrix);
    
    g_pEffect->SetValue("cameraPos", &g_camera.getPosition(), sizeof(D3DXVECTOR3));
    g_pEffect->SetValue("globalAmbient", &g_globalAmbient, sizeof(g_globalAmbient));

    g_pEffect->SetValue("light.dir", g_light.dir, sizeof(g_light.dir));
    g_pEffect->SetValue("light.pos", g_light.pos, sizeof(g_light.pos));
    g_pEffect->SetValue("light.ambient", g_light.ambient, sizeof(g_light.ambient));
    g_pEffect->SetValue("light.diffuse", g_light.diffuse, sizeof(g_light.diffuse));
    g_pEffect->SetValue("light.specular", g_light.specular, sizeof(g_light.specular));
    g_pEffect->SetFloat("light.spotInnerCone", g_light.spotInnerCone);
    g_pEffect->SetFloat("light.spotOuterCone", g_light.spotOuterCone);
    g_pEffect->SetFloat("light.radius", g_light.radius);

    g_pEffect->SetValue("material.ambient", g_material.ambient, sizeof(g_material.ambient));
    g_pEffect->SetValue("material.diffuse", g_material.diffuse, sizeof(g_material.diffuse));
    g_pEffect->SetValue("material.emissive", g_material.emissive, sizeof(g_material.emissive));
    g_pEffect->SetValue("material.specular", g_material.specular, sizeof(g_material.specular));
    g_pEffect->SetFloat("material.shininess", g_material.shininess);

    if (g_disableColorMapTexture)
        g_pEffect->SetTexture("colorMapTexture", g_pNullTexture);
    else
        g_pEffect->SetTexture("colorMapTexture", g_pFloorTexture);

    hTechnique = g_pEffect->GetTechniqueByName("PerPixelSpotLighting");

    if (SUCCEEDED(g_pEffect->SetTechnique(hTechnique)))
    {
        g_pDevice->SetVertexDeclaration(g_pVertexDecl);
        g_pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex));

        if (SUCCEEDED(g_pEffect->Begin(&totalPasses, 0)))
        {
            for (UINT i = 0; i < totalPasses; ++i)
            {
                if (SUCCEEDED(g_pEffect->BeginPass(i)))
                {
                    g_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
                    g_pEffect->EndPass();
                }
            }

            g_pEffect->End();
        }
    }
}

void RenderFrame()
{
    g_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    if (FAILED(g_pDevice->BeginScene()))
        return;

    RenderBall();
    RenderFloor();
    RenderText();

    g_pDevice->EndScene();
    g_pDevice->Present(0, 0, 0, 0);
}

void RenderText()
{
    static RECT rcClient;

    std::ostringstream output;

    if (g_displayHelp)
    {
        output
            << "Press W or UP to roll the ball forwards" << std::endl
            << "Press S or DOWN to roll the ball backwards" << std::endl
            << "Press D or RIGHT to turn the ball to the right" << std::endl
            << "Press A or LEFT to turn the ball to the left" << std::endl
            << std::endl
            << "Press T to toggle color map textures on and off" << std::endl
            << "Press ALT and ENTER to toggle full screen" << std::endl
            << "Press ESC to exit" << std::endl
            << std::endl
            << "Press H to hide help";
    }
    else
    {
        output
            << "FPS: " << g_framesPerSecond << std::endl
            << "Multisample anti-aliasing: " << g_msaaSamples << "x" << std::endl
            << "Anisotropic filtering: " << g_maxAnisotrophy << "x" << std::endl
            << std::endl
            << "Press H to display help";
    }

    GetClientRect(g_hWnd, &rcClient);
    rcClient.left += 4;
    rcClient.top += 2;

    g_pFont->DrawText(0, output.str().c_str(), -1, &rcClient,
        DT_EXPANDTABS | DT_LEFT, D3DCOLOR_XRGB(255, 255, 0));
}

bool ResetDevice()
{
    if (FAILED(g_pEffect->OnLostDevice()))
        return false;

    if (FAILED(g_pFont->OnLostDevice()))
        return false;

    if (FAILED(g_pDevice->Reset(&g_params)))
        return false;

    if (FAILED(g_pFont->OnResetDevice()))
        return false;

    if (FAILED(g_pEffect->OnResetDevice()))
        return false;

    return true;
}

void SetProcessorAffinity()
{
    // Assign the current thread to one processor. This ensures that timing
    // code runs on only one processor, and will not suffer any ill effects
    // from power management.
    //
    // Based on the DXUTSetProcessorAffinity() function in the DXUT framework.

    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;
    HANDLE hCurrentProcess = GetCurrentProcess();

    if (!GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask))
        return;

    if (dwProcessAffinityMask)
    {
        // Find the lowest processor that our process is allowed to run against.

        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

        // Set this as the processor that our thread must always run against.
        // This must be a subset of the process affinity mask.

        HANDLE hCurrentThread = GetCurrentThread();

        if (hCurrentThread != INVALID_HANDLE_VALUE)
        {
            SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
            CloseHandle(hCurrentThread);
        }
    }

    CloseHandle(hCurrentProcess);
}

void ToggleFullScreen()
{
    static DWORD savedExStyle;
    static DWORD savedStyle;
    static RECT rcSaved;

    g_isFullScreen = !g_isFullScreen;

    if (g_isFullScreen)
    {
        // Moving to full screen mode.

        savedExStyle = GetWindowLong(g_hWnd, GWL_EXSTYLE);
        savedStyle = GetWindowLong(g_hWnd, GWL_STYLE);
        GetWindowRect(g_hWnd, &rcSaved);

        SetWindowLong(g_hWnd, GWL_EXSTYLE, 0);
        SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

        g_windowWidth = GetSystemMetrics(SM_CXSCREEN);
        g_windowHeight = GetSystemMetrics(SM_CYSCREEN);

        SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0,
            g_windowWidth, g_windowHeight, SWP_SHOWWINDOW);

        // Update presentation parameters.

        g_params.Windowed = FALSE;
        g_params.BackBufferWidth = g_windowWidth;
        g_params.BackBufferHeight = g_windowHeight;

        if (g_enableVerticalSync)
        {
            g_params.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
            g_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        }
        else
        {
            g_params.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_IMMEDIATE;
            g_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        }
    }
    else
    {
        // Moving back to windowed mode.

        SetWindowLong(g_hWnd, GWL_EXSTYLE, savedExStyle);
        SetWindowLong(g_hWnd, GWL_STYLE, savedStyle);
        SetWindowPos(g_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

        g_windowWidth = rcSaved.right - rcSaved.left;
        g_windowHeight = rcSaved.bottom - rcSaved.top;

        SetWindowPos(g_hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
            g_windowWidth, g_windowHeight, SWP_SHOWWINDOW);

        // Update presentation parameters.

        g_params.Windowed = TRUE;
        g_params.BackBufferWidth = g_windowWidth;
        g_params.BackBufferHeight = g_windowHeight;
        g_params.FullScreen_RefreshRateInHz = 0;

        if (g_enableVerticalSync)
            g_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
        else
            g_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    ResetDevice();

    g_camera.perspective(CAMERA_FOVX,
        static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight),
        CAMERA_ZNEAR, CAMERA_ZFAR);
}

void UpdateBall(float elapsedTimeSec)
{
    Keyboard &keyboard = Keyboard::instance();
    float pitch = 0.0f;
    float heading = 0.0f;
    float forwardSpeed = 0.0f;

    if (keyboard.keyDown(Keyboard::KEY_W) || keyboard.keyDown(Keyboard::KEY_UP))
    {
        forwardSpeed = BALL_FORWARD_SPEED;
        pitch = BALL_ROLLING_SPEED;
    }

    if (keyboard.keyDown(Keyboard::KEY_S) || keyboard.keyDown(Keyboard::KEY_DOWN))
    {
        forwardSpeed = -BALL_FORWARD_SPEED;
        pitch = -BALL_ROLLING_SPEED;
    }

    if (keyboard.keyDown(Keyboard::KEY_D) || keyboard.keyDown(Keyboard::KEY_RIGHT))
        heading = BALL_HEADING_SPEED;

    if (keyboard.keyDown(Keyboard::KEY_A) || keyboard.keyDown(Keyboard::KEY_LEFT))
        heading = -BALL_HEADING_SPEED;

    // Prevent the ball from rolling off the edge of the floor.
    forwardSpeed = ClipBallToFloor(g_ball, forwardSpeed, elapsedTimeSec);

    // First move the ball.
    g_ball.setVelocity(0.0f, 0.0f, forwardSpeed);
    g_ball.orient(heading, 0.0f, 0.0f);
    g_ball.rotate(0.0f, pitch, 0.0f);
    g_ball.update(elapsedTimeSec);

    // Then move the camera based on where the ball has moved to.
    // When the ball is moving backwards rotations are inverted to match
    // the direction of travel. Consequently the camera's rotation needs to be
    // inverted as well.

    g_camera.rotate((forwardSpeed >= 0.0f) ? heading : -heading, 0.0f);
    g_camera.lookAt(g_ball.getPosition());
    g_camera.update(elapsedTimeSec);
}

void UpdateFrame(float elapsedTimeSec)
{
    Keyboard::instance().update();
    ProcessUserInput();

    UpdateFrameRate(elapsedTimeSec);
    UpdateBall(elapsedTimeSec);
}

void UpdateFrameRate(float elapsedTimeSec)
{
    static float accumTimeSec = 0.0f;
    static int frames = 0;

    accumTimeSec += elapsedTimeSec;

    if (accumTimeSec > 1.0f)
    {
        g_framesPerSecond = frames;

        frames = 0;
        accumTimeSec = 0.0f;
    }
    else
    {
        ++frames;
    }
}