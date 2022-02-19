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


	//���_�f�[�^�̐���
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

	//�C���f�b�N�X�f�[�^�̐���
	for (int i = 0; i < index_num; i++) {


		//�s���ڍs���鏈��
		if ((i - ((4 + (width - 1) * 2) * (h)+(h - 1) * 2)) == 0) {

			h++;
			w = 1;

			g_Indexs[i] = g_Indexs[i - 1];
			g_Indexs[i + 1] = (width + 1) + (h - 1) * (width + 1) + (w - 1);

			i += 2;

		}

		if (i % 2 == 0) {//�l�p�`�̉��������_
			g_Indexs[i] = (width + 1) + (h - 1) * (width + 1) + (w - 1);
		}
		else {//�l�p�`�̏㕔�����_
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


	//���C�g��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���C�g(���s����)�̐ݒ���쐬
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

	/* �A���r�G���g�͌ʂ̃��C�g���̂ɂ��ݒ�ł��A���̏ꍇ�ɂ͑S�̂̃A���r�G���g�Ƃ��̃A���r�G���g�𑫂������̂ɂȂ�炵��
	light.Ambient.r = 1.0f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	light.Ambient.a = 1.0f;
	*/

	//�f�B�t���[�Y�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//�A���r�G���g���C�g�̐ݒ�
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 96, 128, 255));
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 192, 192, 255));


	//�A���r�G���g�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//�f�o�C�X�Ƀ��C�g��ݒ�
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);


	//�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	D3DXMATRIX mtxWorld,mtxT,mtxT2,mtxR;
	//D3DXMatrixTranslation(&mtxT, g_width/2.0f, 1.0f, g_height/2.0f);
	D3DXMatrixTranslation(&mtxT, 0.0f, 0.0f, 0.0f);

	//�ǂ̌����̔ɂ��邩��]
	if (walltype == XZ_FIELD) {
		D3DXMatrixIdentity(&mtxR);
		
	}
	else if (walltype == XY_FIELD) {
		D3DXMatrixRotationX(&mtxR, D3DXToRadian(90));
		
	}
	else if (walltype == YZ_FIELD) {
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
	}

	//���S�����킹��
	D3DXMatrixTranslation(&mtxT, -g_width / 2.0f, 0.0f, g_height / 2.0f);

	//�w��|�W�V�����ֈړ�
	D3DXMatrixTranslation(&mtxT2, pos.x, pos.y, pos.z);

	mtxWorld = mtxT * mtxR * mtxT2;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�J�����OOFF
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	//����

	//�f�o�C�X��FVF�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX3D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//���C���[�t���[����
	//if (KeyManager_Trigger(KM_WIRE)) {
	//	wire_sw = !wire_sw;
	//}
	//if(wire_sw) pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	

	//�|���S���̕`��
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VertexMesh3D));

	//�f�o�C�X�ɃC���f�b�N�X�o�b�t�@��ݒ�
	pDevice->SetIndices(g_pIndexBuffer);
	////���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�𗘗p�����v���~�e�B�u�`��
	//�`�����A�ŏ��C���f�b�N�X,�ŏ��C���f�b�N�X,���_�̐�,�ŏ��̂�A�}�`�̐�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, v_num, 0, shape_num);

	//���C���[�t���[������
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//�J�����OON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//�Ж�

}


void meshfield::Draw(int SetTexId) {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();


	//���C�g��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���C�g(���s����)�̐ݒ���쐬
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

	/* �A���r�G���g�͌ʂ̃��C�g���̂ɂ��ݒ�ł��A���̏ꍇ�ɂ͑S�̂̃A���r�G���g�Ƃ��̃A���r�G���g�𑫂������̂ɂȂ�炵��
	light.Ambient.r = 1.0f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	light.Ambient.a = 1.0f;
	*/

	//�f�B�t���[�Y�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//�A���r�G���g���C�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 96, 128, 255));

	//�A���r�G���g�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//�f�o�C�X�Ƀ��C�g��ݒ�
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);


	//�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	D3DXMATRIX mtxWorld, mtxT, mtxT2, mtxR;
	//D3DXMatrixTranslation(&mtxT, g_width/2.0f, 1.0f, g_height/2.0f);
	D3DXMatrixTranslation(&mtxT, 0.0f, 0.0f, 0.0f);

	//�ǂ̌����̔ɂ��邩��]
	if (walltype == XZ_FIELD) {
		D3DXMatrixIdentity(&mtxR);

	}
	else if (walltype == XY_FIELD) {
		D3DXMatrixRotationX(&mtxR, D3DXToRadian(90));

	}
	else if (walltype == YZ_FIELD) {
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
	}

	//���S�����킹��
	D3DXMatrixTranslation(&mtxT, -g_width / 2.0f, 0.0f, g_height / 2.0f);

	//�w��|�W�V�����ֈړ�
	D3DXMatrixTranslation(&mtxT2, pos.x, pos.y, pos.z);

	mtxWorld = mtxT * mtxR * mtxT2;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�J�����OOFF
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//����

	//�f�o�C�X��FVF�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX3D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(SetTexId));

	//���C���[�t���[����
	//if (KeyManager_Trigger(KM_WIRE)) {
	//	wire_sw = !wire_sw;
	//}
	//if(wire_sw) pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);


	//�|���S���̕`��
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VertexMesh3D));

	//�f�o�C�X�ɃC���f�b�N�X�o�b�t�@��ݒ�
	pDevice->SetIndices(g_pIndexBuffer);
	////���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�𗘗p�����v���~�e�B�u�`��
	//�`�����A�ŏ��C���f�b�N�X,�ŏ��C���f�b�N�X,���_�̐�,�ŏ��̂�A�}�`�̐�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, v_num, 0, shape_num);

	//���C���[�t���[������
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//�J�����OON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//�Ж�

}


void meshfield::NotLightDraw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();


	//���C�g��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���C�g(���s����)�̐ݒ���쐬
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

	/* �A���r�G���g�͌ʂ̃��C�g���̂ɂ��ݒ�ł��A���̏ꍇ�ɂ͑S�̂̃A���r�G���g�Ƃ��̃A���r�G���g�𑫂������̂ɂȂ�炵��
	light.Ambient.r = 1.0f;
	light.Ambient.g = 0.8f;
	light.Ambient.b = 0.8f;
	light.Ambient.a = 1.0f;
	*/

	//�f�B�t���[�Y�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	//�A���r�G���g���C�g�̐ݒ�
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(192, 96, 128, 255));
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255, 255, 255, 255));


	//�A���r�G���g�}�e���A����ݒ�(����͒��_�J���[���}�e���A��)
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);


	//�f�o�C�X�Ƀ��C�g��ݒ�
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);


	//�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	D3DXMATRIX mtxWorld, mtxT, mtxT2, mtxR;
	//D3DXMatrixTranslation(&mtxT, g_width/2.0f, 1.0f, g_height/2.0f);
	D3DXMatrixTranslation(&mtxT, 0.0f, 0.0f, 0.0f);

	//�ǂ̌����̔ɂ��邩��]
	if (walltype == XZ_FIELD) {
		D3DXMatrixIdentity(&mtxR);

	}
	else if (walltype == XY_FIELD) {
		D3DXMatrixRotationX(&mtxR, D3DXToRadian(90));

	}
	else if (walltype == YZ_FIELD) {
		D3DXMatrixRotationZ(&mtxR, D3DXToRadian(90));
	}

	//���S�����킹��
	D3DXMatrixTranslation(&mtxT, -g_width / 2.0f, 0.0f, g_height / 2.0f);

	//�w��|�W�V�����ֈړ�
	D3DXMatrixTranslation(&mtxT2, pos.x, pos.y, pos.z);

	mtxWorld = mtxT * mtxR * mtxT2;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�J�����OOFF
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//����

	//�f�o�C�X��FVF�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX3D);

	//�f�o�C�X�Ƀe�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TexId));

	//���C���[�t���[����
	//if (KeyManager_Trigger(KM_WIRE)) {
	//	wire_sw = !wire_sw;
	//}
	//if(wire_sw) pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);


	//�|���S���̕`��
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VertexMesh3D));

	//�f�o�C�X�ɃC���f�b�N�X�o�b�t�@��ݒ�
	pDevice->SetIndices(g_pIndexBuffer);
	////���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�𗘗p�����v���~�e�B�u�`��
	//�`�����A�ŏ��C���f�b�N�X,�ŏ��C���f�b�N�X,���_�̐�,�ŏ��̂�A�}�`�̐�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, v_num, 0, shape_num);

	//���C���[�t���[������
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//�J�����OON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//�Ж�

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}


void meshfield::SetPos(D3DXVECTOR3 set_pos) {
	pos = set_pos;
}