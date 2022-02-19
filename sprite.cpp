#include "mydirect3d.h"
#include "game.h"
#include "texture.h"
#include <d3dx9.h>
#include <d3d9.h>

//グローバル変数
static LPDIRECT3DTEXTURE9 g_pTexture3[6] = { NULL,NULL,NULL,NULL,NULL,NULL };//テクスチャインターフェース テクスチャ１つにつき１つ必要
static float f = 0.0f;

static float move_x = 0.0f;
bool turn = false;

// FVF ...　フレキシブルバーテックスフォーマット
// 座標変換済み頂点 RHW = 1

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;//頂点バッファインターフェース
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;//インデックスバッファインターフェース
static D3DCOLOR g_Color = 0xffffffff;

void Sprite_Initialize(void) {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	if (!pDevice) {

		return;
	}

	//頂点バッファバイト数,使い方,FVF,メモリの管理方法,取得したインターフェースのアドレスを記録するポインタ,謎
	pDevice->CreateVertexBuffer(sizeof(Vertex2D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVertexBuffer,NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD)*6,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);

	Texture_Initialize();
}

void Sprite_Finalize(void) {

	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if (g_pIndexBuffer) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
}

//スプライトポリゴンカラー設定
void Sprite_SetColor(D3DCOLOR color) {

	g_Color = color;
}

void Sprite_SetColor(int r, int g, int b, int a) {

	g_Color = (a << 24) + (r << 16) + (g << 8) + (b);
}

//カラー初期化
void Sprite_SetColorDefault() {

	g_Color = 0xffffffff;
}



void Sprite_Draw_test3(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(dx - 0.5f		,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(dx - 0.5f + dw	,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(dx - 0.5f		,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u0,v1)},
		{D3DXVECTOR4(dx - 0.5f + dw	,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u1,v1)},
	};



	//頂点バッファのロックとデータの書き込み
	Vertex2D* pV;	
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);//書き込みの確保

	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();//書き込みの終了


	//インデックスバッファのロックと書き込み
	WORD* pI;

	g_pIndexBuffer->Lock(0,0,(void**)&pI,0);
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 3;
	pI[3] = 0;
	pI[4] = 3;
	pI[5] = 2;
	g_pIndexBuffer->Unlock();

	//デバイスの頂点バッファを設定
	g_pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	//デバイスにインデックスバッファを設定
	g_pDevice->SetIndices(g_pIndexBuffer);
	////頂点バッファとインデックスバッファを利用したプリミティブ描画
	//描き方、最初インデックス,最小インデックス,頂点の数,最初のやつ、図形の数
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
}


//dx ・・・描画座標
//dy ・・・描画座標
//dw ・・・ポリゴンサイズ幅
//dh ・・・ポリゴンサイズ高さ
//tcx ・・・描画座標
//tcy ・・・描画座標
//tcw ・・・
//tch ・・・
void Sprite_Draw_test2(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(dx - 0.5f		,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(dx - 0.5f + dw	,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(dx - 0.5f		,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u0,v1)},
		{D3DXVECTOR4(dx - 0.5f + dw	,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u1,v1)},
	};

	//頂点バッファのロックとデータの書き込み
	Vertex2D* pV;

	////これが大切
	//引数 ロックする先頭 0は先頭、ロックする長さ 0は全部
	g_pVertexBuffer->Lock(0,0,(void**)&pV,0);//書き込みの確保

	memcpy(pV,v,sizeof(v));

	g_pVertexBuffer->Unlock();//書き込みの終了

	//頂点バッファの指定
	g_pDevice->SetStreamSource(0,g_pVertexBuffer,0,sizeof(Vertex2D));
	
	//プリミティブ描画
	//引数 描き方　スタート地点 いくつ描くか
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);

}

void Sprite_Draw_test(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(dx - 0.5f		,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(dx - 0.5f + dw	,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(dx - 0.5f		,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u0,v1)},
		{D3DXVECTOR4(dx - 0.5f + dw	,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u1,v1)},
	};

	//インデックスデータ
	unsigned short index[] = { 0,1,3,0,3,2 };





	// 最小のインデックス値,使う頂点、図形の数,インデックスデータの先頭アドレス,フォーマット,頂点データ,隣の頂点までのバイト数
	g_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2D));

	//ポリゴンを書く(簡易版)
	//引数 描画タイプ、数、先頭アドレス,隣の頂点まで何バイト
	//g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, v, sizeof(Vertex2D));
}

void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	Vertex2D v[] = {
		{D3DXVECTOR4(dx - 0.5f,dy - 0.5f,0.0f,1.0f)				,g_Color,D3DXVECTOR2(0,0)},
		{D3DXVECTOR4(dx - 0.5f + dw,dy - 0.5f,0.0f,1.0f)		,g_Color,D3DXVECTOR2(1,0)},
		{D3DXVECTOR4(dx - 0.5f,dy - 0.5f + dh,0.0f,1.0f)		,g_Color,D3DXVECTOR2(0,1)},
		{D3DXVECTOR4(dx - 0.5f + dw,dy - 0.5f + dh,0.0f,1.0f)	,g_Color,D3DXVECTOR2(1,1)},
	};
	//ポリゴンを書く(簡易版)
	//引数 描画タイプ、数、先頭アドレス,隣の頂点まで何バイト
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void Sprite_Draw(int textureId, float dx, float dy,float dw,float dh,int tcx,int tcy,int tcw, int tch) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0,Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(dx - 0.5f,dy - 0.5f,1.0f,1.0f)				,g_Color,D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(dx - 0.5f + dw,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(dx - 0.5f,dy - 0.5f + dh,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u0,v1)},
		{D3DXVECTOR4(dx - 0.5f + dw,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u1,v1)},
	};
	//ポリゴンを書く(簡易版)
	//引数 描画タイプ、数、先頭アドレス,隣の頂点まで何バイト
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//cx.cy 回転拡大の中心座標
//angle ポリゴンの回転角度

void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(	- 0.5f,		 - 0.5f		,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(	- 0.5f + dw, - 0.5f		,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(	- 0.5f,		 - 0.5f + dh	,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u0,v1)},
		{D3DXVECTOR4(	- 0.5f + dw, - 0.5f + dh	,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u1,v1)},
	};

	//平行移動行列
	D3DXMATRIX mtxTranslationC;//元に戻す
	D3DXMatrixTranslation(&mtxTranslationC,-cx,-cy,0.0f);
	
	D3DXMATRIX mtxTranslationI;//移動させる
	D3DXMatrixTranslation(&mtxTranslationI, cx + dx, cy + dx, 0.0f);


	//回転行列
	D3DXMATRIX mtxRotation;
	D3DXMatrixRotationZ(&mtxRotation,angle);

	//拡大行列
	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale,1.0f,1.0f,1.0f);

	////拡大して回転して平行移動
	//行列の合成
	D3DXMATRIX mtxWorld;
	mtxWorld = mtxTranslationC * mtxRotation * mtxTranslationI;


	
	//座標変換
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].Position,&v[i].Position,&mtxWorld);
	}


	//ポリゴンを書く(簡易版)
	//引数 描画タイプ、数、先頭アドレス,隣の頂点まで何バイト
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void Sprite_Draw_Turn(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(-0.5f,		 -0.5f		,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(-0.5f + dw, -0.5f		,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(-0.5f,		 -0.5f + dh	,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u1,v1)},
		{D3DXVECTOR4(-0.5f + dw, -0.5f + dh	,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u0,v1)},
	};

	//平行移動行列
	D3DXMATRIX mtxTranslationC;//元に戻す
	D3DXMatrixTranslation(&mtxTranslationC, -cx, -cy, 0.0f);

	D3DXMATRIX mtxTranslationI;//移動させる
	D3DXMatrixTranslation(&mtxTranslationI, cx + dx, cy + dy, 0.0f);


	//回転行列
	D3DXMATRIX mtxRotation;
	D3DXMatrixRotationZ(&mtxRotation, angle);

	//拡大行列
	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);

	////拡大して回転して平行移動
	//行列の合成
	D3DXMATRIX mtxWorld;
	mtxWorld = mtxTranslationC * mtxRotation * mtxTranslationI;



	//座標変換
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].Position, &v[i].Position, &mtxWorld);
	}


	//ポリゴンを書く(簡易版)
	//引数 描画タイプ、数、先頭アドレス,隣の頂点まで何バイト
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

void Sprite_DebugFontDraw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch) {

	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_pDevice->SetFVF(FVF_VERTEX2D);

	g_pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	Vertex2D v[] = {
		{D3DXVECTOR4(dx - 0.5f,dy - 0.5f,1.0f,1.0f)				,g_Color,D3DXVECTOR2(u0,v0)},
		{D3DXVECTOR4(dx - 0.5f + dw,dy - 0.5f,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u1,v0)},
		{D3DXVECTOR4(dx - 0.5f,dy - 0.5f + dh,1.0f,1.0f)		,g_Color,D3DXVECTOR2(u0,v1)},
		{D3DXVECTOR4(dx - 0.5f + dw,dy - 0.5f + dh,1.0f,1.0f)	,g_Color,D3DXVECTOR2(u1,v1)},
	};
	//ポリゴンを書く(簡易版)
	//引数 描画タイプ、数、先頭アドレス,隣の頂点まで何バイト
	g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}