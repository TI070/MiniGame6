#pragma once
#ifndef USEFUL_H
#define USEFUL_H

#include <d3dx9.h>

#define PI (3.141592)

//�x�N�g�����w�肵���p�x(�x���@)�ŉ�]������
D3DXVECTOR2 VectorRotate(D3DXVECTOR2 vec,int angle);

//�P�ڂ̍��W����Q�ڂ̍��W�ւ̃x�N�g�����Z�o
D3DXVECTOR2 PostoPosVector_Normalize(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);

//�x�N�g������p�x���Z�o
int GetVectorAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);


#endif