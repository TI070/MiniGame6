
#include "ymfmodel.h"
#include "mydirect3D.h"
#include "texture.h"
#include <stdio.h>

struct Node {

	D3DXVECTOR3 offsetT;
	D3DXVECTOR3 offsetR;
	D3DXVECTOR3 offsetS;

	D3DXVECTOR3 localT;
	D3DXVECTOR3 localR;
	D3DXVECTOR3 localS;

	int parentIndex;
	int childIndex;
	int siblingIndex;

	int meshIndex;
	int meshCount;

};

struct VertexYmf {

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;//�@���x�N�g���̒����͕K��1
	D3DXVECTOR2 TexCoord;

};

#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)


struct Mesh {

	long long vertexCount;

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;

};


struct YmfModel {

	Node* pNode;
	Mesh* pMesh;

};

#define YMF_MODEL_MAX (64)

static YmfModel g_YmfModels[YMF_MODEL_MAX];

void YmfModel_Initialize(void) {

	YmfMode_ReleaseAll();
}



void YmfModel_Finalize(void) {

	YmfMode_ReleaseAll();

}

//X�t�@�C���ǂݍ���
//�߂�l:�����ԍ�
int YmfModel_Load(const char* pFilename) {

	//g_XModels���疢�g�p�̈������
	int id = YMF_MODEL_INVALID_ID;
	for (int i = 0; i < YMF_MODEL_MAX; i++) {
		if (g_YmfModels[i].pNode == NULL) {
			id = i;
			break;

		}


	}
	//id�͎擾�ł�����
	if (id == YMF_MODEL_INVALID_ID) {

		return YMF_MODEL_INVALID_ID;//���g�p�̈悪�Ȃ�����
	}


	//Ymf�t�@�C���̓ǂݍ���
	FILE* fp = fopen(pFilename,"rb");

	if (fp == NULL) {

		MessageBox(NULL, "YMF�̓ǂݍ��݂Ɏ��s", "�G���[", MB_OK);
		return YMF_MODEL_INVALID_ID;

	}

	long long nodeCount, meshCount;;
	int size = sizeof(nodeCount);
	
	fread(&nodeCount,sizeof(nodeCount),1,fp);
	g_YmfModels[id].pNode = new Node[nodeCount];
	fread(g_YmfModels[id].pNode, sizeof(Node), nodeCount, fp);

	fread(&meshCount, sizeof(meshCount), 1, fp);

	g_YmfModels[id].pMesh = new Mesh[meshCount];
	
	PDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	for (int i = 0; i < meshCount; i++) {
	
		fread(&g_YmfModels[id].pMesh[i].vertexCount, 8, 1, fp);

		
		pDevice->CreateVertexBuffer(sizeof(VertexYmf) * g_YmfModels[id].pMesh[i].vertexCount,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX3D, 
			D3DPOOL_MANAGED, 
			&g_YmfModels[id].pMesh[i].pVertexBuffer,
			NULL);

		VertexYmf* pV;
		g_YmfModels[id].pMesh[i].pVertexBuffer->Lock(0,0,(void**)&pV,0);
		fread(pV,sizeof(VertexYmf) * sizeof(VertexYmf) * g_YmfModels[id].pMesh[i].vertexCount,1,fp);
		g_YmfModels[id].pMesh[i].pVertexBuffer->Unlock();

	}
	fclose(fp);


	return id;

}

//���f���̉��
//����:pIDs ��������������ԍ����������������̈�̐擪�A�h���X
//     count ��������������ԍ��̌�
void YmfModel_Release(const int* pIDs, int count) {



	for (int i = 0; i < count;i++) {

		if (!g_YmfModels[pIDs[i]].pNode) continue;

		delete[] g_YmfModels[pIDs[i]].pNode;
		g_YmfModels[pIDs[i]].pNode = NULL;
		delete[] g_YmfModels[pIDs[i]].pMesh;
		g_YmfModels[pIDs[i]].pMesh = NULL;
	}

}


//���f���S�J��
void YmfMode_ReleaseAll() {

	for (int i = 0; i < YMF_MODEL_MAX; i++) {

		if (!g_YmfModels[i].pMesh) {
			YmfModel_Release(&i, 1);

		}
	}

}

//���f���̕`��
void YmfModel_Draw(int id, const D3DXMATRIX* mtxModel) {

	//if (!g_XModels[id].pMesh) return;//�w�肳�ꂽ���f���ԍ��͖��g�p�̂���

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, mtxModel);

	//���C�g

	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DXVECTOR3 vecDirLight(-2.0f, -2.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirLight);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 0.8f;
	light.Diffuse.b = 0.8f;
	light.Diffuse.a = 1.0f;

	//�f�o�C�X�Ƀ��C�g��ݒ�
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	//////////

	D3DMATERIAL9 mat = {};
	mat.Ambient.r = 0.2f;
	mat.Ambient.g = 0.2f;
	mat.Ambient.b = 0.2f;
	mat.Ambient.a = 0.2f;

	mat.Diffuse.r = 0.8f;
	mat.Diffuse.g = 0.8f;
	mat.Diffuse.b = 0.8f;
	mat.Diffuse.a = 0.8f;

	pDevice->SetMaterial(&mat);


	//�f�B�t���[�Y�}�e���A����ݒ�(����̓}�e���A�����}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	//�A���r�G���g�}�e���A����ݒ�(����̓}�e���A�����}�e���A��)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	pDevice->SetTexture(0, NULL);

	pDevice->SetStreamSource(0,g_YmfModels[id].pMesh[0].pVertexBuffer,0,sizeof(VertexYmf));
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, g_YmfModels[id].pMesh[0].vertexCount / 3);


	/*
	for (DWORD i = 0; i < g_YmfModels[id].materialCount; i++) {

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
		//pDevice->SetMaterial(&mat);//�f�o�C�X�փ}�e���A����ݒ�

		char buf[64];
		sprintf(buf, "i = %d\n", i);
		OutputDebugString(buf);

		pDevice->SetMaterial(&g_XModels[id].pMaterials[i]);

		pDevice->SetTexture(0, Texture_GetTexture(g_XModels[id].pTextureIds[i]));


		sprintf(buf, "TG = %d\n", Texture_GetTexture(g_XModels[id].pTextureIds[i]));
		OutputDebugString(buf);


		//pDevice->SetTexture(0, NULL);

		g_XModels[id].pMesh->DrawSubset(i);//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	}
	*/



}

