#pragma once
#ifndef MESHFIELD_H
#define MESHFIELD_H

#include <d3dx9.h>

#define MASS_SIZE (1.0f)

typedef struct VertexMesh3D_tag {
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;//法線ベクトルの長さは必ず1
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;

}VertexMesh3D;

enum WALL_TYPE{
	
	XZ_FIELD,
	XY_FIELD,
	YZ_FIELD,
};


class meshfield {

private:
	bool wire_sw;

	int shape_num;//描画する図形の数
	int v_num;//頂点の数
	int index_num;//インデックスの数

	int g_width, g_height;
	int walltype;

	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

	int TexId;
	D3DXVECTOR3 pos;

	WORD g_Indexs[10000];

	VertexMesh3D* g_MeshFieldVertexs;

public:

	meshfield();
	meshfield(int width, int height, int Set_TexId,int type,D3DXVECTOR3 set_pos);
	~meshfield();

	void Initialize(int width, int height, int TexId, int type, D3DXVECTOR3 set_pos);
	void Finalize();
	void Draw();
	void NotLightDraw();
	void Draw(int SetTexId);
	void SetPos(D3DXVECTOR3 set_pos);

};

#endif 