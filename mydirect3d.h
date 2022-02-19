#pragma once
//
//
#include <d3d9.h>

#ifndef MY_DIRECT3D_H
#define MY_DIRECT3D_H

//Direct3D9の初期化 戻り値:失敗したらfalse
bool MYDirect3D_Initialize(HWND hWnd);

//Direct3D9の終了
void MYDirect3D_Finalize();

//Direct3Dデバイスの取得
LPDIRECT3DDEVICE9 MYDirect3D_GetDevice(void);//アクセサー(ゲッター・セッター)

#endif //MY_DIRECT3D_H