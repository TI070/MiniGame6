#include "effect.h"
#include "texture.h"
#include "billboard.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "key_manager.h"
#include "writestring.h"
#include "effect_manager.h"
#include "sound.h"
#include <stdio.h>

#define ACCELEFFECT_LIFE_MAX (30)
#define EXPLOSION_LIFE_MAX (30)

effect::effect() {



}

effect::~effect() {



}

effect::effect(D3DXVECTOR3 set_pos, float set_size) {

	pos = set_pos;
	size = set_size;

}

void effect::Initialize() {


}

void effect::Draw() {

	BillBorad_Draw(EffectTexId, pos.x, pos.y, pos.z, size, size);
}

void effect::Update() {

	life--;
}




accel_effect::accel_effect(D3DXVECTOR3 set_pos, float set_size,int chara_num) {

	EffectTexId = Texture_SetTextureLoadFile("asset/effect/effect_0.jpg");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	size = set_size * 2;

	life = ACCELEFFECT_LIFE_MAX;
	c_num = chara_num;
}

void accel_effect::Initialize() {


}

void accel_effect::Draw() {

	//加算合成の設定
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//float a = 1.0f - ((1.0f / EFFECT_AGEMAX) * age);
	float a = 1.0f - ((1.0f / ACCELEFFECT_LIFE_MAX) * life);

	D3DXCOLOR color;
	if(c_num == 0) color = D3DXCOLOR(0, 0.0f, 1.0f, (1.0f - a));
	else if(c_num == 1) color = D3DXCOLOR(1.0f, 0.0, 0, (1.0f - a));
	else if (c_num == 2) color = D3DXCOLOR(0.0f, 1.0, 0, (1.0f - a));
	else if (c_num == 3) color = D3DXCOLOR(1.0f, 1.0, 0, (1.0f - a));


	BillBoard_SetColor(color);

	//Zバッファへの書き込み,Zバッファの参照をしない
    pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	BillBorad_Draw(EffectTexId, pos.x, pos.y + 0.5f, pos.z, size, size);

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	BillBoard_SetColorDefault();

	//元の設定に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void accel_effect::Update() {

	life--;
}




explosion_effect::explosion_effect(D3DXVECTOR3 set_pos, float set_size, int set_aframe) {

	EffectTexId = Texture_SetTextureLoadFile("asset/effect/exp.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	size = set_size;

	life = EXPLOSION_LIFE_MAX;
	anime_frame = 0;// set_aframe;

	//SE
	PlaySound(SOUND_LABEL_SE_EXPLOSION);


}

void explosion_effect::Initialize() {


}

void explosion_effect::Draw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	//Zバッファへの書き込み,Zバッファの参照をしない
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	BillBorad_Draw(EffectTexId, pos.x, pos.y, pos.z, size, size,(((anime_frame) / 4) % 4) * 160, ((anime_frame) / 16) * 160,160,160);

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


}

void explosion_effect::Update() {

	life--;
	//if(anime_frame < EXPLOSION_LIFE_MAX)effect_manager::SetExplosionEffect(pos, size, anime_frame + 1);
	anime_frame++;

}




