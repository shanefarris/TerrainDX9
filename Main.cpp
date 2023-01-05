#include "d3dUtility.h"
#define WIN32_LEAN_AND_MEAN  
#include <Windows.h>

HWND hwnd;

LRESULT WINAPI MsgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
    {
		case WM_DESTROY:
			D3D::Prog_Cleanup();
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            ValidateRect(hwnd, NULL);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)MsgProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "Shell";

	if(!RegisterClass(&wc)) 
	{
		MessageBox(0, "Registering Window Failed.", 0, 0);
		return false;
	}

	hwnd = CreateWindow("Shell", "Shell", 
		WS_EX_TOPMOST, 0, 0, D3D::iWindowWidth, D3D::iWindowHeigth,
		0,0 , hInstance,0); 

	if(!hwnd)
	{
		MessageBox(0, "Could not CreateWindow", 0, 0);
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	if(!D3D::Prog_Init(hwnd, hInstance))
	{
		MessageBox(0, "Prog_Init() - FAILED", 0, 0);
		return 0;
	}

	D3D::Prog_Loop();
	return 0;
}
