#include "ui_states.h"
#include "texture.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "player_manager.h"
#include "wall_manager.h"
#include "writestring.h"
#include "config.h"
#include "stage.h"
#include "gauge.h"

#include <stdio.h>

#define FRAME_WIDTH (150)
#define FRAME_HEIGHT (50)

#define FRAME_STARTPOS_X (30)
#define FRAME_STARTPOS_Y (30)

#define FRAME_TO_FRAME_SPACE (240)


int ui_states::FrameTexId;
int ui_states::BgTexId;
int ui_states::PointTexId;
int ui_states::LifeTexId;
int ui_states::playerBoardId[4];

void ui_states::Initialize() {

	FrameTexId = Texture_SetTextureLoadFile("asset/ui/frame.png");
	BgTexId = Texture_SetTextureLoadFile("asset/white.png");
	PointTexId = Texture_SetTextureLoadFile("asset/ui/point.png");
	LifeTexId = Texture_SetTextureLoadFile("asset/ui/life.png");

	for (int i = 0; i < 4; i++) {

		char buf[128];
		sprintf(buf, "asset/player%d_borad.png", i + 1);
		playerBoardId[i] = Texture_SetTextureLoadFile(buf);

	}

}

void ui_states::BattleDraw() {

	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);


		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH, FRAME_HEIGHT);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH, FRAME_HEIGHT);
		Sprite_SetColorDefault();

		//プレイヤーボード表示
		Sprite_Draw(playerBoardId[j], FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, 0, FRAME_WIDTH / 2, FRAME_HEIGHT / 2);

		//ゲージ表示
		Gauge_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, 30, pP->action_cooltime);

		//ポイント表示
		//for (int i = 0; i < 3; i++) {
		//	if(pP->point > i) Sprite_SetColor(255, 255, 0, 255);
		//	else Sprite_SetColor(96, 96, 96, 255);
		//
		//	Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);
		//
		//}

		//ライフ表示
		for (int i = 0; i < 3; i++) {
			if (pP->life > i) Sprite_SetColor(255, 255, 0, 255);
			else Sprite_SetColor(96, 96, 96, 255);

			Sprite_Draw(LifeTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);

		}

		Sprite_SetColorDefault();	

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void ui_states::ShootingBattleDraw() {

	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);


		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH * 2 / 3, FRAME_HEIGHT * 3);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH * 2 / 3, FRAME_HEIGHT * 3);
		Sprite_SetColorDefault();

		//プレイヤーボード表示
		Sprite_Draw(playerBoardId[j], FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, 0, FRAME_WIDTH / 2, FRAME_HEIGHT / 2);

		//ゲージ表示
		//Gauge_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT * 3, 60, pP->action_cooltime);
		Gauge_Draw2(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT * 3, 60, pP->action_cooltime,60,pP->GetActionButtonTimer());


		//ポイント表示
		for (int i = 0; i < 2; i++) {

			//プレイヤー1のポイント
			if (j == 0) {
				if (pP->point2 > i) Sprite_SetColor(PLAYER2_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point3 > i) Sprite_SetColor(PLAYER3_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point4 > i) Sprite_SetColor(PLAYER4_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT * 2, FRAME_WIDTH / 3, FRAME_HEIGHT);
			}
			else if (j == 1) {
				if (pP->point1 > i) Sprite_SetColor(PLAYER1_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point3 > i) Sprite_SetColor(PLAYER3_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point4 > i) Sprite_SetColor(PLAYER4_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT * 2, FRAME_WIDTH / 3, FRAME_HEIGHT);
			}
			else if (j == 2) {
				if (pP->point1 > i) Sprite_SetColor(PLAYER1_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point2 > i) Sprite_SetColor(PLAYER2_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point4 > i) Sprite_SetColor(PLAYER4_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT * 2, FRAME_WIDTH / 3, FRAME_HEIGHT);
			}
			else if (j == 3) {
				if (pP->point1 > i) Sprite_SetColor(PLAYER1_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point2 > i) Sprite_SetColor(PLAYER2_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, FRAME_WIDTH / 3, FRAME_HEIGHT);

				if (pP->point3 > i) Sprite_SetColor(PLAYER3_COLOR);
				else Sprite_SetColor(96, 96, 96, 255);
				Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT * 2, FRAME_WIDTH / 3, FRAME_HEIGHT);
			}

		}

		Sprite_SetColorDefault();

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}


void ui_states::BattleDomeDraw() {

	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	char buf[8];

	////タイマー表示
	//背景表示
	Sprite_SetColor(255, 255, 255, 255);
	Sprite_Draw(BgTexId, SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 20, FRAME_STARTPOS_Y - 20, FRAME_WIDTH / 2 + 10, FRAME_HEIGHT);

	//枠の表示
	Sprite_SetColor(0, 0, 0, 255);
	Sprite_Draw(FrameTexId, SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 20, FRAME_STARTPOS_Y - 20, FRAME_WIDTH / 2 + 10, FRAME_HEIGHT);
	Sprite_SetColorDefault();

	Sprite_SetColor(0, 0, 0, 255);
	sprintf(buf, "%2d", stage::game_timer / 60);
	writestring::AlphabetFont_Draw(SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 25, FRAME_STARTPOS_Y - 20, buf, 48, 48);


	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);

		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, FRAME_WIDTH, FRAME_HEIGHT);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, FRAME_WIDTH, FRAME_HEIGHT);
		Sprite_SetColorDefault();

		//プレイヤーボード表示
		Sprite_Draw(playerBoardId[j], FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, 30, FRAME_WIDTH / 2, FRAME_HEIGHT / 2);


		//ポイント表示

		if(pP->number == 0) Sprite_SetColor(PLAYER1_COLOR);
		else if (pP->number == 1) Sprite_SetColor(PLAYER2_COLOR);
		else if (pP->number == 2) Sprite_SetColor(PLAYER3_COLOR);
		else if (pP->number == 3) Sprite_SetColor(PLAYER4_COLOR);


		sprintf(buf,"%3d",pP->point);
		writestring::AlphabetFont_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30,buf,48,48);



		Sprite_SetColorDefault();

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}

void ui_states::MenuDraw() {


	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	char buf[8];

	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);

		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH, FRAME_HEIGHT);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH, FRAME_HEIGHT);
		Sprite_SetColorDefault();

		//プレイヤーボード表示
		Sprite_Draw(playerBoardId[j], FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, 0, FRAME_WIDTH / 2, FRAME_HEIGHT / 2);


		//ポイント表示

		if (pP->number == 0) Sprite_SetColor(PLAYER1_COLOR);
		else if (pP->number == 1) Sprite_SetColor(PLAYER2_COLOR);
		else if (pP->number == 2) Sprite_SetColor(PLAYER3_COLOR);
		else if (pP->number == 3) Sprite_SetColor(PLAYER4_COLOR);


		sprintf(buf, "%3d", pP->win);
		writestring::AlphabetFont_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, buf, 48, 48);



		Sprite_SetColorDefault();

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}

void ui_states::RaceDraw() {


	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);


		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH + 10, FRAME_HEIGHT);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH + 10, FRAME_HEIGHT);
		Sprite_SetColorDefault();

		//プレイヤーボード表示
		Sprite_Draw(playerBoardId[j], FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, 0, FRAME_WIDTH / 2, FRAME_HEIGHT / 2);

		//ゲージ表示
		//Gauge_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, 60, pP->action_cooltime);
		Gauge_Draw2(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + FRAME_HEIGHT, 60, pP->action_cooltime,60,pP->GetActionButtonTimer());

		//ポイント表示
		for (int i = 0; i < 5; i++) {
			if(pP->point > i) Sprite_SetColor(255, 255, 0, 255);
			else Sprite_SetColor(96, 96, 96, 255);
		
			char buf[4];
			sprintf(buf, "%d", i + 1);

			//Sprite_Draw(PointTexId, FRAME_STARTPOS_X + FRAME_WIDTH / 3 * i + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y, FRAME_WIDTH / 3, FRAME_HEIGHT);
			writestring::AlphabetFont_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE + i * 32, FRAME_STARTPOS_Y, buf,32,48);

		}

		Sprite_SetColorDefault();

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}


void ui_states::SurvivalDraw() {

	
	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	char buf[8];

	////タイマー表示
	//背景表示
	Sprite_SetColor(255, 255, 255, 255);
	Sprite_Draw(BgTexId, SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 20, FRAME_STARTPOS_Y - 20, FRAME_WIDTH / 2 + 10, FRAME_HEIGHT);

	//枠の表示
	Sprite_SetColor(0, 0, 0, 255);
	Sprite_Draw(FrameTexId, SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 20, FRAME_STARTPOS_Y - 20, FRAME_WIDTH / 2 + 10, FRAME_HEIGHT);
	Sprite_SetColorDefault();

	Sprite_SetColor(0, 0, 0, 255);
	sprintf(buf, "%2d", stage::game_timer / 60);
	writestring::AlphabetFont_Draw(SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 25, FRAME_STARTPOS_Y - 20, buf, 48, 48);


	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);

		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, FRAME_WIDTH, FRAME_HEIGHT);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, FRAME_WIDTH, FRAME_HEIGHT);
		Sprite_SetColorDefault();

		//ポイント表示

		if (pP->number == 0) Sprite_SetColor(PLAYER1_COLOR);
		else if (pP->number == 1) Sprite_SetColor(PLAYER2_COLOR);
		else if (pP->number == 2) Sprite_SetColor(PLAYER3_COLOR);
		else if (pP->number == 3) Sprite_SetColor(PLAYER4_COLOR);


		sprintf(buf, "%3d", wall_manager::CheckPaintColor(pP->number + 1));
		writestring::AlphabetFont_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, buf, 48, 48);



		Sprite_SetColorDefault();

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


}


void ui_states::ActionRaceDraw() {


	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	char buf[8];

	////タイマー表示
	//背景表示
	Sprite_SetColor(255, 255, 255, 255);
	Sprite_Draw(BgTexId, SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 20, FRAME_STARTPOS_Y - 20, FRAME_WIDTH / 2 + 10, FRAME_HEIGHT);

	//枠の表示
	Sprite_SetColor(0, 0, 0, 255);
	Sprite_Draw(FrameTexId, SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 20, FRAME_STARTPOS_Y - 20, FRAME_WIDTH / 2 + 10, FRAME_HEIGHT);
	Sprite_SetColorDefault();

	Sprite_SetColor(0, 0, 0, 255);
	sprintf(buf, "%2d", stage::game_timer / 60);
	writestring::AlphabetFont_Draw(SCREEN_WIDTH / 2 - FRAME_WIDTH / 4 - 25, FRAME_STARTPOS_Y - 20, buf, 48, 48);


	for (int j = 0; j < 4; j++) {

		player* pP = player_manager::GetPlayer(j);

		//背景表示
		Sprite_SetColor(255, 255, 255, 255);
		Sprite_Draw(BgTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, FRAME_WIDTH, FRAME_HEIGHT);

		//枠の表示
		Sprite_SetColor(0, 0, 220, 255);
		Sprite_Draw(FrameTexId, FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, FRAME_WIDTH, FRAME_HEIGHT);
		Sprite_SetColorDefault();

		//プレイヤーボード表示
		Sprite_Draw(playerBoardId[j], FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, 30, FRAME_WIDTH / 2, FRAME_HEIGHT / 2);


		//ポイント表示

		if (pP->number == 0) Sprite_SetColor(PLAYER1_COLOR);
		else if (pP->number == 1) Sprite_SetColor(PLAYER2_COLOR);
		else if (pP->number == 2) Sprite_SetColor(PLAYER3_COLOR);
		else if (pP->number == 3) Sprite_SetColor(PLAYER4_COLOR);


		sprintf(buf, "%3d", pP->point);
		writestring::AlphabetFont_Draw(FRAME_STARTPOS_X + j * FRAME_TO_FRAME_SPACE, FRAME_STARTPOS_Y + 30, buf, 48, 48);



		Sprite_SetColorDefault();

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


}