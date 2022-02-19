#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include <d3d9.h>
#include <d3dx9.h> 

typedef struct CircleCollision_tag 
{
	D3DXVECTOR2 center;
	float radius;
}CircleCollision;

bool Collision_CircleAndCircleHit(const CircleCollision* pA,const CircleCollision* pB);

#endif