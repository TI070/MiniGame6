
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
	
	//予約
	reserveScene = SetScene;

	//フェード開始
	Fade_Start(FADE_OUT,1,1,1,30);

}

void scene_manager::ChangeSceneFade() {

	if(mode == 1){//フェードイン
		if (CurrentScene != reserveScene) {//一致していなかったら

			Fade_Update();
			if (!Fade_IsFade()) {//完了

				//if (CurrentScene == TITLE) stage::UnInit();

				CurrentScene = reserveScene;
				Fade_Start(FADE_IN,1,1,1,30);
				mode = 2;

			}
		}
	}
	else if (mode == 2) {//フェードアウト

		Fade_Update();
		if (!Fade_IsFade()) {//完了

			mode = 0;
		}

	}


}