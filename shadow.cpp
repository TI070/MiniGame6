
#include "shadow.h"
#include "texture.h"
#include "billboard.h"
#include "mydirect3d.h"

#define SHADOW_SIZE (1.0f)

int shadow::ShadowTexId;

#define FVF_SHADOW_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct ShadowVertex_tag {
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;

}ShadowVertex;

void shadow::Initialize() {

	ShadowTexId = Texture_SetTextureLoadFile("asset/effect000.jpg");
}

void shadow::Draw(D3DXVECTOR3 pos) {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	//Œ¸ŽZÝ’è
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//BillBorad_Draw(ShadowTexId,pos.x, 0.5f, pos.z,SHADOW_SIZE, SHADOW_SIZE);

	float width, height;
	width = SHADOW_SIZE;
	height = SHADOW_SIZE;

	static const ShadowVertex v[] = {
		{{-width / 2,0.0f, height / 2,},0xffffffff,{0.0f,0.0f} },
		{{ width / 2,0.0f, height / 2,},0xffffffff,{1.0f,0.0f} },
		{{-width / 2,0.0f,-height / 2,},0xffffffff,{0.0f,1.0f} },
		{{ width / 2,0.0f,-height / 2,},0xffffffff,{1.0f,1.0f} },

	};

	D3DXMATRIX mtxWorld, mtxT;

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, pos.x, 0, pos.z);

	mtxWorld = mtxT;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetFVF(FVF_SHADOW_VERTEX);
	pDevice->SetTexture(0, Texture_GetTexture(ShadowTexId));
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(ShadowVertex));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);














	//Œ¸ŽZÝ’è‚Ì‰ðœ
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}