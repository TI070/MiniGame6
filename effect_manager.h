#pragma once
#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include <d3dx9.h>
#include "effect.h"

#define EFFECT_MAX (1024)

class effect_manager {
private:
	effect_manager();
	static effect* pEffect[EFFECT_MAX];
public:

	static void Initialize();
	static void Draw();
	static void Update();
	static void SetEffect(D3DXVECTOR3 set_pos, float size,int type);
	static void SetExplosionEffect(D3DXVECTOR3 set_pos, float size, int anime_frame);
	static void DestroyEffect(int index);
	static void AllDestroyEffect();
	static bool LifeCheck(int index);
};
#endif