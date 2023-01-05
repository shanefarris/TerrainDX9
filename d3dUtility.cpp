#include "d3dUtility.h"
#include "D3DpipeLine.h"
#include "Camera.h"

DWORD dFrameCnt			= 0;		// how many frames
float fTimeElapsed		= 0;		// checks for 1 second mark
float fFPS				= 0;
float fTimeDelta		= 0;		// time between loops

MSG msg;
BYTE	buff[256];									// keyboard buffer 

SpectatorCamera	Camera(SpectatorCamera::WALKING);
D3DpipeLine		Pipe;
Utility*		d3dUtil;

bool D3D::Prog_Init(HWND hwnd, HINSTANCE hInstance)
{
	HRESULT hr = 0;

	Pipe.g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(!Pipe.g_pD3D)
	{
		MessageBox(0, "Could not Create D3D COM", 0, 0);
		return false;
	}
 
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = iWindowWidth;
	d3dpp.BackBufferHeight           = iWindowHeigth;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = Pipe.g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,					// primary adapter
		D3DDEVTYPE_REF,						// device type
		hwnd,								// window associated with device
		D3DCREATE_HARDWARE_VERTEXPROCESSING, // vertex processing
	    &d3dpp,								// present parameters
		&Pipe.Device);						// return created device

	if(FAILED(hr))
	{
		Release(Pipe.Device); 
		MessageBox(0, "Could not Create D3D Device", 0, 0);
		return false;
	}
	
	// ************Setup projection************
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)D3D::iWindowWidth / (float)D3D::iWindowHeigth,
		1.0f,
		1000.0f);
	Pipe.Device->SetTransform(D3DTS_PROJECTION, &proj);

	// ***********Setup Input device************
	hr = DirectInput8Create(hInstance, 
							DIRECTINPUT_VERSION, 
							IID_IDirectInput8, 
							(void**)&Pipe.g_lpDID, 
							NULL);
	if(FAILED(hr))
	{
		Release(Pipe.Device); 
		MessageBox(0, "Could not Create D3D Input Device", 0, 0);
		return false;
	}
	Pipe.g_lpDID->CreateDevice(GUID_SysKeyboard, &Pipe.g_Keyboard, NULL);
	Pipe.g_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Pipe.g_Keyboard->SetDataFormat(&c_dfDIKeyboard);

	Pipe.g_lpDID->CreateDevice(GUID_SysKeyboard, &Pipe.g_Mouse, NULL);
	Pipe.g_Mouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	Pipe.g_Mouse->SetDataFormat(&c_dfDIMouse);
	Pipe.g_Mouse->Acquire();
	// *****************************************
	d3dUtil->FontInit(Pipe.Device, &Pipe.g_Font);
	//*********************************************
	return true;
}

void D3D::Prog_Loop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
        {	
			d3dUtil->ProgLoopTime();
			d3dUtil->ProgLoopKeyInput();
			//******************************************************************
			DIMOUSESTATE2 dims2;
			HRESULT hr = 0;
			hr = Pipe.g_Mouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );
			//******************************************************************
			D3DXVECTOR3 pos;
			Camera.getPosition(&pos);
			pos.y = 10 + 5.0f;					// add height because we're standing up
			Camera.setPosition(&pos);

			D3DXMATRIX V;
			Pipe.StartPipe(Camera.getViewMatrix(&V));
        }
    }

}

void D3D::Prog_Cleanup()
{
	Pipe.g_pD3D->Release();
	Pipe.Device->Release();
	Pipe.g_lpDID->Release();
}

void Utility::ProgLoopTime(void)
{
	static float fTime = (float)timeGetTime();			// base time
	float fCurTime  = (float)timeGetTime();
	fTimeDelta = (fCurTime - fTime) * 0.001f;		// get the difference
	fTimeElapsed += fTimeDelta;
	dFrameCnt++;
	
	if (fTimeElapsed >= 1.0f)
	{
		fFPS = (float)dFrameCnt / fTimeElapsed;
		sprintf(Pipe.FPS, "%f", fFPS);
		dFrameCnt = 0;
		fTimeElapsed = 0.0f;
	}
}

void Utility::ProgLoopKeyInput(void)
{
	Pipe.g_Keyboard->Acquire();
	ZeroMemory(buff, sizeof(buff));
    Pipe.g_Keyboard->GetDeviceState(sizeof(buff), buff);

	//**************************Input**********************************
	if(KEYDOWN(buff, DIK_W))
		Camera.walk(4.0f * fTimeDelta);
		
	if(KEYDOWN(buff, DIK_S))
		Camera.walk(-4.0f * fTimeDelta);

	if(KEYDOWN(buff, DIK_A))
		Camera.strafe(4.0f * fTimeDelta);

	if(KEYDOWN(buff, DIK_D))
		Camera.strafe(-4.0f * fTimeDelta);

	if(KEYDOWN(buff, DIK_R))
		Camera.fly(4.0f * fTimeDelta);

	if(KEYDOWN(buff, DIK_F))
		Camera.walk(-4.0f * fTimeDelta);

	if(KEYDOWN(buff, DIK_UP))
		Camera.pitch(0.1f * fTimeDelta);

	if(KEYDOWN(buff, DIK_DOWN))
		Camera.pitch(-0.1f * fTimeDelta);

	if(KEYDOWN(buff, DIK_LEFT))
		Camera.yaw(-0.1f * fTimeDelta);
			
	if(KEYDOWN(buff, DIK_RIGHT))
		Camera.yaw(0.1f * fTimeDelta);

	if(KEYDOWN(buff, DIK_N))
		Camera.roll(0.1f * fTimeDelta);

	if(KEYDOWN(buff, DIK_M))
		Camera.roll(-0.1f * fTimeDelta);

	if(KEYDOWN(buff, DIK_ESCAPE))
		PostQuitMessage(0);
}
