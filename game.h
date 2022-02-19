/////////////////////////////////////
//ゲームモジュール
/////////////////////////////////////

#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include "config.h"

//enum scene {
//	TITLE,
//	GAME,
//	GAMEOVER,
//	GAMEEND,
//	RESULT,
//	SCENE_MAX
//};

typedef struct Vertex2D_tag {
	D3DXVECTOR4 Position;
	D3DCOLOR Color;
	D3DXVECTOR2 Texcoord;

}Vertex2D;

#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define RIGHT_LIMIT (SCREEN_WIDTH - 250.0f) //ゲーム画面右端
#define LEFT_LIMIT (0.0f) //ゲーム画面左端
#define UP_LIMIT (0.0f) //ゲーム画面上端
#define DOWN_LIMIT (SCREEN_HEIGHT) //ゲーム画面下端



//ゲームの初期化
bool Game_Initialize();

//ゲームの更新
int Game_Update();

//ゲームの描画
void Game_Draw();

//ゲームの終了処理
void Game_Finalize();

void SetCurrentScene(int scene);


#endif