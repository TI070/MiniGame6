#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <d3dx9.h>
#include "meshfield.h"

#define MOVEBALL_SPEED (0.1f)


enum BULLET_TYPE{

	BATTLE_BULLET,
	RADIATION_BULLET,
	BALL_BULLET,
	BALL3_BULLET,
	RACE_BULLET,
	SENSOR_BULLET,
	POINT_BULLET

};

class player;

class bullet {
protected:

	int BulletTexId;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 velocity;
	float size;
	int bullet_owner;
	player* pPlayer;


public:
	bullet();
	bullet(D3DXVECTOR3 set_pos, float set_size,player* pShotPlayer, int owner);

	void Initialize();
	virtual void Draw();
	virtual void Update();
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetSize();
	int GetBulletOwner();
	player* GetBulletPlayer();
	virtual int GetBulletType();
	void SetVelocity(D3DXVECTOR3 set_v);

	int life;

};

class radiation_bullet:public bullet {

public:

	radiation_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner);

	void Update();
	int GetBulletType();
};

class ball_bullet :public bullet {

public:

	ball_bullet(D3DXVECTOR3 set_pos, float set_size);

	int GetBulletType();
};

class ball3_bullet :public bullet {

public:

	ball3_bullet(D3DXVECTOR3 set_pos, float set_size);

	int GetBulletType();
};

class race_bullet :public bullet {

public:

	race_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner);

	int GetBulletType();
};

class racemine_bullet :public bullet {

public:

	racemine_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner);

	void Update();
	int GetBulletType();
};

class sensor_bullet :public bullet {

public:

	sensor_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner);

	int GetBulletType();
	void Update();
	void Draw();

};

class point_bullet :public bullet {

public:

	point_bullet(D3DXVECTOR3 set_pos, float set_size);

	int GetBulletType();
	void Update();

};

#endif