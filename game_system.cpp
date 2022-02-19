
#include "game_system.h"
#include "game_player.h"
#include "writestring.h"
#include "config.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "sound.h"
#include <stdio.h>


char game_system::message[64];

bool game_system::messageIsUse;
int game_system::start_counter;
int game_system::win_player;

void game_system::Initiaize() {

	messageIsUse = false;
	message[0] = '0';

	start_counter = 3 * 60;

	win_player = 99;

}

void game_system::PlayerWin(player* pP,int i) {

	pP->win++;

	char buf[64];
	sprintf(buf, "%dP Win", i + 1);
	SetMessage(buf);

	win_player = i;

}
void game_system::Draw() {

	//スタートカウントダウン
	if (StartCountdown()) return;


	if (!messageIsUse) return;

	//Zバッファへの書き込み,Zバッファの参照をしない
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	Sprite_SetColor(0,0,0,255);

	writestring::AlphabetFont_Draw(SCREEN_WIDTH / 2 - 64 * 6 / 2, 100, message, 64, 64);

	ExplainToMenu();

	Sprite_SetColorDefault();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


}


void game_system::SetMessage(char mes[64]) {

	strcpy(message,mes);

	messageIsUse = true;


}

void game_system::ExplainToMenu() {

	writestring::AlphabetFont_Draw(TOMENU_POSX, TOMENU_POSY - 32, "Enter", 32, 32);
	writestring::JapaneseFont_Draw(TOMENU_POSX + 32 * 5, TOMENU_POSY - 32, "キーで", 32, 32);
	writestring::JapaneseFont_Draw(TOMENU_POSX, TOMENU_POSY, "メニューにもどる", 32, 32);


}

//カウントダウン中ならtrueを返す
bool game_system::StartCountdown() {

	if (start_counter <= COUNTDOWN_END) return false;

	if (start_counter == 0) PlaySound(SOUND_LABEL_SE_COUNTDOWN_END);
	else if(start_counter % 60 == 0) PlaySound(SOUND_LABEL_SE_COUNTDOWN);

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	start_counter--;

	Sprite_SetColor(0,0,0,255);

	if (start_counter > 0) {
		char buf[16];
		sprintf(buf, "%d", (start_counter / 60) + 1);
		writestring::AlphabetFont_Draw(SCREEN_WIDTH / 2, TOMENU_POSY - 32, buf, 64, 64);
	}
	else if(start_counter > COUNTDOWN_END){
		writestring::AlphabetFont_Draw(TOMENU_POSX, TOMENU_POSY - 32, "START", 64, 64);

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	Sprite_SetColorDefault();


	return true;
	


}