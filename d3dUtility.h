#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <d3dx9.h>
#include <d3d9.h>
#include <string>
#include <time.h>

class Utility
{
public:
	void ProgLoopTime(void);
	void ProgLoopKeyInput(void);
	int FontInit(LPDIRECT3DDEVICE9 Device, ID3DXFont** g_Font);
};

/**********************
/Master namespace
/**********************/
namespace D3D
{
	const int iWindowWidth	= 600;
	const int iWindowHeigth = 480;

	bool Prog_Init(HWND, HINSTANCE);
	void Prog_Loop(void);
	void Prog_Cleanup(void);

	//=================================================
	//deletes coms
	template<class T> void Release(T t)
	{
		if( t )
		{
			//t->Release();
			t = NULL;
		}
	}
	//deletes variables	
	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}
	//=================================================

	//******************TEMP***************************
	//
	// Colors
	//
	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

	const D3DXCOLOR BEACH_SAND( D3DCOLOR_XRGB(255, 249, 157) );
	const D3DXCOLOR DESERT_SAND( D3DCOLOR_XRGB(250, 205, 135) );

	const D3DXCOLOR LIGHTGREEN( D3DCOLOR_XRGB( 60, 184, 120) );
	const D3DXCOLOR  PUREGREEN( D3DCOLOR_XRGB(  0, 166,  81) );
	const D3DXCOLOR  DARKGREEN( D3DCOLOR_XRGB(  0, 114,  54) );

	const D3DXCOLOR LIGHT_YELLOW_GREEN( D3DCOLOR_XRGB(124, 197, 118) );
	const D3DXCOLOR  PURE_YELLOW_GREEN( D3DCOLOR_XRGB( 57, 181,  74) );
	const D3DXCOLOR  DARK_YELLOW_GREEN( D3DCOLOR_XRGB( 25, 123,  48) );

	const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
	const D3DXCOLOR DARKBROWN( D3DCOLOR_XRGB(115, 100,  87));

	//*******************************************************
}
#endif