#include <d3dx9.h>
#include "mydirect3d.h"
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include <d3d9.h>
#include <time.h>
#include "debug_font.h"
#include "keyboard.h"
#include "key_manager.h"
#include "mouse.h"
#include "sound.h"
#include "fade.h"
#include "debug_primitive.h"
#include "cube.h"
#include "debug_grid.h"
#include "camera.h"
#include "meshfield.h"
#include "xmodel.h"
#include "ymfmodel.h"
#include "game_player.h"
#include "billboard.h"
#include "shadow.h"
#include "wall_manager.h"
#include "bullet_manager.h"
#include "player_manager.h"
#include "effect_manager.h"
#include "judge.h"
#include "stage.h"
#include "ui_states.h"
#include "writestring.h"
#include "game_system.h"
#include "scene_manager.h"
#include "menu.h"
#include "game_cpu.h"
#include "title.h"
#include "result.h"
#include "gauge.h"

static int g_TextureId = TEXTURE_INVALID_ID;
static int g_BillBoardTexId = TEXTURE_INVALID_ID;

int current_scene = TITLE;

float g_angle = 0, g_angle2 = 0;
float g_Value = 0;

float y;
float x = y = 0;

bool camera_mode;

static int g_xmodel_id = XMODEL_INVALID_ID;
static int g_xmodel_cart_id = XMODEL_INVALID_ID;

static int ymf_xmodel_id = XMODEL_INVALID_ID;

int mesh_id;

player* pPlayer[PLAYER_MAX];

//ゲームの初期化
bool Game_Initialize() {

	Sprite_Initialize();
	
	LPDIRECT3DDEVICE9 g_pDevice = MYDirect3D_GetDevice();

	g_TextureId = Texture_SetTextureLoadFile("asset/debug_font_32x64.png");

	if (Texture_Load() > 0) {
		MessageBox(NULL,"テクスチャの読み込みに失敗","エラー",MB_OK);
	}
	
	//システム
	srand((unsigned)time(NULL));

	//Fade_Initialize();
	DebugGrid_Initialize();
	Cube_Initialize();

	Camera_Initialize();
	Gauge_Initialize();

	//SetCamera();
	title::Initialize();
	result::Initialize();

	wall_manager::Initialize();


	bullet_manager::Initialize();
	writestring::Initialize();

	XModel_Initialize();
	YmfModel_Initialize();

	g_BillBoardTexId = Texture_SetTextureLoadFile("asset/bullet000.png");

	//GamePlayer_Initialize();
	player_manager::Initialize();
	BillBoard_Initialize();
	shadow::Initialize();
	effect_manager::Initialize();
	ui_states::Initialize();
	stage::Initialize();
	menu::Initialize();
	Fade_Initialize();

	//ステージをセット
	stage::SetStage(BATTLE_STAGE);

	pPlayer[0] = player_manager::GetPlayer(0);
	pPlayer[1] = player_manager::GetPlayer(1);
	pPlayer[2] = player_manager::GetPlayer(2);
	pPlayer[3] = player_manager::GetPlayer(3);

	//CPUのセット
	pPlayer[1]->SetCPU(CPU_RANDOM);
	pPlayer[2]->SetCPU(CPU_RANDOM);
	pPlayer[3]->SetCPU(CPU_RANDOM);

	Texture_Load();

	camera_mode = false;


	//初期シーンセット
	//scene_manager::SetCurrentScene(RESULT);
	//scene_manager::SetCurrentScene(TITLE);
	//scene_manager::SetCurrentScene(MENU);
	//scene_manager::SetCurrentScene(GAME);

	return true;
}

//ゲームの更新
int Game_Update() {

	
	


	switch (scene_manager::GetCurrentScene())
	{
		case TITLE:
			
			title::Update();

			//背景
			wall_manager::Update();
			Camera_Update(camera_mode);
			//Cube_Update();
			Camera_SetMatrixToDevice();

			break;
			
		case MENU:

			menu::Update();

			break;

		case GAME:

			Camera_Update(camera_mode);
			//Cube_Update();
			Camera_SetMatrixToDevice();

			//ゲームが開始していなければパス

			if (game_system::start_counter > 0) break;

			//if (KeyManager_Trigger(KM_MODECHANGE)) {
			//	camera_mode = !camera_mode;
			//}



			if (camera_mode);// Camera_Update(camera_mode);
			else if (pPlayer[0]->moveable) {
				D3DXVECTOR3 playerDir = { 0.0f,0.0f,0.0f };

				if (KeyManager_Press(KM_UP)) {


					playerDir += *Camera_GetFrontVector();
					pPlayer[0]->Acceleration();
					
				}
				if (KeyManager_Press(KM_DOWN)) {

					playerDir -= *Camera_GetFrontVector();
					pPlayer[0]->Acceleration();
					
				}
				if (KeyManager_Press(KM_RIGHT)) {

					playerDir += *Camera_GetRightVector();
					pPlayer[0]->Acceleration();
				}
				if (KeyManager_Press(KM_LEFT)) {

					playerDir -= *Camera_GetRightVector();
					pPlayer[0]->Acceleration();
				}

				if (KeyManager_Press(KM_SPACE)) {
					//bullet_manager::SetBullet(pPlayer[0]->GetPos(), pPlayer[0]);
					//pPlayer[0]->SetHighAccel();
					pPlayer[0]->Action();
				}
				else if (KeyManager_Release(KM_SPACE)) {
					pPlayer[0]->ActionRealse();
				}

				playerDir.y = 0.0f;
				pPlayer[0]->SetDirection(D3DXVec3Normalize(&playerDir, &playerDir));

			}


			player_manager::Update();

			bullet_manager::Update();
			effect_manager::Update();
			wall_manager::Update();
			judge::Update();
			stage::Update();

			//ビルボードにビュー行列を設定する
			BillBoard_SetViewMatrix(Camera_GetViewMatrix());


			//CPU行動
			for (int i = 1; i < 4;i++) {
				pPlayer[i]->CPUMove();

			}

			break;

		case GAMEOVER:
			
			break;

		case RESULT:
			result::Update();

			break;

	}

	//共通
	//Fade_Update();
	scene_manager::ChangeSceneFade();

	return scene_manager::GetCurrentScene();
}


//ゲームの描画
void Game_Draw() {
	
	D3DXMATRIX mtxT, mtxT2, mtxR, mtxS, mtxWorld;

	switch (scene_manager::GetCurrentScene())
	{
		case TITLE:

			//背景
			wall_manager::Draw();

			title::Draw();



			break;

		case MENU:

			menu::Draw();

			if(menu::GetMode() == FREEPLAY) ui_states::MenuDraw();

			break;

		case GAME:
			//背景

			D3DXMatrixIdentity(&mtxWorld);
			D3DXMatrixRotationY(&mtxR, g_angle);
			D3DXMatrixScaling(&mtxS, 3.0f + (2 * sinf(g_angle)), 3.0f, 3.0f);


			//mtxWorld = mtxR * mtxS;
			D3DXMatrixTranslation(&mtxT, x, 0.00, y);
			mtxWorld = mtxT * mtxR;// *mtxS;
			//Cube_Draw(&mtxWorld);


			//D3DXMatrixTranslation(&mtxT, Camera_GetAt()->x, Camera_GetAt()->y, Camera_GetAt()->z);
			//D3DXMatrixScaling(&mtxS, 0.2f, 0.2f, 0.2f);
			//mtxWorld = mtxS * mtxT;
			//Cube_Draw(&mtxWorld);

			//BillBorad_Draw(g_BillBoardTexId,0,0,0);

			wall_manager::Draw();
			bullet_manager::Draw();

			//カメラ座標表示
			const D3DXVECTOR3* camera_pos;
			camera_pos = Camera_GetPosition();

			player_manager::Draw();
			effect_manager::Draw();

			//UI
			stage::Draw();

			//システム
			game_system::Draw();
		
			break;
		case GAMEOVER:
			//背景

			break;

		case RESULT:
			result::Draw();

			break;
	}


	//共通
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	Fade_Draw();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


}
//ゲームの終了処理
void Game_Finalize() {

	//オブジェクト


	//システム


	KeyManager_Finalize();
	Mouse_Finalize();
	MYDirect3D_Finalize();
	Texture_Finalize();

	Texture_AllRelease();

	Sprite_Finalize();
	Fade_Finalize();


}


void SetCurrentScene(int scene) {

	current_scene = scene;
}


