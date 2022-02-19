#pragma once
#ifndef CUBE_H
#define CUBE_H

#include <d3d9.h>

typedef struct Vertex3D_tag {
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;//法線ベクトルの長さは必ず1
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;
		
}Vertex3D;


void Cube_Initialize();
void Cube_Finalize();
void Cube_Draw(const D3DXMATRIX* pMtxWorld);
void Cube_Update();


#endif CUBE_H
