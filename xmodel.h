#pragma once
#ifndef XMODEL_H
#define XMODEL_H

#include <d3dx9.h>

#define XMODEL_INVALID_ID (-1) //������X���f���Ǘ��ԍ�

//X�t�@�C�����f���`�惂�W���[���̏�����
void XModel_Initialize(void);

void XModel_Finalize(void);

//X�t�@�C���ǂݍ���
//�߂�l:�����ԍ�
int XModel_Load(const char* pFilename);

//���f���̉��
//����:pIDs ��������������ԍ����������������̈�̐擪�A�h���X
//     count ��������������ԍ��̌�
void XModel_Release(const int* pIDs,int count);


//���f���S�J��
void XMode_ReleaseAll();

//���f���̕`��
void XModel_Draw(int id,const D3DXMATRIX* mtxModel);


#endif