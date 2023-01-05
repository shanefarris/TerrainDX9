#ifndef __CameraH__
#define __CameraH__

#include <d3dx9.h>

class SpectatorCamera
{
public:
	enum CameraType { WALKING, FLYING };

	//void (*PCameraPos)(float);
	SpectatorCamera();
	SpectatorCamera(CameraType cameraType);
	~SpectatorCamera(){}

	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	
	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	D3DXMATRIX* getViewMatrix(D3DXMATRIX* V); 
	void setCameraType(CameraType cameraType); 
	void getPosition(D3DXVECTOR3* pos){*pos = gPos;}
	void setPosition(D3DXVECTOR3* pos){gPos = *pos;}

	void getRight(D3DXVECTOR3* right){*right = gRight;}
	void getUp(D3DXVECTOR3* up){*up = gUp;}
	void getLook(D3DXVECTOR3* look){*look = gLook;}
public:
	CameraType  gCameraType;
	D3DXVECTOR3 gRight;
	D3DXVECTOR3 gUp;
	D3DXVECTOR3 gLook;
	D3DXVECTOR3 gPos;
};
#endif // __cameraH_
