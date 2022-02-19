#include "useful.h"
#include <stdio.h>

D3DXVECTOR2 VectorRotate(D3DXVECTOR2 vec,int angle) {

	D3DXVECTOR2 r_vec;
	float f_angle;

	f_angle = angle * PI / 180.0f;


	r_vec.x = vec.x * cosf(f_angle) - vec.y * sinf(f_angle);
	r_vec.y = vec.x * sinf(f_angle) + vec.y * cosf(f_angle);

	return r_vec;

}


D3DXVECTOR2 PostoPosVector_Normalize(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2) {


	float length;
	D3DXVECTOR2 r_pos;

	r_pos = pos2 - pos1;
	length = sqrt(r_pos.x * r_pos.x + r_pos.y * r_pos.y);


	r_pos.x /= length;
	r_pos.y /= length;

	return r_pos;

}

int GetVectorAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2) {

	float res;

	res = (pos1.x * pos2.x + pos1.y * pos2.y)/(sqrt(pow(pos1.x,2) + pow(pos1.y, 2)) * sqrt(pow(pos2.x, 2) + pow(pos2.y, 2)));

	res = acosf(res);

	//ìxêîñ@Ç…íºÇ∑
	res = res * 180 / PI;


	return res;
}

