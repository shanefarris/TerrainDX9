#ifndef __D3DpipeLineH__
#define __D3DpipeLineH__

#include <string>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class D3DpipeLine
{
	//*************************************************************
	// Public Methods
	//*************************************************************
public:
	D3DpipeLine(void)
	{
		setTerrain = false;
	}
	void StartPipe(D3DXMATRIX* V);
	static float Lerp(float a, float b, float t){return a - (a*t) + (b*t);}
	//*************************************************************
	// Private Methods
	//*************************************************************
private:
	void RenderSene(D3DXMATRIX* CameraMatrix);
	//*************************************************************
	// Public Variables
	//*************************************************************
public:
	LPDIRECT3D9				g_pD3D;
	LPDIRECT3DDEVICE9		Device;
	LPDIRECTINPUT8			g_lpDID;

	LPDIRECTINPUTDEVICE8	g_Keyboard;
	LPDIRECTINPUTDEVICE8	g_Mouse;
	LPDIRECT3DSURFACE9		g_MouseCur;
	
	ID3DXFont*				g_Font;

	char FPS[8];
	//*************************************************************
	// Private Variables
	//*************************************************************
private:
	LPDIRECT3DVERTEXBUFFER9	gp_VB;
	LPDIRECT3DTEXTURE9*		g_pTexture;				// Our texture
	LPD3DXMESH				g_pMesh;				// Our mesh object in sysmem
	D3DMATERIAL9*			g_pMeshMaterials; 		// Materials for our mesh
	DWORD					g_dwNumMaterials; 		// Number of mesh materials
	LPD3DXFONT				g_pFont;			 	// Font device
	
	bool setTerrain;								// Is the terrain set

};

//*************************************************************
// Structures
//*************************************************************
struct BASIC_MESH
{
	BASIC_MESH()
	{
		iVertices = NULL;
		iIndices  = NULL;
		m_tex     = NULL;
		m_vb      = NULL;
		m_ib      = NULL;
	};

	std::string sName;
	int iID;								// Index ID (sort ID)
	int iVertices;							// number of vertices
	int iIndices;							// number of indices

	IDirect3DTexture9*      m_tex;
	IDirect3DVertexBuffer9* m_vb;
	IDirect3DIndexBuffer9*  m_ib;
	DWORD					color;
};

struct BASIC_TERRAIN
{
	BASIC_TERRAIN()
	{
		iVertices = NULL;
		iIndices  = NULL;
		m_vb      = NULL;
		m_ib      = NULL;
	};

	int iID;								// Index ID (sort ID)
	int iVertices;							// number of vertices
	int iIndices;							// number of indices

	IDirect3DVertexBuffer9* m_vb;
	IDirect3DIndexBuffer9*  m_ib;
};	
#endif