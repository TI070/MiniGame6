/////////////////////////////////////
//�Q�[�����W���[��
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

#define RIGHT_LIMIT (SCREEN_WIDTH - 250.0f) //�Q�[����ʉE�[
#define LEFT_LIMIT (0.0f) //�Q�[����ʍ��[
#define UP_LIMIT (0.0f) //�Q�[����ʏ�[
#define DOWN_LIMIT (SCREEN_HEIGHT) //�Q�[����ʉ��[



//�Q�[���̏�����
bool Game_Initialize();

//�Q�[���̍X�V
int Game_Update();

//�Q�[���̕`��
void Game_Draw();

//�Q�[���̏I������
void Game_Finalize();

void SetCurrentScene(int scene);


#endif