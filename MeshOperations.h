#include <d3dx9.h>


struct BasicMeshX
{
	BasicMeshX(LPDIRECT3DTEXTURE9* Texture,
				D3DMATERIAL9* MeshMaterials,
				DWORD NumMaterials)
	{
		m_pTexture			= Texture;
		m_pMeshMaterials	= MeshMaterials;
		m_dwNumMaterials	= NumMaterials;
	}
	BasicMeshX(){}

	LPDIRECT3DTEXTURE9*		m_pTexture;				// Our texture
	D3DMATERIAL9*			m_pMeshMaterials; 		// Materials for our mesh
	DWORD					m_dwNumMaterials; 		// Number of mesh materials
};

namespace MeshOperations
{
	BasicMeshX load_xfile(LPDIRECT3DDEVICE9 Device);
	
	

}
