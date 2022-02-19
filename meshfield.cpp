#include <d3dx9.h>
#include <stdio.h>
#include "meshfield.h"
#include "mydirect3d.h"
#include "texture.h"
#include "key_manager.h"
#include "meshfield.h"

#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)

meshfield::meshfield() {

}

meshfield::~meshfield() {
	
}

meshfield::meshfield(int width, int height, int TexId, int type, D3DXVECTOR3 set_pos) {

	Initialize(width,height,TexId,type,set_pos);
}

void meshfield::Initialize(int width,int height,int Set_TexId, int type, D3DXVECTOR3 set_pos) {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	pos = set_pos;
	TexId = Set_TexId;
	walltype = type;


	VertexMesh3D* g_MeshFieldVertexs;
	WORD* g_Indexs;

	v_num = (width + 1) * (height + 1);
	shape_num = (width * height) * 2 + ((height - 1) * 4);
	index_num = (4 + (width - 1) * 2) * (height)+(height - 1) * 2;

	g_width = width;
	g_height = height;

	pDevice->CreateVertexBuffer(sizeof(VertexMesh3D) * v_num, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * index_num, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);


	g_pVertexBuffer->Lock(0, 0, (void**)&g_MeshFieldVertexs, 0);


	//頂点データの生成
	for (int i = 0; i < height + 1; i++) {
		for (int j = 0; j < width + 1; j++) {

			g_MeshFieldVertexs[(i * (width + 1)) + j].Position = D3DXVECTOR3(1.0f * j, 0.0f, -1.0f * i);
			g_MeshFieldVertexs[(i * (width + 1)) + j].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_MeshFieldVertexs[(i * (width + 1)) + j].Color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_MeshFieldVertexs[(i * (width + 1)) + j].TexCoord = D3DXVECTOR2(1.0f * j, 1.0f * i);

		}
	}

	g_pVertexBuffer->Unlock();


	int w, h;
	w = h = 1;


	g_pIndexBuffer->Lock(0, 0, (void**)&g_Indexs, 0);

	//インデックスデータの生成
	for (int i = 0; i < index_num; i++) {


		//行を移行する処理
		if ((i - ((4 + (width - 1) * 2) * (h)+(h - 1) * 2)) == 0) {

			h++;
			w = 1;

			g_Indexs[i] = g_Indexs[i - 1];
			g_Indexs[i + 1] = (width + 1) + (h - 1) * (width + 1) + (w - 1);

			i += 2;

		}

		if (i % 2 == 0) {//四角形の下部分頂点
			g_Indexs[i] = (width + 1) + (h - 1) * (width + 1) + (w - 1);
		}
		else {//四角形の上部分頂点
			g_Indexs[i] = 0 + (h - 1) * (width + 1) + (w - 1);
			w++;
		}



	}

	g_pIndexBuffer->Unlock();

	wire_sw = false;
}


void meshfield::Finalize() {



}


void meshfield::Draw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();


	//ライトを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//ライト(平行光源)の設定を作成
	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-2.0f, -2.0f, 1.0f);
	//D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	//light.Direction = vecDirLight;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	/* アンビエントは個別のライト自体にも設定でき、その場合には全体のアンビエントとこのアンビエントを足したものになるらしい
	light.Ambient.r = 1.0f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	light.Ambient.a = 1.0f;
	*/

	//ディフューズマテリアルを設定(今回は頂点カラーがマテリアル)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//アンビエントライトの設定
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 96, 128, 255));
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 192, 192, 255));


	//アンビエントマテリアルを設定(今回は頂点カラーがマテリアル)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//デバイスにライトを設定
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);


	//デバイスにワールド変換行列を設定
	D3DXMATRIX mtxWorld,mtxT,mtxT2,mtxR;
	//D3DXMatrixTranslation(&mtxT, g_width/2.0f, 1.0f, g_height/2.0f);
	D3DXMatrixTranslation(&mtxT, 0.0f, 0.0f, 0.0f);

	//どの向きの板にするか回転
	if (walltype == XZ_FIELD) {
		D3DXMatrixIdentity(&mtxR);
		
	}
	else if (walltype == XY_FIELD) {
		D3DXMatrixRotationX(&mtxR, D3DXToRadian(90));
		
	}
	else if (walltype == YZ_FIELD) {
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
	}

	//中心を合わせる
	D3DXMatrixTranslation(&mtxT, -g_width / 2.0f, 0.0f, g_height / 2.0f);

	//指定ポジションへ移動
	D3DXMatrixTranslation(&mtxT2, pos.x, pos.y, pos.z);

	mtxWorld = mtxT * mtxR * mtxT2;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//カリングOFF
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	//両面

	//デバイスにFVFの設定
	pDevice->SetFVF(FVF_VERTEX3D);

	//デバイスにテクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ワイヤーフレーム化
	//if (KeyManager_Trigger(KM_WIRE)) {
	//	wire_sw = !wire_sw;
	//}
	//if(wire_sw) pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	

	//ポリゴンの描画
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VertexMesh3D));

	//デバイスにインデックスバッファを設定
	pDevice->SetIndices(g_pIndexBuffer);
	////頂点バッファとインデックスバッファを利用したプリミティブ描画
	//描き方、最初インデックス,最小インデックス,頂点の数,最初のやつ、図形の数
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, v_num, 0, shape_num);

	//ワイヤーフレーム解除
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//カリングON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//片面

}


void meshfield::Draw(int SetTexId) {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();


	//ライトを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//ライト(平行光源)の設定を作成
	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-2.0f, -2.0f, 1.0f);
	//D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	//light.Direction = vecDirLight;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 0.8f;
	light.Diffuse.b = 0.8f;
	light.Diffuse.a = 1.0f;

	/* アンビエントは個別のライト自体にも設定でき、その場合には全体のアンビエントとこのアンビエントを足したものになるらしい
	light.Ambient.r = 1.0f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	light.Ambient.a = 1.0f;
	*/

	//ディフューズマテリアルを設定(今回は頂点カラーがマテリアル)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//アンビエントライトの設定
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 96, 128, 255));

	//アンビエントマテリアルを設定(今回は頂点カラーがマテリアル)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//デバイスにライトを設定
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);


	//デバイスにワールド変換行列を設定
	D3DXMATRIX mtxWorld, mtxT, mtxT2, mtxR;
	//D3DXMatrixTranslation(&mtxT, g_width/2.0f, 1.0f, g_height/2.0f);
	D3DXMatrixTranslation(&mtxT, 0.0f, 0.0f, 0.0f);

	//どの向きの板にするか回転
	if (walltype == XZ_FIELD) {
		D3DXMatrixIdentity(&mtxR);

	}
	else if (walltype == XY_FIELD) {
		D3DXMatrixRotationX(&mtxR, D3DXToRadian(90));

	}
	else if (walltype == YZ_FIELD) {
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
	}

	//中心を合わせる
	D3DXMatrixTranslation(&mtxT, -g_width / 2.0f, 0.0f, g_height / 2.0f);

	//指定ポジションへ移動
	D3DXMatrixTranslation(&mtxT2, pos.x, pos.y, pos.z);

	mtxWorld = mtxT * mtxR * mtxT2;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//カリングOFF
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//両面

	//デバイスにFVFの設定
	pDevice->SetFVF(FVF_VERTEX3D);

	//デバイスにテクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(SetTexId));

	//ワイヤーフレーム化
	//if (KeyManager_Trigger(KM_WIRE)) {
	//	wire_sw = !wire_sw;
	//}
	//if(wire_sw) pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);


	//ポリゴンの描画
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VertexMesh3D));

	//デバイスにインデックスバッファを設定
	pDevice->SetIndices(g_pIndexBuffer);
	////頂点バッファとインデックスバッファを利用したプリミティブ描画
	//描き方、最初インデックス,最小インデックス,頂点の数,最初のやつ、図形の数
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, v_num, 0, shape_num);

	//ワイヤーフレーム解除
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//カリングON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//片面

}


void meshfield::NotLightDraw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();


	//ライトを有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ライト(平行光源)の設定を作成
	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-2.0f, -2.0f, 1.0f);
	//D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	//light.Direction = vecDirLight;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	/* アンビエントは個別のライト自体にも設定でき、その場合には全体のアンビエントとこのアンビエントを足したものになるらしい
	light.Ambient.r = 1.0f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	light.Ambient.a = 1.0f;
	*/

	//ディフューズマテリアルを設定(今回は頂点カラーがマテリアル)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//アンビエントライトの設定
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 96, 128, 255));
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255, 255, 255, 255));


	//アンビエントマテリアルを設定(今回は頂点カラーがマテリアル)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//デバイスにライトを設定
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);


	//デバイスにワールド変換行列を設定
	D3DXMATRIX mtxWorld, mtxT, mtxT2, mtxR;
	//D3DXMatrixTranslation(&mtxT, g_width/2.0f, 1.0f, g_height/2.0f);
	D3DXMatrixTranslation(&mtxT, 0.0f, 0.0f, 0.0f);

	//どの向きの板にするか回転
	if (walltype == XZ_FIELD) {
		D3DXMatrixIdentity(&mtxR);

	}
	else if (walltype == XY_FIELD) {
		D3DXMatrixRotationX(&mtxR, D3DXToRadian(90));

	}
	else if (walltype == YZ_FIELD) {
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
	}

	//中心を合わせる
	D3DXMatrixTranslation(&mtxT, -g_width / 2.0f, 0.0f, g_height / 2.0f);

	//指定ポジションへ移動
	D3DXMatrixTranslation(&mtxT2, pos.x, pos.y, pos.z);

	mtxWorld = mtxT * mtxR * mtxT2;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//カリングOFF
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//両面

	//デバイスにFVFの設定
	pDevice->SetFVF(FVF_VERTEX3D);

	//デバイスにテクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//ワイヤーフレーム化
	//if (KeyManager_Trigger(KM_WIRE)) {
	//	wire_sw = !wire_sw;
	//}
	//if(wire_sw) pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);


	//ポリゴンの描画
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VertexMesh3D));

	//デバイスにインデックスバッファを設定
	pDevice->SetIndices(g_pIndexBuffer);
	////頂点バッファとインデックスバッファを利用したプリミティブ描画
	//描き方、最初インデックス,最小インデックス,頂点の数,最初のやつ、図形の数
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, v_num, 0, shape_num);

	//ワイヤーフレーム解除
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//カリングON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//片面

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}


void meshfield::SetPos(D3DXVECTOR3 set_pos) {
	pos = set_pos;
}