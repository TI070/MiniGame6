
#include "xmodel.h"
#include "mydirect3D.h"
#include "texture.h"
#include <stdio.h>

struct XModel{

	LPD3DXMESH pMesh;//メッシュ
	//LPD3DXBUFFER pAdjacency;//隣接情報バッファ
	DWORD materialCount;//マテリアル数
	//マテリアル情報
	D3DMATERIAL9* pMaterials;
	//テクスチャID
	int* pTextureIds;
};

#define XMODEL_MAX (64)


static XModel g_XModels[XMODEL_MAX] = {};

//Xファイルモデル描画モジュールの初期化
void XModel_Initialize(void) {

	XMode_ReleaseAll();

	for (int i = 0; i < XMODEL_MAX; i++) {
	
		g_XModels[i].pMesh = NULL;//pMeshがNULLだったら未使用
	
	}

	

}

void XModel_Finalize(void) {

	XMode_ReleaseAll();

}

//Xファイル読み込み
//戻り値:整理番号
int XModel_Load(const char* pFilename) {


	//g_XModelsから未使用領域を検索
	int id = XMODEL_INVALID_ID;
	for (int i = 0; i < XMODEL_MAX; i++) {
		if (g_XModels[i].pMesh == NULL) {
			id = i;
			break;

		}


	}
	//idは取得できたか
	if (id == XMODEL_INVALID_ID) {

		return XMODEL_INVALID_ID;//未使用領域がなかった
	}

	//モデルデータ読み込み
	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	LPD3DXBUFFER pAdjacency;//隣接情報バッファ(一時使用)
	LPD3DXMESH pTempMesh;//モデルメッシュ(一時使用)

	LPD3DXBUFFER pMaterial;//マテリアルバッファ(itiji)
	DWORD materialcount;//マテリアル数(1ji)

	//とりあえず編集可能な状態で読み込む
	hr = D3DXLoadMeshFromX(
		pFilename,//ファイル名
		D3DXMESH_SYSTEMMEM,//オプション(メインメモリに情報を作成)
		pDevice,//Direct3DDevice
		&pAdjacency,//隣接情報(取得)
		&pMaterial,//マテリアル情報
		NULL,//エフェクト(シェーダー)(取得)
		&materialcount,//マテリアル数(取得)
		&pTempMesh //メッシュ(取得)
	);

	g_XModels[id].materialCount = materialcount;

	if (FAILED(hr)) {
		
		return XMODEL_INVALID_ID;
	}

	D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterial->GetBufferPointer();
	g_XModels[id].pMaterials = new D3DMATERIAL9[materialcount];
	g_XModels[id].pTextureIds = new int[materialcount];

	for (int i = 0; i < materialcount; i++) {

		//マテリアル値のコピー

		g_XModels[id].pMaterials[i] = pD3DXMaterial[i].MatD3D;
		//要素を選んでコピーすると良い
		//g_XModels[id].pMaterials[i].Diffuse = pD3DXMaterial[i].MatD3D.Emissive;
		
		//テクスチャのファイル名
		//pD3DXMaterial[i].pTextureFilename;

		if (pD3DXMaterial[i].pTextureFilename) {

			char buf[256];
			strcpy(buf,pFilename);

			char* pSplit = strrchr(buf,'/');
			*(pSplit + 1) = '\0';
			strcat(buf,pD3DXMaterial[i].pTextureFilename);

			g_XModels[id].pTextureIds[i] = Texture_SetTextureLoadFile(buf);
		
		}
		else {
			g_XModels[id].pTextureIds[i] = TEXTURE_INVALID_ID;

		}

	}

	//メッシュの最適化をする
	pTempMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);
		
	//高速で面倒を見てくれるtyoutennbaffawotoriatukau 
	pTempMesh->CloneMeshFVF(
		D3DXMESH_MANAGED|D3DXMESH_WRITEONLY,
		pTempMesh->GetFVF(),
		pDevice,&g_XModels[id].pMesh
		);

	//用済みのバッファを削除
	pAdjacency->Release();
	pTempMesh->Release();


	return id;
}

//モデルの解放
//引数:pIDs 解放したい整理番号が入ったメモリ領域の先頭アドレス
//     count 解放したい整理番号の個数
void XModel_Release(const int* pIDs, int count) {

	for (int i = 0; i < count; i++) {

		if (!g_XModels[pIDs[i]].pMesh) continue;

		g_XModels[pIDs[i]].pMesh->Release();
		g_XModels[pIDs[i]].pMesh = NULL;


		delete[] g_XModels[pIDs[i]].pMaterials;
		delete[] g_XModels[pIDs[i]].pTextureIds;
	}



}


//モデル全開放
void XMode_ReleaseAll() {

	for (int i = 0; i < XMODEL_MAX; i++) {

		if (!g_XModels[i].pMesh) {
			XModel_Release(&i,1);

		}
	}

}


//モデルの描画
void XModel_Draw(int id, const D3DXMATRIX* mtxModel) {


	if (!g_XModels[id].pMesh) return;//指定されたモデル番号は未使用のもの

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	pDevice->SetTransform(D3DTS_WORLD,mtxModel);

	//ライト


	D3DLIGHT9 light = {};
	//memset(&light,0,sizeof(light));第２引数で初期化
	//ZeroMemory(&light, sizeof(light));0で初期化
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-2.0f, -2.0f, 1.0f);
	//D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	//light.Direction = vecDirLight;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 0.8f;
	light.Diffuse.b = 0.8f;
	light.Diffuse.a = 1.0f;



	//////////




	//ディフューズマテリアルを設定(今回はマテリアルがマテリアル)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	//アンビエントマテリアルを設定(今回はマテリアルがマテリアル)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);



	//デバイスにライトを設定
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);   // 発電所を回す！
	//pDevice->SetRenderState(D3DRS_AMBIENT, 0xff030303);   // 世の中をちょっと白く照らす

	for (DWORD i = 0; i < g_XModels[id].materialCount;i++) {

		//D3DMATERIAL9 mat = {};
		//mat.Diffuse.r = 1.0f;
		//mat.Diffuse.g = 1.0f;
		//mat.Diffuse.b = 1.0f;
		//mat.Diffuse.a = 1.0f;
		//
		//mat.Ambient.r = 1.0f;
		//mat.Ambient.g = 1.0f;
		//mat.Ambient.b = 1.0f;
		//mat.Ambient.a = 1.0f;
		//
		//pDevice->SetMaterial(&mat);//デバイスへマテリアルを設定


		pDevice->SetMaterial(&g_XModels[id].pMaterials[i]);

		pDevice->SetTexture(0,Texture_GetTexture(g_XModels[id].pTextureIds[i]));

		//pDevice->SetTexture(0, NULL);

		g_XModels[id].pMesh->DrawSubset(i);//頂点バッファとインデックスバッファの設定
	}


}

