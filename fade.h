#pragma once
#ifndef FADE_H
#define FADE_H


typedef enum FadeMode_tag {
	FADE_NONE,
	FADE_IN,
	FADE_OUT

}FadeMode;

//初期化
void Fade_Initialize();

//更新
void Fade_Update();

//描画
void Fade_Draw();

//終了処理
void Fade_Finalize();

void Fade_Start(FadeMode mode,float r, float g, float b,int frame);
bool Fade_IsFade();

#endif