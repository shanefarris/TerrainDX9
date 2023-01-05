#include "Camera.h"

SpectatorCamera::CameraType  gCameraType;
D3DXVECTOR3 gRight;
D3DXVECTOR3 gUp;
D3DXVECTOR3 gLook;
D3DXVECTOR3 gPos;

SpectatorCamera::SpectatorCamera()
{
	gCameraType = WALKING;

	gPos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	gUp    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	gLook  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

SpectatorCamera::SpectatorCamera(CameraType cameraType)
{
	gCameraType = cameraType;

	gPos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	gRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	gUp    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	gLook  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void SpectatorCamera::walk(float units)
{
	// move only on xz plane for land object
	if( gCameraType == WALKING )
		gPos += D3DXVECTOR3(gLook.x, 0.0f, gLook.z) * units;

	if( gCameraType == FLYING )
		gPos += gLook * units;
}

void SpectatorCamera::strafe(float units)
{
	// move only on xz plane for land object
	if( gCameraType == WALKING )
		gPos += D3DXVECTOR3(gRight.x, 0.0f, gRight.z) * units;

	if( gCameraType == FLYING )
		gPos += gRight * units;
}

void SpectatorCamera::fly(float units)
{
	// move only on y-axis for land object
	if( gCameraType == WALKING )
		gPos.y += units;

	if( gCameraType == FLYING )
		gPos += gUp * units;
}

void SpectatorCamera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &gRight,	angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&gUp,&gUp, &T);
	D3DXVec3TransformCoord(&gLook,&gLook, &T);
}

void SpectatorCamera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( gCameraType == WALKING )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( gCameraType == FLYING )
		D3DXMatrixRotationAxis(&T, &gUp, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&gRight,&gRight, &T);
	D3DXVec3TransformCoord(&gLook,&gLook, &T);
}

void SpectatorCamera::roll(float angle)
{
	// only roll for aircraft type
	if( gCameraType == WALKING )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &gLook,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&gRight,&gRight, &T);
		D3DXVec3TransformCoord(&gUp,&gUp, &T);
	}
}

D3DXMATRIX* SpectatorCamera::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	//************************************************
	D3DXVec3Normalize(&gLook, &gLook);
	D3DXVec3Cross(&gUp, &gLook, &gRight);
	D3DXVec3Normalize(&gUp, &gUp);
	D3DXVec3Cross(&gRight, &gUp, &gLook);
	D3DXVec3Normalize(&gRight, &gRight);
	//************************************************

	// Build the view matrix:
	float x = -D3DXVec3Dot(&gRight, &gPos);
	float y = -D3DXVec3Dot(&gUp, &gPos);
	float z = -D3DXVec3Dot(&gLook, &gPos);

	(*V)(0,0) = gRight.x; (*V)(0, 1) = gUp.x; (*V)(0, 2) = gLook.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = gRight.y; (*V)(1, 1) = gUp.y; (*V)(1, 2) = gLook.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = gRight.z; (*V)(2, 1) = gUp.z; (*V)(2, 2) = gLook.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;

	return V;
}

void SpectatorCamera::setCameraType(CameraType cameraType)
{
	gCameraType = cameraType;
}
