#pragma once
#ifndef DEBUG_FONT_H
#define DEBUG_FONT_H

//�f�o�b�O�t�H���g���W���[���̏�����
void DebugFont_Initialize();

//�f�o�b�O�t�H���g���W���[���̏I������
void DebugFont_Finalize();

//�f�o�b�O�t�H���g���W���[���̕\��
//����:�`����W,������ւ̃|�C���^
void DebugFont_Draw(float dx, float dy,const char* pString);

void DebugFont_Draw(float dx, float dy, float size_x, float size_y, const char* pString);
#endif