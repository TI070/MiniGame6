#pragma once
#ifndef YMF_H
#define YMF_H

#include <d3dx9.h>

#define YMF_MODEL_INVALID_ID (-1) //無効なXモデル管理番号

//Xファイルモデル描画モジュールの初期化
void YmfModel_Initialize(void);

void YmfModel_Finalize(void);

//Xファイル読み込み
//戻り値:整理番号
int YmfModel_Load(const char* pFilename);

//モデルの解放
//引数:pIDs 解放したい整理番号が入ったメモリ領域の先頭アドレス
//     count 解放したい整理番号の個数
void YmfModel_Release(const int* pIDs, int count);


//モデル全開放
void YmfMode_ReleaseAll();

//モデルの描画
void YmfModel_Draw(int id, const D3DXMATRIX* mtxModel);


#endif