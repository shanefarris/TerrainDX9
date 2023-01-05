#include "MeshOperations.h"

LPDIRECT3DTEXTURE9*		m_pTexture;				// Our texture
LPD3DXMESH				m_pMesh;				// Our mesh object in sysmem
D3DMATERIAL9*			m_pMeshMaterials; 		// Materials for our mesh
DWORD					m_dwNumMaterials; 		// Number of mesh materials

// Xfile loading and setup
BasicMeshX MeshOperations::load_xfile(LPDIRECT3DDEVICE9 Device)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;						//Stores all materials and textures
	ID3DXBuffer* adjBuffer  = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	
    if( FAILED( D3DXLoadMeshFromX( "bigship1.x",			// Load the mesh from the specified file
									D3DXMESH_SYSTEMMEM, 
									Device, 
									&adjBuffer,			//specify the three neighbors for each face in the mesh
                                    &pD3DXMtrlBuffer,	//Stores all materials and textures
								    NULL, 
								    &m_dwNumMaterials, 
                                    &m_pMesh ) ) )
	{
		MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return BasicMeshX(NULL, NULL, NULL);
	}

	// Extract the materials, and load textures.
	if(  m_dwNumMaterials != 0 )
	{
		//getting the pointer to the material buffer
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
		m_pTexture  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
		
		for( DWORD i=0; i< m_dwNumMaterials; i++ )
		{
			m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;				// Copy the material
			m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

			m_pTexture[i] = NULL;
			if( d3dxMaterials[i].pTextureFilename != 0 )
			{
				if( FAILED( D3DXCreateTextureFromFile(Device, 
													d3dxMaterials[i].pTextureFilename, 
													&m_pTexture[i] ) ) )
					{
						MessageBox(0, "D3DXCreateTextureFromFile() - FAILED", 0, 0);
						return BasicMeshX(NULL, NULL, NULL);;
					}
			}
		}
		pD3DXMtrlBuffer->Release();								// Done with the material buffer
	}
	return BasicMeshX(m_pTexture, m_pMeshMaterials, m_dwNumMaterials);
}


