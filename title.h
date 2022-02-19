#pragma once
#ifndef TITLE_H
#define TITLE_H

#include "stage.h"

enum TITLE_MENU {
	FREEPLAY,
	WINPLAY,
	TITLE_MAX
};

class title {
private:

	static int TitleTexId;
	static int select;

public:

	static void Initialize();
	static void Draw();
	static void Update();
	static void MenuSelect();

};
#endif