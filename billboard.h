#pragma once
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <d3dx9.h>

void BillBoard_Initialize();
void BillBoard_Finalize();
void BillBoard_SetViewMatrix(const D3DXMATRIX& mtxView);
//void BillBoard_SetOffsetY(float offset_y);
void BillBorad_Draw(int textureId,float x,float y,float z, float height, float width);
void BillBorad_Draw(int textureId, float x, float y, float z, float height, float width, int tcx, int tcy, int tcw, int tch);
void BillBoard_SetColor(D3DCOLOR color);
void BillBoard_SetColorDefault();
void BillBoard_SetColor(int r, int g, int b, int a);
#endif