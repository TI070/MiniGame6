#pragma once
#ifndef FADE_H
#define FADE_H


typedef enum FadeMode_tag {
	FADE_NONE,
	FADE_IN,
	FADE_OUT

}FadeMode;

//������
void Fade_Initialize();

//�X�V
void Fade_Update();

//�`��
void Fade_Draw();

//�I������
void Fade_Finalize();

void Fade_Start(FadeMode mode,float r, float g, float b,int frame);
bool Fade_IsFade();

#endif