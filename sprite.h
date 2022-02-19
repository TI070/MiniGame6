#pragma once
////////////
//íËêîíËã`
////////////
#ifndef SPRITE_H
#define SPRITE_H

void Sprite_Initialize(void);

void Sprite_Finalize(void);

void Sprite_SetColor(D3DCOLOR color);
void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch,float cx,float cy,float angle);
void Sprite_Draw(int textureId, float dx, float dy, float dw, float dh);

void Sprite_Draw_test(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
void Sprite_Draw_test2(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
void Sprite_Draw_test3(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

void Sprite_SetColorDefault();
void Sprite_SetColor(int r, int g, int b, int a);

void Sprite_Draw_Turn(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle);
void Sprite_DebugFontDraw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);
#endif

