#pragma once
#ifndef GAME_CPU_H
#define GAME_CPU_H

enum CPUTYPE {

	CPU_RANDOM,//�����_��
	CPU_THINK // �v�l

};


class player;
class bullet;

class game_cpu {
private:
	

	player* pPlayer;
	int cpuType;
	
	int IntervalTime;
	float add_vecx, add_vecz;

	int shot_cooltime;

	bullet* TargetBullet;
	int TargetBulletNum;

public:

	game_cpu();
	game_cpu(int type, player* pP);
	int CurrentStage;
	void Initiaize();
	void Update();
	void SetPlayer(player* pP,int type);

	void BulletSearch();

};
#endif