
#include "result.h"
#include "texture.h"
#include "sprite.h"
#include "mydirect3d.h"
#include "config.h"
#include "writestring.h"
#include "game_system.h"
#include "key_manager.h"
#include "scene_manager.h"


#define CLEARLOGO_WIDTH (700)
#define CLEARLOGO_HEIGHT (70)


int result::ClearTexId;

void result::Initialize() {

	ClearTexId = Texture_SetTextureLoadFile("asset/clear.png");
}


void result::Draw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);


	//タイトル表示
	Sprite_Draw(ClearTexId, SCREEN_WIDTH / 2 - CLEARLOGO_WIDTH / 2, 200, CLEARLOGO_WIDTH, CLEARLOGO_HEIGHT);

	//説明
	writestring::AlphabetFont_Draw(TOMENU_POSX, TOMENU_POSY - 32, "Enter", 32, 32);
	writestring::JapaneseFont_Draw(TOMENU_POSX + 32 * 5, TOMENU_POSY - 32, "キーで", 32, 32);
	writestring::JapaneseFont_Draw(TOMENU_POSX, TOMENU_POSY, "タイトルにもどる", 32, 32);




}


void result::Update() {


	if (KeyManager_Trigger(KM_ENTER)) {

		scene_manager::SetCurrentScene(TITLE);
	}

}