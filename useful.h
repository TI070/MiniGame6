#pragma once
#ifndef USEFUL_H
#define USEFUL_H

#include <d3dx9.h>

#define PI (3.141592)

//ベクトルを指定した角度(度数法)で回転させる
D3DXVECTOR2 VectorRotate(D3DXVECTOR2 vec,int angle);

//１つ目の座標から２つ目の座標へのベクトルを算出
D3DXVECTOR2 PostoPosVector_Normalize(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);

//ベクトルから角度を算出
int GetVectorAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);


#endif