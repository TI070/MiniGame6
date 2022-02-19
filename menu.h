#pragma once
#ifndef MENU_H
#define MENU_H

#include "stage.h"

class menu {
private:
	menu();

	static int FrameTexId;
	static int BgTexId;
	static int GameScreenTexId[STAGE_MAX];
	static int exp_page;

	static int select_game;
	static int mode;

public:

	static void Initialize();
	static void Draw();
	static void Update();
	static void SelectChange(int num);
	static void SelectPlus(int num);

	static void GameSelect();
	static int GetSelectGame();
	static void SetSelectGame(int num);

	static void MenuExplainDraw();


	static void PageChange(int num);

	static void SetMode(int set_mode);
	static int GetMode();

};
#endif