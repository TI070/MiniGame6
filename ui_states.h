#pragma once
#ifndef UI_STATES_H
#define UI_STATES_H

#include <d3dx9.h>

class ui_states {
private:
	ui_states();

	static int FrameTexId;
	static int BgTexId;
	static int PointTexId;
	static int LifeTexId;
	static int playerBoardId[4];

public:

	static void Initialize();
	static void BattleDraw();
	static void ShootingBattleDraw();
	static void BattleDomeDraw();
	static void RaceDraw();
	static void ActionRaceDraw();
	static void MenuDraw();
	static void SurvivalDraw();

};
#endif