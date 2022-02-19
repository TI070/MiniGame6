#include "menu.h"
#include "texture.h"
#include "sprite.h"
#include "writestring.h"
#include "key_manager.h"
#include "scene_manager.h"
#include "config.h"
#include "stage.h"
#include "mydirect3d.h"
#include "sound.h"
#include "title.h"
#include <stdio.h>

#define SELECTMENU_STARTPOS_X (150)
#define SELECTMENU_STARTPOS_Y (100)

#define MENU_GAMEEXPLAIN_STARTPOS_X (50)

#define MENU_GAMEEXPLAIN_WIDTH  (570)
#define MENU_GAMEEXPLAIN_HEIGHT (240)

#define MENU_GAMESCREEN_HEIGHT (240)
#define MENU_GAMESCREEN_WIDTH (320)

#define MENU_GAMESCREEN_STARTPOS_X (SCREEN_WIDTH - MENU_GAMESCREEN_WIDTH - 10)
#define MENU_GAMESCREEN_STARTPOS_Y (SCREEN_HEIGHT - 250)

enum MENU_EXP_PAGE {

	RULE,
	SOUSA,
	PAGE_MAX

};

int menu::select_game;

int menu::FrameTexId;
int menu::BgTexId;
int menu::GameScreenTexId[STAGE_MAX];
int menu::exp_page;
int menu::mode;

void menu::Initialize() {

	FrameTexId = Texture_SetTextureLoadFile("asset/ui/frame.png");
	BgTexId = Texture_SetTextureLoadFile("asset/white.png");

	char buf[128];

	for (int i = 0; i < STAGE_MAX; i++) {
		sprintf(buf, "asset/scrshot_0%d.png",i + 1);
		GameScreenTexId[i] = Texture_SetTextureLoadFile(buf);
	}

	exp_page = RULE;

	select_game = 0;

}

void menu::Draw() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	char buf[32];

	for (int i = 0; i <= STAGE_MAX; i++) {

		if(select_game == i) Sprite_SetColor(255,255,255,255);
		else Sprite_SetColor(224, 224, 224, 255);

		if(i != STAGE_MAX) sprintf(buf, "%d",i + 1);
		else sprintf(buf, "END");

		writestring::AlphabetFont_Draw(SELECTMENU_STARTPOS_X + ((i % 3)* 200), SELECTMENU_STARTPOS_Y + (i / 3 * 150), buf);

	}

	if (select_game != STAGE_MAX) {
		//�Q�[�������\��
		MenuExplainDraw();

		//�������
		if (mode == FREEPLAY) {//�����ʂ����[�h�͑I�����Ȃ�
			writestring::AlphabetFont_Draw(SELECTMENU_STARTPOS_X + 2.4f * 200, SELECTMENU_STARTPOS_Y, "WASD:", 32, 32);
			writestring::JapaneseFont_Draw(SELECTMENU_STARTPOS_X + 2.4f * 200 + 32 * 5, SELECTMENU_STARTPOS_Y, "���ǂ�", 32, 32);
		}

		writestring::AlphabetFont_Draw(SELECTMENU_STARTPOS_X + 2.4f * 200, SELECTMENU_STARTPOS_Y + 40, "Space:", 32, 32);
		writestring::JapaneseFont_Draw(SELECTMENU_STARTPOS_X + 2.4f * 200 + 32 * 6, SELECTMENU_STARTPOS_Y + 40, "�����Ă�", 32, 32);

		writestring::AlphabetFont_Draw(SELECTMENU_STARTPOS_X + 2.4f * 200, SELECTMENU_STARTPOS_Y + 80, "Q:", 32, 32);
		writestring::JapaneseFont_Draw(SELECTMENU_STARTPOS_X + 2.4f * 200 + 32 * 2, SELECTMENU_STARTPOS_Y + 80, "�y�[�W�߂���", 32, 32);
	}
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}

void menu::Update() {

	if (KeyManager_Trigger(KM_RIGHT)) {
		
		//�����ʂ����[�h�͑I���ł��Ȃ�
		if (mode == WINPLAY) return;
		
		SelectChange(1);

	}
	else if (KeyManager_Trigger(KM_LEFT)) {
		//�����ʂ����[�h�͑I���ł��Ȃ�
		if (mode == WINPLAY) return;

		SelectChange(-1);

	}
	else if (KeyManager_Trigger(KM_UP)) {
		//�����ʂ����[�h�͑I���ł��Ȃ�
		if (mode == WINPLAY) return;

		SelectChange(-3);

	}
	else if (KeyManager_Trigger(KM_DOWN)) {
		//�����ʂ����[�h�͑I���ł��Ȃ�
		if (mode == WINPLAY) return;

		SelectChange(3);

	}
	else if (KeyManager_Trigger(KM_SPACE)) {
		
		//������
		//if(select_game != ACTION_RACE_STAGE) 
		GameSelect();
	}

	//�y�[�W�߂���
	if (KeyManager_Trigger(KM_Q)) {
		PageChange(1);

	}

}

void menu::SelectChange(int num) {

	//SE
	PlaySound(SOUND_LABEL_SE_CURSOL);

	if (num == 1) {
		if ((select_game + 1) % 3 == 0) select_game -= 2;
		else select_game += num;
	}
	else if (num == -1) {
		if (select_game % 3 == 0) select_game += 2;
		else select_game += num;
	}
	else if (num == 3) {
		select_game += num;
		//select_game %= STAGE_MAX;
	}
	else if (num == -3) {
		select_game += num;
		//select_game %= (STAGE_MAX + 1);
	}

	if (select_game < 0) select_game = STAGE_MAX;
	else if (select_game > STAGE_MAX && select_game < STAGE_MAX + 3) select_game = STAGE_MAX;
	else if (select_game >= STAGE_MAX + 3)select_game = 0;

}

void menu::SelectPlus(int num) {

	select_game++;
	select_game %= STAGE_MAX;

}


void menu::GameSelect() {

	stage::UnInit();

	//SE
	PlaySound(SOUND_LABEL_SE_KETTEI);

	if (select_game == STAGE_MAX) {
		scene_manager::SetCurrentScene(GAMEEND);
		return;
	}
	else {
		stage::SetStage(select_game);
		scene_manager::SetCurrentScene(GAME);
	}

	//BGM
	StopSound(SOUND_LABEL_BGMTITLE);
	PlaySound(SOUND_LABEL_BGMSTAGE);



}


void menu::MenuExplainDraw() {


	//�w�i�\��
	Sprite_SetColor(255, 255, 255, 255);
	Sprite_Draw(BgTexId, MENU_GAMEEXPLAIN_STARTPOS_X, SCREEN_HEIGHT - 250, MENU_GAMEEXPLAIN_WIDTH, MENU_GAMEEXPLAIN_HEIGHT);

	//�g�̕\��
	Sprite_SetColor(0, 0, 220, 255);
	Sprite_Draw(FrameTexId, MENU_GAMEEXPLAIN_STARTPOS_X, SCREEN_HEIGHT - 250, MENU_GAMEEXPLAIN_WIDTH, MENU_GAMEEXPLAIN_HEIGHT);
	Sprite_SetColorDefault();

	//�Q�[�����
	Sprite_SetColor(255, 255, 255, 255);
	Sprite_Draw(GameScreenTexId[select_game], MENU_GAMESCREEN_STARTPOS_X, MENU_GAMESCREEN_STARTPOS_Y, MENU_GAMESCREEN_WIDTH, MENU_GAMESCREEN_HEIGHT);

	//�Q�[����ʘg
	Sprite_SetColor(128, 128, 0, 255);
	Sprite_Draw(FrameTexId, MENU_GAMESCREEN_STARTPOS_X, MENU_GAMESCREEN_STARTPOS_Y, MENU_GAMESCREEN_WIDTH, MENU_GAMESCREEN_HEIGHT);
	Sprite_SetColorDefault();


	////������

	Sprite_SetColor(255, 0, 0, 255);
	if (exp_page == RULE)writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 50, "���[�����߂�", 32, 32);
	else if (exp_page == SOUSA)writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 50, "���������߂�", 32, 32);


	char buf[128], buf2[128], title[64];

	Sprite_SetColor(0, 0, 0, 255);
	switch (select_game) {

		case SHOOTINGBATTLE_STAGE:

			if (exp_page == RULE) {

				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "�����Ă��˂���Ă��܂����Ă悤", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "���񂢂��", 32, 32);
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 5, SCREEN_HEIGHT - 250 + 130, "2", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 6, SCREEN_HEIGHT - 250 + 130, "�������Ă�΂���", 32, 32);

			}
			else if (exp_page == SOUSA) {
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "WASD:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 5, SCREEN_HEIGHT - 250 + 90, "���ǂ�", 32, 32);
			
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "Space:", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 130, "�܂��������Ⴐ��", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 170, "Space", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 2.5f, SCREEN_HEIGHT - 250 + 170, "�Ȃ�����", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 6.5f, SCREEN_HEIGHT - 250 + 170, ":", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 7.0f, SCREEN_HEIGHT - 250 + 170, "��܂Ȃ�ɂ��Ⴐ��", 32, 32);

			
			}
			sprintf(title, "�˂炢�����o�g��");

			break;

		case BATTLE_STAGE:

			if (exp_page == RULE) {

				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "�����ĂɂԂ����Ă��Ƃɂ��Ƃ�", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "�������̂ЂƂ�ɂ̂���΂���", 32, 32);


			}
			else if (exp_page == SOUSA) {
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "WASD:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 5, SCREEN_HEIGHT - 250 + 90, "���ǂ�", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "Space:", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 130, "������", 32, 32);


			}
			sprintf(title, "�����ƂT�o�C�o��");

			break;

		case BATTLEDOME_STAGE:

			if (exp_page == RULE) {

				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "���܂��͂˂������ăS�[�����܂���", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "�Ă�̂����Ȃ��ЂƂ�����", 32, 32);


			}
			else if (exp_page == SOUSA) {
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "AD:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 90, "���ǂ�", 32, 32);


			}

			sprintf(title, "�}���`�F�A�z�b�P�[");

			break;

		case RACE_STAGE:

			if (exp_page == RULE) {

				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "�ڂ��������Ȃ���S�[���ɂ�����", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "������", 32, 32);
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 130, "5", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 4, SCREEN_HEIGHT - 250 + 130, "���イ����΂���", 32, 32);


			}
			else if (exp_page == SOUSA) {
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "WASD:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 5, SCREEN_HEIGHT - 250 + 90, "���ǂ�", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "Space:", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 130, "�܂��������Ⴐ��", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 170, "Space", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 2.5f, SCREEN_HEIGHT - 250 + 170, "�Ȃ�����", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 6.5f, SCREEN_HEIGHT - 250 + 170, ":", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 7.0f, SCREEN_HEIGHT - 250 + 170, "�΂����񂹂���", 32, 32);

			}


			sprintf(title, "�΂��͂��[�X");

			break;

		case SURVIVAL_STAGE:

			if (exp_page == RULE) {

				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "������ʂ��Ă��񂿂��ӂ₻��", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "���񂿂������΂񂨂����ЂƂ�����", 32, 32);

			}
			else if (exp_page == SOUSA) {
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "WASD:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 5, SCREEN_HEIGHT - 250 + 90, "���ǂ�", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "Space:", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 130, "��܂Ȃ�ɂ��Ⴐ��", 32, 32);

			}

			sprintf(title, "����ʂ肶��Ƃ�");

			break;

		case ACTION_RACE_STAGE:

			if (exp_page == RULE) {

				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "���܂��Ƃ��ă|�C���g�����߂悤", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "�|�C���g�������΂񂨂����ЂƂ�����", 32, 32);

			}
			else if (exp_page == SOUSA) {
				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 90, "WASD:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 5, SCREEN_HEIGHT - 250 + 90, "���ǂ�", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 130, "Space:", 16, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 3, SCREEN_HEIGHT - 250 + 130, "���傤���傤", 32, 32);

				writestring::AlphabetFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10, SCREEN_HEIGHT - 250 + 170, "Q E:", 32, 32);
				writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 4, SCREEN_HEIGHT - 250 + 170, "�J���������Ă�", 32, 32);

			}
			sprintf(title, "�������ł��܂���");

			break;

		default:

			sprintf(buf,"");
			sprintf(title, "");
			return;
			break;
	}
	
	//�^�C�g��
	Sprite_SetColor(100, 149, 237, 255);
	writestring::JapaneseFont_Draw(MENU_GAMEEXPLAIN_STARTPOS_X + 10 + 32 * 4, SCREEN_HEIGHT - 250 + 10, title, 32, 32);




	Sprite_SetColorDefault();

}


void menu::PageChange(int num) {

	//SE
	PlaySound(SOUND_LABEL_SE_PAGE);

	exp_page += num;

	if (exp_page < 0) exp_page = PAGE_MAX - 1;
	else if (exp_page >= PAGE_MAX) exp_page = 0;

}

void menu::SetMode(int set_mode) {

	mode = set_mode;

}

int menu::GetMode() {

	return mode;
}

int menu::GetSelectGame() {

	return select_game;
}

void menu::SetSelectGame(int num) {

	select_game = num;
}