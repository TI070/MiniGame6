
#include "scene_manager.h"
#include "fade.h"
#include "stage.h"
#include <stdio.h>

int scene_manager::CurrentScene = 0;
int scene_manager::reserveScene;
int scene_manager::mode;

int scene_manager::GetCurrentScene() {
	return CurrentScene;
}


void scene_manager::SetCurrentScene(int SetScene) {

	mode = 1;

	//CurrentScene = SetScene;
	
	//�\��
	reserveScene = SetScene;

	//�t�F�[�h�J�n
	Fade_Start(FADE_OUT,1,1,1,30);

}

void scene_manager::ChangeSceneFade() {

	if(mode == 1){//�t�F�[�h�C��
		if (CurrentScene != reserveScene) {//��v���Ă��Ȃ�������

			Fade_Update();
			if (!Fade_IsFade()) {//����

				//if (CurrentScene == TITLE) stage::UnInit();

				CurrentScene = reserveScene;
				Fade_Start(FADE_IN,1,1,1,30);
				mode = 2;

			}
		}
	}
	else if (mode == 2) {//�t�F�[�h�A�E�g

		Fade_Update();
		if (!Fade_IsFade()) {//����

			mode = 0;
		}

	}


}