#include "d3dUtility.h"
#include "D3DpipeLine.h"
#include <d3dx9core.h>

int Utility::FontInit(LPDIRECT3DDEVICE9 m_Device, ID3DXFont** m_Font)
{
	HRESULT hr;
	D3DXFONT_DESC fd;

	memset(&fd, '0', sizeof(D3DXFONT_DESC));
	fd.Height         = 17;
	fd.Width          = 6;
	fd.Weight         = 300;
	fd.Italic         = false;   
	fd.CharSet        = DEFAULT_CHARSET;
	fd.Quality        = 0;           
	fd.PitchAndFamily = 0;           
	strcpy(fd.FaceName, "Times New Roman"); // Default font

	hr = D3DXCreateFontIndirect(m_Device, &fd, m_Font);
	if(FAILED(hr))
	{
		MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
		return -1;
	}
	return 0;
}