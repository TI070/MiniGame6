#pragma once
#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <d3dx9.h>
#include "bullet.h"

#define BULLET_MAX (128)

class bullet_manager {
private:
	bullet_manager();
	static bullet* pBullet[BULLET_MAX];
public:


	static void Initialize();
	static void Draw();
	static void Update();
	static void SetBullet(D3DXVECTOR3 set_pos,player* pShotPlayer, int owner);
	static void SetRadiationBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner);
	static void SetBallBullet(D3DXVECTOR3 set_pos, int type);
	static void SetRaceBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner);
	static void SetRaceMineBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner);
	static void SetSensorBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner);
	static void SetPointBullet(D3DXVECTOR3 set_pos,int type);

	static bullet* GetBullet(int index);

	static void DestroyBullet(int index);
	static void AllDestroyBullet();

	static void CheckMineBulletLife();

};
#endif