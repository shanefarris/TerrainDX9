#ifndef __terrainH__
#define __terrainH__

#include "d3dUtility.h"
#include "D3DpipeLine.h"

class Terrain
{
public:
	Terrain(void);
	Terrain(
		IDirect3DDevice9* device, 
		int VertsPerRow,  
		int VertsPerCol, 
		int cellSpacing);   

	~Terrain(void);
	virtual bool render(D3DXMATRIX* world);

private:
	IDirect3DDevice9*       device;
	IDirect3DTexture9*      m_tex;
	IDirect3DVertexBuffer9* m_vb;
	IDirect3DIndexBuffer9*  m_ib;

	int iVertsPerRow;	int iVertsPerCol;
	int iCellSpacing;
	int iCellsPerRow;	int iCellsPerCol;
	int iWidth;			int iDepth;
	int iVertices;		int iTriangles;

	bool WireFrame;
	
	bool  computeVertices();
	bool  computeIndices();
	bool  lightTerrain(D3DXVECTOR3* directionToLight);
	float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);
	inline float RangedRandom( float f1, float f2 )
		{return (f1+(f2-f1)*((float)rand())/((float)RAND_MAX ));}

	struct TerrainVertex
	{
		TerrainVertex(){}
		TerrainVertex(float x, float y, float z, float u, float v)
		{
			_x = x; _y = y; _z = z; _u = u; _v = v;
		}
		float _x, _y, _z;
		float _u, _v;

		static const DWORD FVF;
	};
public:
	TerrainVertex* v;
};

#endif