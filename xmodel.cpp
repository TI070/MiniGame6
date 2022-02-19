
#include "xmodel.h"
#include "mydirect3D.h"
#include "texture.h"
#include <stdio.h>

struct XModel{

	LPD3DXMESH pMesh;//���b�V��
	//LPD3DXBUFFER pAdjacency;//�אڏ��o�b�t�@
	DWORD materialCount;//�}�e���A����
	//�}�e���A�����
	D3DMATERIAL9* pMaterials;
	//�e�N�X�`��ID
	int* pTextureIds;
};

#define XMODEL_MAX (64)


static XModel g_XModels[XMODEL_MAX] = {};

//X�t�@�C�����f���`�惂�W���[���̏�����
void XModel_Initialize(void) {

	XMode_ReleaseAll();

	for (int i = 0; i < XMODEL_MAX; i++) {
	
		g_XModels[i].pMesh = NULL;//pMesh��NULL�������疢�g�p
	
	}

	

}

void XModel_Finalize(void) {

	XMode_ReleaseAll();

}

//X�t�@�C���ǂݍ���
//�߂�l:�����ԍ�
int XModel_Load(const char* pFilename) {


	//g_XModels���疢�g�p�̈������
	int id = XMODEL_INVALID_ID;
	for (int i = 0; i < XMODEL_MAX; i++) {
		if (g_XModels[i].pMesh == NULL) {
			id = i;
			break;

		}


	}
	//id�͎擾�ł�����
	if (id == XMODEL_INVALID_ID) {

		return XMODEL_INVALID_ID;//���g�p�̈悪�Ȃ�����
	}

	//���f���f�[�^�ǂݍ���
	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	LPD3DXBUFFER pAdjacency;//�אڏ��o�b�t�@(�ꎞ�g�p)
	LPD3DXMESH pTempMesh;//���f�����b�V��(�ꎞ�g�p)

	LPD3DXBUFFER pMaterial;//�}�e���A���o�b�t�@(itiji)
	DWORD materialcount;//�}�e���A����(1ji)

	//�Ƃ肠�����ҏW�\�ȏ�Ԃœǂݍ���
	hr = D3DXLoadMeshFromX(
		pFilename,//�t�@�C����
		D3DXMESH_SYSTEMMEM,//�I�v�V����(���C���������ɏ����쐬)
		pDevice,//Direct3DDevice
		&pAdjacency,//�אڏ��(�擾)
		&pMaterial,//�}�e���A�����
		NULL,//�G�t�F�N�g(�V�F�[�_�[)(�擾)
		&materialcount,//�}�e���A����(�擾)
		&pTempMesh //���b�V��(�擾)
	);

	g_XModels[id].materialCount = materialcount;

	if (FAILED(hr)) {
		
		return XMODEL_INVALID_ID;
	}

	D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterial->GetBufferPointer();
	g_XModels[id].pMaterials = new D3DMATERIAL9[materialcount];
	g_XModels[id].pTextureIds = new int[materialcount];

	for (int i = 0; i < materialcount; i++) {

		//�}�e���A���l�̃R�s�[

		g_XModels[id].pMaterials[i] = pD3DXMaterial[i].MatD3D;
		//�v�f��I��ŃR�s�[����Ɨǂ�
		//g_XModels[id].pMaterials[i].Diffuse = pD3DXMaterial[i].MatD3D.Emissive;
		
		//�e�N�X�`���̃t�@�C����
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

	//���b�V���̍œK��������
	pTempMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);
		
	//�����Ŗʓ|�����Ă����tyoutennbaffawotoriatukau 
	pTempMesh->CloneMeshFVF(
		D3DXMESH_MANAGED|D3DXMESH_WRITEONLY,
		pTempMesh->GetFVF(),
		pDevice,&g_XModels[id].pMesh
		);

	//�p�ς݂̃o�b�t�@���폜
	pAdjacency->Release();
	pTempMesh->Release();


	return id;
}

//���f���̉��
//����:pIDs ��������������ԍ����������������̈�̐擪�A�h���X
//     count ��������������ԍ��̌�
void XModel_Release(const int* pIDs, int count) {

	for (int i = 0; i < count; i++) {

		if (!g_XModels[pIDs[i]].pMesh) continue;

		g_XModels[pIDs[i]].pMesh->Release();
		g_XModels[pIDs[i]].pMesh = NULL;


		delete[] g_XModels[pIDs[i]].pMaterials;
		delete[] g_XModels[pIDs[i]].pTextureIds;
	}



}


//���f���S�J��
void XMode_ReleaseAll() {

	for (int i = 0; i < XMODEL_MAX; i++) {

		if (!g_XModels[i].pMesh) {
			XModel_Release(&i,1);

		}
	}

}


//���f���̕`��
void XModel_Draw(int id, const D3DXMATRIX* mtxModel) {


	if (!g_XModels[id].pMesh) return;//�w�肳�ꂽ���f���ԍ��͖��g�p�̂���

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	pDevice->SetTransform(D3DTS_WORLD,mtxModel);

	//���C�g


	D3DLIGHT9 light = {};
	//memset(&light,0,sizeof(light));��Q�����ŏ�����
	//ZeroMemory(&light, sizeof(light));0�ŏ�����
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




	//�f�B�t���[�Y�}�e���A����ݒ�(����̓}�e���A�����}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	//�A���r�G���g�}�e���A����ݒ�(����̓}�e���A�����}�e���A��)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);



	//�f�o�C�X�Ƀ��C�g��ݒ�
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);   // ���d�����񂷁I
	//pDevice->SetRenderState(D3DRS_AMBIENT, 0xff030303);   // ���̒���������Ɣ����Ƃ炷

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
		//pDevice->SetMaterial(&mat);//�f�o�C�X�փ}�e���A����ݒ�


		pDevice->SetMaterial(&g_XModels[id].pMaterials[i]);

		pDevice->SetTexture(0,Texture_GetTexture(g_XModels[id].pTextureIds[i]));

		//pDevice->SetTexture(0, NULL);

		g_XModels[id].pMesh->DrawSubset(i);//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	}


}

