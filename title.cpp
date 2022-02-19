
#include "title.h"
#include "texture.h"
#include "sprite.h"
#include "mydirect3d.h"
#include "config.h"
#include "writestring.h"
#include "key_manager.h"
#include "scene_manager.h"
#include "menu.h"
#include "sound.h"
#include <stdio.h>

#define TITLELOGO_WIDTH (420)
#define TITLELOGO_HEIGHT (70)




int title::TitleTexId;
int title::select;


void title::Initialize() {

	//BGM
	PlaySound(SOUND_LABEL_BGMTITLE);

	TitleTexId = Texture_SetTextureLoadFile("asset/title.png");

	select = 0;

}


void title::Draw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	Sprite_SetColor(255, 127, 80,255);
	//タイトル表示
	Sprite_Draw(TitleTexId,SCREEN_WIDTH / 2 - TITLELOGO_WIDTH / 2, 200,TITLELOGO_WIDTH, TITLELOGO_HEIGHT);

	Sprite_SetColorDefault();

	//メニュー表示
	char buf[128];
	if (select == FREEPLAY) Sprite_SetColor(255,255,255,255);
	else Sprite_SetColor(128, 128, 128, 255);
	sprintf(buf,"フリープレイ");
	writestring::JapaneseFont_Draw(SCREEN_WIDTH / 2 - 64 * 3, 500, buf);
	if (select == WINPLAY) Sprite_SetColor(255, 255, 255, 255);
	else Sprite_SetColor(128, 128, 128, 255);
	sprintf(buf, "かちぬきプレイ");
	writestring::JapaneseFont_Draw(SCREEN_WIDTH / 2 - 64 * 3.5f, 600, buf);

	Sprite_SetColorDefault();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}


void title::Update() {

	MenuSelect();

}

void title::MenuSelect() {

	if (KeyManager_Trigger(KM_UP)) {
		//SE
		PlaySound(SOUND_LABEL_SE_CURSOL);

		select--;

		if (select < 0) select = TITLE_MAX - 1;

	}
	else if (KeyManager_Trigger(KM_DOWN)) {
		//SE
		PlaySound(SOUND_LABEL_SE_CURSOL);
		
		select++;

		select %= TITLE_MAX;

	}
	else if (KeyManager_Trigger(KM_SPACE)) {
		//SE
		PlaySound(SOUND_LABEL_SE_KETTEI);

		scene_manager::SetCurrentScene(MENU);

		menu::SetMode(select);

		menu::SetSelectGame(0);

	}


}