#include "collision.h"

bool Collision_CircleAndCircleHit(const CircleCollision* pA, const CircleCollision* pB) {

	D3DXVECTOR2 vec = pA->center - pB->center;

	return D3DXVec2LengthSq(&(vec)) < (pA->radius + pB->radius) * (pA->radius + pB->radius);
}



