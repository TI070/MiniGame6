#pragma once
#ifndef DEBUG_FONT_H
#define DEBUG_FONT_H

//デバッグフォントモジュールの初期化
void DebugFont_Initialize();

//デバッグフォントモジュールの終了処理
void DebugFont_Finalize();

//デバッグフォントモジュールの表示
//引数:描画座標,文字列へのポインタ
void DebugFont_Draw(float dx, float dy,const char* pString);

void DebugFont_Draw(float dx, float dy, float size_x, float size_y, const char* pString);
#endif