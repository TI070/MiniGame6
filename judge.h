#pragma once
#ifndef JUDGE_H
#define JUDGE_H

class wall_manager;
class game_player;

class judge {

private:

	static void JudgePlayerandWall();
	static void JudgeBulletandWall();
	static void JudgeBulletandBullet();
	static void JudgePlayerandPlayer();
	static void JudgeBulletandPlayer();
	static void JudgeBulletandSensor();

	judge();

public:

	static  void Initilize();

	//ÉQÅ[ÉÄÇÃçXêV
	static  void Update();



};

#endif