#pragma once
#ifndef YMF_H
#define YMF_H

#include <d3dx9.h>

#define YMF_MODEL_INVALID_ID (-1) //������X���f���Ǘ��ԍ�

//X�t�@�C�����f���`�惂�W���[���̏�����
void YmfModel_Initialize(void);

void YmfModel_Finalize(void);

//X�t�@�C���ǂݍ���
//�߂�l:�����ԍ�
int YmfModel_Load(const char* pFilename);

//���f���̉��
//����:pIDs ��������������ԍ����������������̈�̐擪�A�h���X
//     count ��������������ԍ��̌�
void YmfModel_Release(const int* pIDs, int count);


//���f���S�J��
void YmfMode_ReleaseAll();

//���f���̕`��
void YmfModel_Draw(int id, const D3DXMATRIX* mtxModel);


#endif