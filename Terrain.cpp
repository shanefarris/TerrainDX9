#include "terrain.h"

const DWORD Terrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

Terrain::Terrain(IDirect3DDevice9* Device,
				 int VertsPerRow,
				 int VertsPerCol,
				 int CellSpacing)
{
	device = Device;
	iVertsPerRow = VertsPerRow;
	iVertsPerCol = VertsPerCol;
	iCellSpacing = CellSpacing;

	iCellsPerRow = iVertsPerRow - 1;
	iCellsPerCol = iVertsPerCol - 1;

	iWidth = iCellsPerRow * iCellSpacing;
	iDepth = iCellsPerCol * iCellSpacing;

	iVertices  = iVertsPerRow * iVertsPerCol;
	iTriangles = iCellsPerRow * iCellsPerCol * 2;

	WireFrame = false;

	if( !computeVertices() )
	{
		MessageBox(0, "computeVertices - FAILED", 0, 0);
		PostQuitMessage(0);
	}

	if( !computeIndices() )
	{
		MessageBox(0, "computeIndices - FAILED", 0, 0);
		PostQuitMessage(0);
	}
}

bool Terrain::computeVertices()
{
	if(FAILED(device->CreateVertexBuffer(
		iVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&m_vb,
		0)))
		return false;

	//*********Texture Setup******************
	if(FAILED(D3DXCreateTexture(
		device,
		iCellsPerRow, iCellsPerCol,
		0,									// create a complete mipmap chain
		0,									// usage
		D3DFMT_X8R8G8B8,					// 32 bit XRGB format
		D3DPOOL_MANAGED, &m_tex)))
		return false;

	D3DSURFACE_DESC textureDesc; 
	m_tex->GetLevelDesc(0 /*level*/, &textureDesc);
	if(textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	m_tex->LockRect(0/*lock top surface*/, 
					&lockedRect, 
					0 /* lock entire tex*/, 
					0/*flags*/);
	DWORD* HeightColor = (DWORD*)lockedRect.pBits;			//points to m_tex rect region

	float uCoordIncrementSize = 1.0f / (float)iCellsPerRow;
	float vCoordIncrementSize = 1.0f / (float)iCellsPerCol;

	int startX = -iWidth / 2;
	int startZ =  iDepth / 2;
	int endX =  iWidth / 2;
	int endZ = -iDepth / 2;

	m_vb->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for(int z = startZ; z >= endZ; z -= iCellSpacing)
	{
		int j = 0;
		for(int x = startX; x <= endX; x += iCellSpacing)
		{
			D3DXCOLOR c;
			float fHeight = (float)RangedRandom(-10.0f, 50.0f) / 5.0f;
			int index = i * iVertsPerRow + j;
			v[index] = TerrainVertex(
				(float)x,
				(float)fHeight,
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);
			
			//This will need some work 9-17-04
			if((fHeight) < 5.0f) 		 c = D3D::BEACH_SAND;
			else if((fHeight) < 10.0f)	 c = D3D::LIGHT_YELLOW_GREEN;
			else if((fHeight) < 20.0f)	 c = D3D::PUREGREEN;
			else if((fHeight) < 30.0f)	 c = D3D::DARK_YELLOW_GREEN;
			else if((fHeight) < 50.0f)	 c = D3D::DARKBROWN;
			else	                     c = D3D::WHITE;
			HeightColor[i * lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
			//**********************************
			j++; // next column
		}
		i++; // next row
	}
	m_tex->UnlockRect(0);
	m_vb->Unlock();
	//******************End texture setup********
	
	D3DXVECTOR3 lightDirection(0.0f, 1.0f, 0.0f);
	if(!lightTerrain(&lightDirection))
	{
		MessageBox(0, "Setting up light in computeVertices", 0, 0);
		return false;
	}
	
	return true;
}

bool Terrain::computeIndices()
{
	if(FAILED(device->CreateIndexBuffer(
		iTriangles * 3 * sizeof(WORD),// 3 indices per triangle
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_ib,
		0)))
		return false;

	WORD* indices = 0;
	m_ib->Lock(0, 0, (void**)&indices, 0);

	// loop through and compute the triangles of each quad (square)
	// each quad consists of 6 indices or 2 triangles
	int baseIndex = 0;
	for(int i = 0; i < iCellsPerCol; i++)
	{
		for(int j = 0; j < iCellsPerRow; j++)
		{
			indices[baseIndex]     =   i   * iVertsPerRow + j;		//0, 1
			indices[baseIndex + 1] =   i   * iVertsPerRow + j + 1;	//1, 2
			indices[baseIndex + 2] = (i+1) * iVertsPerRow + j;		//64,65

			indices[baseIndex + 3] = (i+1) * iVertsPerRow + j;		//64,65
			indices[baseIndex + 4] =   i   * iVertsPerRow + j + 1;	//1, 2
			indices[baseIndex + 5] = (i+1) * iVertsPerRow + j + 1;	//65,66 

			baseIndex += 6;
		}
	}
	m_ib->Unlock();
	return true;
}

bool Terrain::lightTerrain(D3DXVECTOR3* directionToLight)
{
	D3DSURFACE_DESC textureDesc; 
	m_tex->GetLevelDesc(0, &textureDesc);

	if(textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;
		
	D3DLOCKED_RECT lockedRect;
	m_tex->LockRect(0, &lockedRect, 0, 0);

	DWORD* HeightColor = (DWORD*)lockedRect.pBits;
	for(unsigned int i = 0; i < textureDesc.Height; i++)
	{
		for(unsigned int j = 0; j < textureDesc.Width; j++)
		{
			// index: iterate through the texture. Pitch is
			// give in DWORD as divid by 4
			int index = i * lockedRect.Pitch / 4 + j;
			D3DXCOLOR c(HeightColor[index]);			// get current color of quad
			c *= computeShade(i, j, directionToLight);	// shade current quad
			HeightColor[index] = (D3DCOLOR)c;			// save shaded color
		}
	}
	m_tex->UnlockRect(0);
	return true;
}

float Terrain::computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight)
{
	// get heights of three vertices on the quad
	float heightA = v[cellRow * iVertsPerRow + cellCol]._y;			//  A--B
	float heightB = v[cellRow * iVertsPerRow + (cellCol + 1)]._y;	//  | /
	float heightC = v[(cellRow + 1) * iVertsPerRow + cellCol]._y;	//  C

	// build two vectors on the quad
	D3DXVECTOR3 u((float)iCellSpacing, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, (float)-iCellSpacing);

	// normalize the vertors
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, directionToLight);
	if(cosine < 0.0f)
		cosine = 0.0f;

	return cosine;
}

bool Terrain::render(D3DXMATRIX* world)
{
	if(device)
	{
		device->SetTransform(D3DTS_WORLD, world);
		device->SetStreamSource(0, m_vb, 0, sizeof(TerrainVertex));
		device->SetFVF(TerrainVertex::FVF);
		device->SetIndices(m_ib);
		device->SetTexture(0, m_tex);
		device->SetRenderState(D3DRS_LIGHTING, false);

		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			iVertices,
			0,
			iTriangles);

		device->SetRenderState(D3DRS_LIGHTING, true);
	}
	return true;
}

Terrain::~Terrain()
{
	D3D::Release(device);
	D3D::Release(m_tex);
	D3D::Release(m_vb);
	D3D::Release(m_ib);
}