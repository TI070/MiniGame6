#include <d3dx9.h>
#include <stdio.h>
#include "cube.h"
#include "mydirect3d.h"
#include "camera.h"
#include "texture.h"
#include "game.h"

#define FVF_BILLBOARD_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

static float g_angle = 0.0f;
static float g_value = 0.0f;

static int g_TexId = TEXTURE_INVALID_ID;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_mtxView;

static D3DCOLOR Color = 0xffffffff;

typedef struct BillboardVertex_tag {
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;

}BillboardVertex;

//頂点データ配列

void BillBoard_Initialize() {

	D3DXMatrixIdentity(&g_mtxView);

}

void BillBoard_Finalize() {


}

//カラー設定
void BillBoard_SetColor(D3DCOLOR color) {

	Color = color;
}

void BillBoard_SetColor(int r, int g, int b, int a) {

	Color = (a << 24) + (r << 16) + (g << 8) + (b);
}

//カラー初期化
void BillBoard_SetColorDefault() {

	Color = 0xffffffff;
}

//void BillBoard_SetOffsetY(float offset_y);

void BillBorad_Draw(int textureId, float x, float y, float z,float height, float width) {

	BillboardVertex v[] = {
		{{-width / 2, height / 2,0.0f},Color,{0.0f,0.0f} },
		{{ width / 2, height / 2,0.0f},Color,{1.0f,0.0f} },
		{{-width / 2,-height / 2,0.0f},Color,{0.0f,1.0f} },
		{{ width / 2,-height / 2,0.0f},Color,{1.0f,1.0f} },

	};

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	D3DXMATRIX mtxWorld,mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;
	
	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, x, y, z);

	mtxWorld = mtxInvView * mtxT;
	//mtxWorld = mtxInvView * mtxS * mtxRZ * mtxT;
	
	
	//αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF,0.8f);//閾値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);//判定方法

	//Zテスト,Zバッファの有効、無効の切り替え
	//Zバッファへの書き込み,Zバッファの参照をしない
	//pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	//Zバッファへの書き込みをしない
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);
	pDevice->SetFVF(FVF_BILLBOARD_VERTEX);
	pDevice->SetTexture(0,Texture_GetTexture(textureId));
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,v,sizeof(BillboardVertex));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void BillBorad_Draw(int textureId, float x, float y, float z, float width, float height,int tcx, int tcy, int tcw, int tch) {
	
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	//pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);


	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	
	BillboardVertex v[] = {
		{{-width / 2, height / 2,0.0f},Color,{u0, v0} },
		{{ width / 2, height / 2,0.0f},Color,{u1, v0} },
		{{-width / 2,-height / 2,0.0f},Color,{u0, v1} },
		{{ width / 2,-height / 2,0.0f},Color,{u1, v1} },
		//{{-width / 2, height / 2,0.0f},Color,{0.25f, 0.50f} },
		//{{ width / 2, height / 2,0.0f},Color,{0.50f, 0.50f} },
		//{{-width / 2,-height / 2,0.0f},Color,{0.25f, 1.00f} },
		//{{ width / 2,-height / 2,0.0f},Color,{0.50f, 1.00f} },

	};


	D3DXMATRIX mtxWorld, mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;

	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, x, y, z);

	mtxWorld = mtxInvView * mtxT;
	//mtxWorld = mtxInvView * mtxS * mtxRZ * mtxT;


	//αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0.8f);//閾値
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//判定方法

	//Zテスト,Zバッファの有効、無効の切り替え
	//Zバッファへの書き込み,Zバッファの参照をしない
	//pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	//Zバッファへの書き込みをしない
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetFVF(FVF_BILLBOARD_VERTEX);
	//pDevice->SetTexture(0, Texture_GetTexture(textureId));
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(BillboardVertex));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void BillBoard_SetViewMatrix(const D3DXMATRIX& mtxView) {

	g_mtxView = mtxView;
}