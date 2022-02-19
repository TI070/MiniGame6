
#include "bullet_manager.h"
#include "game_player.h"
#include "stage.h"

#define BULLET_SIZE (0.50f)

bullet* bullet_manager::pBullet[BULLET_MAX];

void bullet_manager::Initialize() {

	for (int i = 0; i < BULLET_MAX; i++) {

		pBullet[i] = NULL;

	}
}

void bullet_manager::Draw() {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] == NULL) continue;

		pBullet[i]->Draw();

	}
}

void bullet_manager::Update() {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] == NULL) continue;

		pBullet[i]->Update();

		//ì¡éÍèàóù
		if (stage::stage_n == RACE_STAGE) {
			CheckMineBulletLife();
		}

	}
}


void bullet_manager::SetBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		pBullet[i] = new bullet(set_pos, BULLET_SIZE,pShotPlayer, owner);

		break;
	}

}

void bullet_manager::SetRadiationBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		pBullet[i] = new radiation_bullet(set_pos, BULLET_SIZE, pShotPlayer, owner);

		break;
	}

}

void bullet_manager::SetBallBullet(D3DXVECTOR3 set_pos,int type) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		switch (type)
		{
		case 0:
			pBullet[i] = new ball_bullet(set_pos, BULLET_SIZE);
			break;

		case 1:
			pBullet[i] = new ball3_bullet(set_pos, BULLET_SIZE);
			break;


		}
		

		break;
	}
}

void bullet_manager::SetRaceBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		pBullet[i] = new race_bullet(set_pos, BULLET_SIZE, pShotPlayer, owner);

		break;
	}
}

void bullet_manager::SetRaceMineBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		pBullet[i] = new racemine_bullet(set_pos, BULLET_SIZE, pShotPlayer, owner);

		break;
	}
}

void bullet_manager::SetSensorBullet(D3DXVECTOR3 set_pos, player* pShotPlayer, int owner) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		pBullet[i] = new sensor_bullet(set_pos, BULLET_SIZE, pShotPlayer, owner);

		break;
	}
}

void bullet_manager::SetPointBullet(D3DXVECTOR3 set_pos, int type) {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] != NULL) continue;

		switch (type)
		{
		case 0:
			pBullet[i] = new point_bullet(set_pos, BULLET_SIZE);
			break;


		}


		break;
	}

}


bullet* bullet_manager::GetBullet(int index) {

	return pBullet[index];
}

void bullet_manager::DestroyBullet(int index) {

	delete pBullet[index];
	pBullet[index] = NULL;
}

void bullet_manager::AllDestroyBullet() {

	for (int i = 0; i < BULLET_MAX; i++) {

		DestroyBullet(i);
	}

}

void bullet_manager::CheckMineBulletLife() {

	for (int i = 0; i < BULLET_MAX; i++) {

		if (pBullet[i] == NULL) continue;

		if (pBullet[i]->life <= 0) {
			DestroyBullet(i);
		}

	}

}
