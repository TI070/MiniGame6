#pragma once
#ifndef SHADOW_H
#define SHADOW_H

#include <d3dx9.h>

class shadow{
private:
	shadow();

	static int ShadowTexId;

public:

	static void Initialize();
	static void Draw(D3DXVECTOR3 pos);

};
#endif