#include "effect_manager.h"

#define BULLET_SIZE (0.25f)

effect* effect_manager::pEffect[EFFECT_MAX];

void effect_manager::Initialize() {

	for (int i = 0; i < EFFECT_MAX; i++) {

		pEffect[i] = NULL;

	}
}

void effect_manager::Draw() {

	for (int i = 0; i < EFFECT_MAX; i++) {

		if (pEffect[i] == NULL) continue;

		pEffect[i]->Draw();

	}
}

void effect_manager::Update() {

	for (int i = 0; i < EFFECT_MAX; i++) {

		if (pEffect[i] == NULL) continue;

		pEffect[i]->Update();

		if (!LifeCheck(i)) DestroyEffect(i);
	}
}


void effect_manager::SetEffect(D3DXVECTOR3 set_pos, float size, int type) {

	for (int i = 0; i < EFFECT_MAX; i++) {

		if (pEffect[i] != NULL) continue;

		pEffect[i] = new accel_effect(set_pos, size,type);

		break;
	}

}

void effect_manager::SetExplosionEffect(D3DXVECTOR3 set_pos, float size,int anime_frame) {

	for (int i = 0; i < EFFECT_MAX; i++) {

		if (pEffect[i] != NULL) continue;
		
		pEffect[i] = new explosion_effect(set_pos, size,anime_frame);

		break;
	}

}


void effect_manager::DestroyEffect(int index) {

	delete pEffect[index];
	pEffect[index] = NULL;
}

void effect_manager::AllDestroyEffect() {

	for (int i = 0; i < EFFECT_MAX; i++) {
	
		DestroyEffect(i);
	}


}


//life‚ª0ˆÈ‰º‚È‚çfalse‚ð•Ô‚·
bool effect_manager::LifeCheck(int index) {

	if (pEffect[index]->life <= 0) return false;
	return true;
}