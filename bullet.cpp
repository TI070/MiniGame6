#include "bullet.h"
#include "texture.h"
#include "game_player.h"
#include "billboard.h"
#include "useful.h"
#include "stage.h"
#include "wall.h"
#include <stdio.h>

#define SHOT_SPEED (0.2f)
#define RADIATION_UP_SPEED (1.0f)

#define SENSOR_SIZE (2.0f)

#define RACEMINEBULLET_LIFE (15 * 60)

bullet::bullet() {



}

bullet::bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer,int owner) {

	char buf[128];
	sprintf(buf, "asset/bullet%d.png", pShotPlayer->number + 1);
	BulletTexId = Texture_SetTextureLoadFile(buf);
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	
	//速度生成
	velocity = pShotPlayer->GetFront();
	D3DXVec3Normalize(&velocity, &velocity);
	velocity *= SHOT_SPEED;
	size = set_size;
	pPlayer = pShotPlayer;

}

void bullet::Initialize() {


}

void bullet::Draw() {

	BillBorad_Draw(BulletTexId,pos.x, pos.y, pos.z,size,size);
}

void bullet::Update() {

	pos += velocity;
}

D3DXVECTOR3 bullet::GetPos() {
	return pos;
}

D3DXVECTOR3 bullet::GetSize() {
	return D3DXVECTOR3(size,size,size);
}

int bullet::GetBulletOwner() {
	return bullet_owner;
}

player* bullet::GetBulletPlayer() {
	return pPlayer;
}

int bullet::GetBulletType() {
	return BATTLE_BULLET;

}

void bullet::SetVelocity(D3DXVECTOR3 set_v) {
	velocity = set_v;

}



radiation_bullet::radiation_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner) {

	char buf[128];
	sprintf(buf, "asset/bullet%d.png", pShotPlayer->number + 1);
	BulletTexId = Texture_SetTextureLoadFile(buf);
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;

	//速度生成
	velocity = pShotPlayer->GetFront();
	D3DXVec3Normalize(&velocity, &velocity);
	velocity.y = RADIATION_UP_SPEED;
	velocity *= SHOT_SPEED;
	size = set_size;
	pPlayer = pShotPlayer;
}

void radiation_bullet::Update() {

	velocity.y -= GRAVITY_SCALE * 1 / 5;
	pos += velocity / 2;
}

int radiation_bullet::GetBulletType() {
	return RADIATION_BULLET;

}


//////

ball_bullet::ball_bullet(D3DXVECTOR3 set_pos, float set_size) {

	BulletTexId = Texture_SetTextureLoadFile("asset/bullet.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	size = set_size;

	D3DXVECTOR2 vec{1,0};

	vec = VectorRotate(vec,rand() % 360);

	velocity.x = vec.x * MOVEBALL_SPEED;
	velocity.z = vec.y * MOVEBALL_SPEED;
	velocity.y = 0.0f;

}

int ball_bullet::GetBulletType() {
	return BALL_BULLET;
}

ball3_bullet::ball3_bullet(D3DXVECTOR3 set_pos, float set_size) {

	BulletTexId = Texture_SetTextureLoadFile("asset/bullet_3s.png");
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	size = set_size;

	D3DXVECTOR2 vec{ 1,0 };

	vec = VectorRotate(vec, rand() % 360);

	velocity.x = vec.x * MOVEBALL_SPEED;
	velocity.z = vec.y * MOVEBALL_SPEED;
	velocity.y = 0.0f;

}

int ball3_bullet::GetBulletType() {
	return BALL3_BULLET;
}



race_bullet::race_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner) {

	char buf[128];
	sprintf(buf, "asset/bullet%d.png", pShotPlayer->number + 1);
	BulletTexId = Texture_SetTextureLoadFile(buf);
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	size = set_size;

	//速度生成
	velocity = pShotPlayer->GetFront();
	D3DXVec3Normalize(&velocity, &velocity);
	velocity *= SHOT_SPEED;
	size = set_size;
	pPlayer = pShotPlayer;

	life = 10000;

}

int race_bullet::GetBulletType() {
	return RACE_BULLET;
}

racemine_bullet::racemine_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner) {

	char buf[128];
	sprintf(buf, "asset/bullet%d.png", pShotPlayer->number + 1);
	BulletTexId = Texture_SetTextureLoadFile(buf);
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;
	size = set_size;

	//速度生成
	velocity = {0,0,0};
	size = set_size;
	pPlayer = pShotPlayer;

	life = RACEMINEBULLET_LIFE;

}

void racemine_bullet::Update() {

	life--;
}

int racemine_bullet::GetBulletType() {
	return RACE_BULLET;
}


sensor_bullet::sensor_bullet(D3DXVECTOR3 set_pos, float set_size, player* pShotPlayer, int owner) {

	char buf[128];
	sprintf(buf, "asset/bullet%d.png", pShotPlayer->number + 1);
	BulletTexId = Texture_SetTextureLoadFile(buf);
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	size = SENSOR_SIZE;

	//前方に出す
	velocity = { 0,0,0 };
	pPlayer = pShotPlayer;

	D3DXVECTOR3 o_front = pPlayer->GetFront();
	o_front.y += 0.5f;

	pos = pPlayer->GetPos() + o_front;

}

void sensor_bullet::Update() {

	//位置更新
	pos = pPlayer->GetPos() + pPlayer->GetFront();

	if (stage::stage_n == BATTLEDOME_STAGE) {

		if(pPlayer->number == 1) pos = pPlayer->pPlayerWall->GetPos() + D3DXVECTOR3(0,0,-0.5f);
		else if (pPlayer->number == 2) pos = pPlayer->pPlayerWall->GetPos() + D3DXVECTOR3(-0.5f, 0, 0);
		else if (pPlayer->number == 3) pos = pPlayer->pPlayerWall->GetPos() + D3DXVECTOR3(0.5f, 0, 0);


	}
	else if (stage::stage_n == SHOOTINGBATTLE_STAGE) {

		pos = pPlayer->GetPos();
		size = SENSOR_SIZE * 5;

	}
	else if (stage::stage_n == BATTLE_STAGE) {

	
	}
	else if (stage::stage_n == RACE_STAGE) {


	}
	else if (stage::stage_n == SURVIVAL_STAGE) {


	}




}

void sensor_bullet::Draw() {

	//BillBorad_Draw(BulletTexId, pos.x, pos.y, pos.z, size, size);

}


int sensor_bullet::GetBulletType() {

	return SENSOR_BULLET;
}

point_bullet::point_bullet(D3DXVECTOR3 set_pos, float set_size) {

	char buf[128];
	sprintf(buf, "asset/bullet1.png");
	BulletTexId = Texture_SetTextureLoadFile(buf);
	if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗", "エラー", MB_OK);
	}

	pos = set_pos;

	//速度生成
	size = set_size;


}

void point_bullet::Update() {


}

int point_bullet::GetBulletType() {

	return POINT_BULLET;
}

