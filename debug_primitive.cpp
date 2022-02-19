
#include "texture.h"
#include "sprite.h"
#include "game.h"
#include <d3dx9.h>
#include <math.h>
#include "mydirect3d.h"

#if defined(_DEBUG) || defined(DEBUG)

typedef struct DebugVertex_tag {

	D3DXVECTOR4 position;
	D3DCOLOR color;

}DebugVertex;

#define FVF_DEBUG_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


#define CIRCLE_VERTEX_COUNT (8)
#define CIRCLE_DRAW_MAX (2048)


static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static int g_CircleCount = 0;
static DebugVertex* g_pDebugVertex = NULL;
static WORD* g_pDebugVertexIndex = NULL;

#endif

void DebugPrimitive_Initialize() {

#if defined(_DEBUG) || defined(DEBUG)

	//各グローバル変数の初期化
	g_CircleCount = 0;
	g_pDebugVertex = NULL;
	g_pDebugVertexIndex = NULL;

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(DebugVertex) * CIRCLE_VERTEX_COUNT * CIRCLE_DRAW_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_DEBUG_VERTEX,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

	pDevice->CreateIndexBuffer(
		sizeof(DebugVertex) * CIRCLE_VERTEX_COUNT * 2 * CIRCLE_DRAW_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,
		NULL
	);
#endif
}


void DebugPrimitive_Finalize() {

#if defined(_DEBUG) || defined(DEBUG)

	if (g_pIndexBuffer) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

#endif

}

void DebugPrimitive_BatchBegin() {

#if defined(_DEBUG) || defined(DEBUG)

	//頂点バッファと頂点インデックスバッファをロックする
	g_pVertexBuffer->Lock(0,0,(void**)&g_pDebugVertex,0);
	g_pIndexBuffer->Lock(0, 0, (void**)&g_pDebugVertexIndex, 0);


	//丸の描画命令数を初期化
	g_CircleCount = 0;

#endif

}

void DebugPrimitive_BatchDrawCircle(float x, float y, float radius) {

#if defined(_DEBUG) || defined(DEBUG)
	//指定座標を円の中心とした軽の頂点座標を計算して、
	//頂点バッファと頂点インデックスバッファに書き込む


	//配列のどこから書き入れるか
	int n = CIRCLE_VERTEX_COUNT * g_CircleCount;

	//図形の角一つ分の角度(ラジアン角)
	float s = D3DX_PI * 2.0 / CIRCLE_VERTEX_COUNT;

	//頂点バッファとインデックスバッファへ情報を書き込む
	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		g_pDebugVertex[n + i].position.x = cosf(s * i) * radius + x;
		g_pDebugVertex[n + i].position.y = sinf(s * i) * radius + y;
		g_pDebugVertex[n + i].position.z = 1.0f;
		g_pDebugVertex[n + i].position.w = 1.0f;
		g_pDebugVertex[n + i].color = D3DCOLOR_RGBA(255,0,0,255);
		
		g_pDebugVertexIndex[n * 2 + i * 2] = n + i;
		g_pDebugVertexIndex[n * 2 + i * 2 + 1] = n + (i + 1) % CIRCLE_VERTEX_COUNT;

	}

	//丸の描画命令数を一増やす
	g_CircleCount++;

#endif

}

void DebugPrimitive_BatchRun() {

#if defined(_DEBUG) || defined(DEBUG)

	//丸の描画命令数分の丸を書く
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	int vn = g_CircleCount * CIRCLE_VERTEX_COUNT;

	pDevice->SetFVF(FVF_DEBUG_VERTEX);
	pDevice->SetStreamSource(0,g_pVertexBuffer,0,sizeof(DebugVertex));
	pDevice->SetTexture(0,NULL);
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST,0,0,vn,0,vn);

	//頂点バッファと頂点インデックスバッファをアンロックする
	g_pVertexBuffer->Unlock();
	g_pIndexBuffer->Unlock();

#endif

}