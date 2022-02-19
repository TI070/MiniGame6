#pragma once
#ifndef XMODEL_H
#define XMODEL_H

#include <d3dx9.h>

#define XMODEL_INVALID_ID (-1) //無効なXモデル管理番号

//Xファイルモデル描画モジュールの初期化
void XModel_Initialize(void);

void XModel_Finalize(void);

//Xファイル読み込み
//戻り値:整理番号
int XModel_Load(const char* pFilename);

//モデルの解放
//引数:pIDs 解放したい整理番号が入ったメモリ領域の先頭アドレス
//     count 解放したい整理番号の個数
void XModel_Release(const int* pIDs,int count);


//モデル全開放
void XMode_ReleaseAll();

//モデルの描画
void XModel_Draw(int id,const D3DXMATRIX* mtxModel);


#endif