#pragma once
#ifndef CAMERA_H
#define CAMERA_H

enum CAMERA_MODE {
	FIXED_CAMERA,//å≈íËÉJÉÅÉâ
	FOLLOW_CAMERA//í«è]

};


void SetCamera();
void Camera_Initialize();
void Camera_Update(bool move_on);
void Camera_SetMatrixToDevice();

const D3DXVECTOR3* Camera_GetFrontVector();
const D3DXVECTOR3* Camera_GetRightVector();
const D3DXVECTOR3* Camera_GetPosition();
void Camera_SetPosition(D3DXVECTOR3 set_pos);
const D3DXVECTOR3* Camera_GetAt();
void Camera_SetAt(D3DXVECTOR3 set_at);
const D3DXMATRIX& Camera_GetViewMatrix();
bool SetUpViewport(int id);
void SetCameraMode(int mode);
int GetCameraMode();

void SetCameraVec();

#endif