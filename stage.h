#pragma once
#ifndef STAGE_H
#define STAGE_H

#include <d3dx9.h>

#define SHOOTINGBATTLE_STAGE_SIZE (20)
#define BATTLEDOME_STAGE_SIZE (15)
#define BATTLE_STAGE_SIZE (12)
#define ACTIONRACE_STAGE_SIZE (25)
#define RACE_STAGE_SIZE_X (20)
#define RACE_STAGE_SIZE_Y (10)
#define SURVIVAL_STAGE_SIZE (15)

//enum STAGE_TYPE {
//	
//	SHOOTINGBATTLE_STAGE,
//	BATTLE_STAGE,
//	ACTION_RACE_STAGE,
//	BATTLEDOME_STAGE,
//	SURVIVAL_STAGE,
//	RACE_STAGE,
//	STAGE_MAX
//};

enum STAGE_TYPE {

	SHOOTINGBATTLE_STAGE,
	BATTLE_STAGE,
	RACE_STAGE,
	BATTLEDOME_STAGE,
	SURVIVAL_STAGE,
	ACTION_RACE_STAGE,
	STAGE_MAX
};

enum COURSE_CHECKPOINT {

	HOME_STRETCH_RIGHT,
	FIRST_CORNER,
	BACK_STRETCH,
	THIRD_CORNER,
	HOME_STRETCH_LEFT

};


class player;

class stage {
private:
	stage();

	static player* pPlayer[4];
	
	static bool minigame_finish;
public:

	static void Initialize();
	static void SetStage(int stage_num);
	static void Draw();
	static void Update();
	static void FallDeathCheck();
	static void FallDeath(player* pP,int i);
	static bool Finish_Check();
	static bool OtherDeathCheck(int i);//自分以外が生存しているかチェック
	static void LapCheck();

	static int stage_n;
	static int game_timer;
	static int sub_timer;

	static void BattleDomePointCheck();
	static void TimerCheck();

	static void ActionRaceStageSet();

	static void UnInit();
};
#endif