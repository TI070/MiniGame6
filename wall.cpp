
#include "wall.h"
#include "texture.h"
#include "mydirect3d.h"
#include "key_manager.h"
#include "game_player.h"
#include "billboard.h"
#include "config.h"
#include "billboard.h"
#include <stdio.h>


#define WALLMOVE_NUM (0.05f)

#define CLOUD_LEFTLIMIT  (-30)
#define CLOUD_RIGHTLIMIT (30)


wall::wall() {


}

wall::wall(D3DXVECTOR3 set_pos, float s_height, float s_width, int type) {

	WallTexId = Texture_SetTextureLoadFile("asset/block_1.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height,width,WallTexId,walltype,pos);
}

void wall::Initialize() {


}


void wall::Draw() {

	pMeshField->Draw();

}

void wall::Update() {


}

D3DXVECTOR3 wall::GetPos() {
	return pos;

}

int wall::GetWallType() {
	return NORMAL_WALL;
}

void wall::Action(player* pP) {


}

transparent_wall::transparent_wall(D3DXVECTOR3 set_pos, int s_height, int s_width, int type) {

	WallTexId = Texture_SetTextureLoadFile("asset/toumei.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height, width, WallTexId, walltype, pos);
}

void transparent_wall::Draw() {

	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	pMeshField->Draw();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

int transparent_wall::GetWallType() {
	return TRANSPARENT_WALL;
}

move_wall::move_wall(D3DXVECTOR3 set_pos, int s_height, int s_width, int type, player* pP) {

	WallTexId = Texture_SetTextureLoadFile("asset/block_1.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height, width, WallTexId, walltype, pos);

	pPlayer = pP;
	pPlayer->pPlayerWall = this;

	posx_max = 0.0f;
	posz_max = 0.0f;

	switch (pPlayer->number)
	{
		case 0:
			posx_max = 1.5f;
			break;

		case 1:
			posx_max = 1.5f;
			break;

		case 2:
			posz_max = 1.5f;
			break;

		case 3:
			posz_max = 1.5f;
			break;


	}



}


void move_wall::Update() {

	//移動
	if (pPlayer->number == 0) {
		if (KeyManager_Press(KM_RIGHT)) {


			RightMove();

		}
		if (KeyManager_Press(KM_LEFT)) {

			LeftMove();
		}
	}
	else {//CPU移動
		//タイプごとの移動
		if(pPlayer->CPUmoveDir == RIGHT){
			RightMove();
		
		}
		else if (pPlayer->CPUmoveDir == LEFT) {
		
			LeftMove();
		}
	}

	

	//移動補正
	if (pPlayer->number == 0 || pPlayer->number == 1) {
		if (pos.x > posx_max) pos.x = posx_max;
		else if (pos.x < -posx_max) pos.x = -posx_max;
	}
	else {
		if (pos.z > posz_max) pos.z = posz_max;
		else if (pos.z < -posz_max) pos.z = -posz_max;
	}


	pMeshField->SetPos(pos);

}


void move_wall::RightMove() {

	switch (pPlayer->number)
	{
		case 0:

			pos.x += WALLMOVE_NUM;
			break;

		case 1:

			pos.x += WALLMOVE_NUM;
			break;

		case 2:
			
			pos.z += WALLMOVE_NUM;
			break;

		case 3:

			pos.z += WALLMOVE_NUM;
			break;


		default:
			break;
	}

}

void move_wall::LeftMove() {

	switch (pPlayer->number)
	{
	case 0:

		pos.x -= WALLMOVE_NUM;
		break;

	case 1:

		pos.x -= WALLMOVE_NUM;
		break;

	case 2:

		pos.z -= WALLMOVE_NUM;
		break;

	case 3:

		pos.z -= WALLMOVE_NUM;
		break;


	default:
		break;
	}



}

int move_wall::GetWallType() {
	return MOVE_WALL;
}



paint_wall::paint_wall(D3DXVECTOR3 set_pos, int s_height, int s_width, int type) {

	paint_type = NONE_PAINT;

	PaintWallTexId[0] = Texture_SetTextureLoadFile("asset/paintblock01.png");
	PaintWallTexId[1] = Texture_SetTextureLoadFile("asset/paintblock02.png");
	PaintWallTexId[2] = Texture_SetTextureLoadFile("asset/paintblock03.png");
	PaintWallTexId[3] = Texture_SetTextureLoadFile("asset/paintblock04.png");
	PaintWallTexId[4] = Texture_SetTextureLoadFile("asset/paintblock05.png");

	
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height, width, WallTexId, walltype, pos);

}


void paint_wall::Draw() {

	//pMeshField->Draw();
	pMeshField->Draw(PaintWallTexId[paint_type]);
}

int paint_wall::GetWallType() {

	return PAINT_WALL;
}

void paint_wall::Action(player* pP) {

	paint_type = pP->number + 1;

}


grass_wall::grass_wall(D3DXVECTOR3 set_pos, int s_height, int s_width, int type) {

	WallTexId = Texture_SetTextureLoadFile("asset/grass.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height, width, WallTexId, walltype, pos);

}

int grass_wall::GetWallType() {

	return BACKGROUND_WALL;

}


sky_wall::sky_wall(D3DXVECTOR3 set_pos, int s_height, int s_width, int type) {

	WallTexId = Texture_SetTextureLoadFile("asset/sky.png");
	//WallTexId = Texture_SetTextureLoadFile("asset/paintblock01.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height, width, WallTexId, walltype, pos);

}

void sky_wall::Draw() {

	pMeshField->NotLightDraw();

}

int sky_wall::GetWallType() {

	return BACKGROUND_WALL;

}


cloud_wall::cloud_wall(D3DXVECTOR3 set_pos, int s_height, int s_width, int type) {

	WallTexId = Texture_SetTextureLoadFile("asset/white.png");
	//WallTexId = Texture_SetTextureLoadFile("asset/paintblock01.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	height = s_height;
	width = s_width;
	walltype = type;

	pMeshField = new meshfield(height, width, WallTexId, walltype, pos);

}

void cloud_wall::Draw() {

	pMeshField->NotLightDraw();
}

void cloud_wall::Update() {

	pos.x -= 0.05f;

	if (pos.x < CLOUD_LEFTLIMIT) pos.x = CLOUD_RIGHTLIMIT;

	pMeshField->SetPos(pos);
}

int cloud_wall::GetWallType() {

	return BACKGROUND_WALL;

}