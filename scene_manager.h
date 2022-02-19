#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <d3dx9.h>

enum SCENE_KIND {
	TITLE,
	MENU,
	GAME,
	PAUSE,
	GAMEOVER,
	GAMEEND,
	RESULT,
	SCENE_MAX
};

class scene_manager {//staticƒNƒ‰ƒX

private:
	scene_manager();
	static int CurrentScene;
	static int reserveScene;
	static int mode;

public:

	static int GetCurrentScene();
	static void SetCurrentScene(int SetScene);
	static void ChangeSceneFade();


};


#endif