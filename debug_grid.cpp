#include <d3dx9.h>
#include <stdio.h>
#include "mydirect3d.h"

#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define GRIDMAX (10)
#define GRIDSIZE (1.0f)

typedef struct Vertex3D_tag {
	D3DXVECTOR3 Position;
	//D3DXVECTOR3 Normal;//法線ベクトルの長さは必ず1
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;

}Vertex3D;

static Vertex3D g_GridVertex[(GRIDMAX +1) * 4] = {

};


void DebugGrid_Initialize() {

	int GVexCount = 0;

	//縦線
	for (int i = 0; i < (GRIDMAX + 1) * 2; i = i + 2) {

		g_GridVertex[i].Position.x = GRIDSIZE * (i - GRIDMAX) / 2;
		g_GridVertex[i].Position.y = 0;
		g_GridVertex[i].Position.z = -GRIDMAX * GRIDSIZE / 2;

		g_GridVertex[i + 1].Position.x = GRIDSIZE * (i - GRIDMAX) / 2;
		g_GridVertex[i + 1].Position.y = 0;
		g_GridVertex[i + 1].Position.z = GRIDMAX * GRIDSIZE / 2;

		if ((GRIDSIZE * (i - GRIDMAX) / 2) == 0) {
			g_GridVertex[i].Color = D3DCOLOR_RGBA(255, 0, 0, 255);
			g_GridVertex[i + 1].Color = D3DCOLOR_RGBA(255, 0, 0, 255);

		}
		else {
			g_GridVertex[i].Color = D3DCOLOR_RGBA(0, 255, 0, 255);
			g_GridVertex[i + 1].Color = D3DCOLOR_RGBA(0, 255, 0, 255);
		}

		GVexCount += 2;
	}
	

	for (int i = 0;i < (GRIDMAX + 1) * 2; i = i + 2) {

		g_GridVertex[GVexCount].Position.x = -GRIDMAX * GRIDSIZE / 2;
		g_GridVertex[GVexCount].Position.y = 0;
		g_GridVertex[GVexCount].Position.z = GRIDSIZE * (i - GRIDMAX) / 2;

		g_GridVertex[GVexCount + 1].Position.x = GRIDMAX * GRIDSIZE / 2;
		g_GridVertex[GVexCount + 1].Position.y = 0;
		g_GridVertex[GVexCount + 1].Position.z = GRIDSIZE * (i - GRIDMAX) / 2;

		if ((GRIDSIZE * (i - GRIDMAX) / 2) == 0) {
			g_GridVertex[GVexCount].Color = D3DCOLOR_RGBA(255, 0, 0, 255);
			g_GridVertex[GVexCount + 1].Color = D3DCOLOR_RGBA(255, 0, 0, 255);

		}
		else {
			g_GridVertex[GVexCount].Color = D3DCOLOR_RGBA(0, 255, 0, 255);
			g_GridVertex[GVexCount + 1].Color = D3DCOLOR_RGBA(0, 255, 0, 255);
		}

		GVexCount += 2;
	}
}


void DebugGrid_Finalize() {



}

void DebugGrid_Draw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	//ワールド座標変換行列
	D3DXMATRIX mtxWorld;
	//単位行列の作成
	D3DXMatrixIdentity(&mtxWorld);
	//回転行列の作成
	//D3DXMatrixRotationY(&mtxWorld, g_angle);

	//デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);


	//デバイスにFVFの設定
	pDevice->SetFVF(FVF_VERTEX3D);

	//デバイスにテクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ライトを無効化(練習のため)
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4 * (GRIDMAX + 1), &g_GridVertex, sizeof(Vertex3D));

}


void DebugGrid_Update() {



}
