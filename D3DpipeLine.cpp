#include "D3DpipeLine.h"
#include "MeshOperations.h"
#include "terrain.h"

//using namespace MeshOperations;

//BasicMeshX basicMeshX;			
//basicMeshX = load_xfile(Device);
//Terrain* basicTerrain = 0;

RECT rect = {0, 0, 600, 600};


void D3DpipeLine::StartPipe(D3DXMATRIX* CameraMatrix)
{
	//****************Terrain***********************
	//if(!setTerrain)
	//{
	//	basicTerrain = new Terrain(Device, 128, 128, 10);
	//	setTerrain = true;
	//}
	//**********************************************
	
	RenderSene(CameraMatrix);
}

void D3DpipeLine::RenderSene(D3DXMATRIX* CameraMatrix)
{
	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	Device->BeginScene();
	
	//****************Terrain***********************
	//D3DXMATRIX I;
	//D3DXMatrixIdentity(&I);
	//Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//basicTerrain->render(&I);
	//***************Camera*************************
	int i = g_Font->DrawText(NULL, FPS, sizeof(FPS), &rect, DT_NOCLIP, 0xff000000);
	Device->SetTransform(D3DTS_VIEW, CameraMatrix);
	Device->EndScene();

	Device->Present(0, 0, 0, 0);
}