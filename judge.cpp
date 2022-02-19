
#include "judge.h"
#include "wall_manager.h"
#include "bullet_manager.h"
#include "game_player.h"
#include "player_manager.h"
#include "effect_manager.h"
#include "useful.h"
#include "stage.h"
#include "config.h"
#include "sound.h"
#include <stdio.h>

void judge::Initilize() {


}

//ゲームの更新
void judge::Update() {

	

	JudgePlayerandWall();
	JudgeBulletandWall();
	JudgePlayerandPlayer();
	JudgeBulletandPlayer();
	JudgeBulletandBullet();
	JudgeBulletandSensor();
}

void judge::JudgePlayerandWall() {

	player* pPlayer;

	for (int j = 0; j < PLAYER_MAX; j++) {

		pPlayer = player_manager::GetPlayer(j);

		if (pPlayer == NULL) continue;

		for (int i = 0; i < WALL_MAX; i++) {

			wall* pWall;

			pWall = wall_manager::GetWall(i);

			if (pWall == NULL) continue;

			//背景なら処理しない
			if (pWall->GetWallType() == BACKGROUND_WALL) continue;

			//壁の向きごとに判定
			switch (pWall->walltype)
			{
			case XY_FIELD:

				if (pPlayer->GetPos().z > pWall->GetPos().z - MASS_SIZE / 2 && pPlayer->GetPos().z < pWall->GetPos().z + MASS_SIZE / 2 &&
					pPlayer->GetPos().x > pWall->GetPos().x - pWall->height / 2 * MASS_SIZE && pPlayer->GetPos().x < pWall->GetPos().x + pWall->height / 2 * MASS_SIZE &&
					pPlayer->GetPos().y > pWall->GetPos().y - pWall->width / 2 * MASS_SIZE && pPlayer->GetPos().y < pWall->GetPos().y + pWall->width / 2 * MASS_SIZE
					) {

					if (stage::stage_n == ACTION_RACE_STAGE) {//特殊処理
						if (pPlayer->GetPos().z > pWall->GetPos().z) continue;

					}

					if (stage::stage_n == SURVIVAL_STAGE) {//特殊処理
						//if (pPlayer->GetPos().z > pWall->GetPos().z + MASS_SIZE/10) continue;

					}


					pPlayer->UndoPosX();
					pPlayer->UndoPosZ();

				}

				break;

			case YZ_FIELD:

				if (pPlayer->GetPos().x > pWall->GetPos().x - MASS_SIZE / 2 && pPlayer->GetPos().x < pWall->GetPos().x + MASS_SIZE / 2 &&
					pPlayer->GetPos().z > pWall->GetPos().z - pWall->width / 2 * MASS_SIZE && pPlayer->GetPos().z < pWall->GetPos().z + pWall->width / 2 * MASS_SIZE &&
					pPlayer->GetPos().y > pWall->GetPos().y - pWall->height / 2 * MASS_SIZE && pPlayer->GetPos().y < pWall->GetPos().y + pWall->height / 2 * MASS_SIZE
					) {

					pPlayer->UndoPosX();
					pPlayer->UndoPosZ();

				}

				break;

			case XZ_FIELD:

				if (pPlayer->GetPos().y > pWall->GetPos().y - MASS_SIZE / 10 && pPlayer->GetPos().y < pWall->GetPos().y + MASS_SIZE / 10 &&
					pPlayer->GetPos().z > pWall->GetPos().z - pWall->width / 2 * MASS_SIZE && pPlayer->GetPos().z < pWall->GetPos().z + pWall->width / 2 * MASS_SIZE &&
					pPlayer->GetPos().x > pWall->GetPos().x - pWall->height / 2 * MASS_SIZE && pPlayer->GetPos().x < pWall->GetPos().x + pWall->height / 2 * MASS_SIZE
					) {

					pPlayer->UndoPosY();
					pPlayer->isJump = false;

					//色を塗る
					if (stage::stage_n == SURVIVAL_STAGE) {

						pWall->Action(pPlayer);

					}



				}

				break;


			}

		}
	}
}

void judge::JudgeBulletandWall() {

	wall* pWall;
	bullet* pBullet;

	for (int i = 0; i < WALL_MAX; i++) {

		pWall = wall_manager::GetWall(i);

		if (pWall == NULL) continue;

		//背景なら処理しない
		if (pWall->GetWallType() == BACKGROUND_WALL) continue;

		for (int j = 0; j < BULLET_MAX; j++) {

			pBullet = bullet_manager::GetBullet(j);

			if (pBullet == NULL) continue;

			//センサーは処理しない
			if (pBullet->GetBulletType() == SENSOR_BULLET) continue;

			//壁の向きごとに判定
			switch (pWall->walltype)
			{
			case XY_FIELD:

				if (pBullet->GetPos().z > pWall->GetPos().z - MASS_SIZE / 2 && pBullet->GetPos().z < pWall->GetPos().z + MASS_SIZE / 2 &&
					pBullet->GetPos().x > pWall->GetPos().x - pWall->height / 2 * MASS_SIZE && pBullet->GetPos().x < pWall->GetPos().x + pWall->height / 2 * MASS_SIZE &&
					pBullet->GetPos().y > pWall->GetPos().y - pWall->width / 2 * MASS_SIZE && pBullet->GetPos().y < pWall->GetPos().y + pWall->width / 2 * MASS_SIZE
					) {

					if (stage::stage_n == SURVIVAL_STAGE) {

						bullet_manager::DestroyBullet(j);

						continue;
					}

					if (pBullet->GetBulletType() == BATTLE_BULLET || pBullet->GetBulletType() == RACE_BULLET || pBullet->GetBulletType() == RADIATION_BULLET) {//攻撃用弾なら消す

						//爆発エフェクトの生成
						effect_manager::SetExplosionEffect(pBullet->GetPos(),EXPLOSION_SIZE,0);


						bullet_manager::DestroyBullet(j);
					}
					else if (pBullet->GetBulletType() == BALL_BULLET || pBullet->GetBulletType() == BALL3_BULLET) {//ボール用弾なら反射する
						

						//ベクトルを生成
						D3DXVECTOR2 Vec;
						D3DXVECTOR3 Velocity;

						Vec = PostoPosVector_Normalize({ pBullet->GetPos().x,pBullet->GetPos().z }, { pWall->GetPos().x,pWall->GetPos().z });

						Velocity.x = -Vec.x * MOVEBALL_SPEED;
						Velocity.z = -Vec.y * MOVEBALL_SPEED;
						Velocity.y = 0;


						pBullet->SetVelocity(Velocity);

					}
				}

				break;

			case YZ_FIELD:

				if (pBullet->GetPos().x > pWall->GetPos().x - MASS_SIZE / 2 && pBullet->GetPos().x < pWall->GetPos().x + MASS_SIZE / 2 &&
					pBullet->GetPos().z > pWall->GetPos().z - pWall->width / 2 * MASS_SIZE && pBullet->GetPos().z < pWall->GetPos().z + pWall->width / 2 * MASS_SIZE &&
					pBullet->GetPos().y > pWall->GetPos().y - pWall->height / 2 * MASS_SIZE && pBullet->GetPos().y < pWall->GetPos().y + pWall->height / 2 * MASS_SIZE
					) {

					if (stage::stage_n == SURVIVAL_STAGE) {

						bullet_manager::DestroyBullet(j);

						continue;
					}

					if (pBullet->GetBulletType() == BATTLE_BULLET || pBullet->GetBulletType() == RACE_BULLET || pBullet->GetBulletType() == RADIATION_BULLET) {//攻撃用弾なら消す
						//爆発エフェクトの生成
						effect_manager::SetExplosionEffect(pBullet->GetPos(), EXPLOSION_SIZE, 0);
						
						bullet_manager::DestroyBullet(j);
					}
					else if (pBullet->GetBulletType() == BALL_BULLET || pBullet->GetBulletType() == BALL3_BULLET) {//ボール用弾なら反射する


						//ベクトルを生成
						D3DXVECTOR2 Vec;
						D3DXVECTOR3 Velocity;

						Vec = PostoPosVector_Normalize({ pBullet->GetPos().x,pBullet->GetPos().z }, { pWall->GetPos().x,pWall->GetPos().z });

						Velocity.x = -Vec.x * MOVEBALL_SPEED;
						Velocity.z = -Vec.y * MOVEBALL_SPEED;
						Velocity.y = 0;


						pBullet->SetVelocity(Velocity);

					}
				}

				break;


			case XZ_FIELD:


				if (pBullet->GetPos().y > pWall->GetPos().y - MASS_SIZE / 2 && pBullet->GetPos().y < pWall->GetPos().y + MASS_SIZE / 2 &&
					pBullet->GetPos().z > pWall->GetPos().z - pWall->width / 2 * MASS_SIZE && pBullet->GetPos().z < pWall->GetPos().z + pWall->width / 2 * MASS_SIZE &&
					pBullet->GetPos().x > pWall->GetPos().x - pWall->height / 2 * MASS_SIZE && pBullet->GetPos().x < pWall->GetPos().x + pWall->height / 2 * MASS_SIZE
					) {

					if (pBullet->GetBulletType() == BATTLE_BULLET || pBullet->GetBulletType() == RACE_BULLET || pBullet->GetBulletType() == RADIATION_BULLET) {//攻撃用弾なら消す
						//爆発エフェクトの生成
						//effect_manager::SetExplosionEffect(pBullet->GetPos(), EXPLOSION_SIZE, 0);

						//色を塗る
						if (stage::stage_n == SURVIVAL_STAGE) {

							pWall->Action(pBullet->GetBulletPlayer());

						}

						bullet_manager::DestroyBullet(j);
					}
					else if (pBullet->GetBulletType() == BALL_BULLET) {//ボール用弾なら反射する


					}
				}

				break;



			}
		}
	}

}

void judge::JudgePlayerandPlayer() {

	player* pPlayer1;
	player* pPlayer2;

	for (int i = 0; i < PLAYER_MAX; i++) {

		pPlayer1 = player_manager::GetPlayer(i);

		if (pPlayer1 == NULL) continue;

		for (int j = i + 1; j < PLAYER_MAX; j++) {

			pPlayer2 = player_manager::GetPlayer(j);

			if (pPlayer2 == NULL) continue;

			if (pPlayer1->GetPos().z > pPlayer2->GetPos().z - pPlayer2->GetSize().z && pPlayer1->GetPos().z < pPlayer2->GetPos().z + pPlayer2->GetSize().z &&
				pPlayer1->GetPos().x > pPlayer2->GetPos().x - pPlayer2->GetSize().x && pPlayer1->GetPos().x < pPlayer2->GetPos().x + pPlayer2->GetSize().x &&
				pPlayer1->GetPos().y > pPlayer2->GetPos().y - pPlayer2->GetSize().y && pPlayer1->GetPos().y < pPlayer2->GetPos().y + pPlayer2->GetSize().y
				) {

				////衝突時にノックバック

				//playerのノックバックベクトルを生成
				D3DXVECTOR2 KB_Vec;
				KB_Vec = PostoPosVector_Normalize({pPlayer1->GetPos().x,pPlayer1->GetPos().z}, { pPlayer2->GetPos().x,pPlayer2->GetPos().z });

				//当たる相手の速度によってノックバック速度を設定
				float KB_Vel1, KB_Vel2;
				KB_Vel1 = pPlayer2->GetSpeed();
				KB_Vel2 = pPlayer1->GetSpeed();

				//最低値を下回ったら補正
				if (KB_Vel1 < KNOCKBACK_SPEED_MIN) KB_Vel1 = KNOCKBACK_SPEED_MIN;
				if (KB_Vel2 < KNOCKBACK_SPEED_MIN) KB_Vel2 = KNOCKBACK_SPEED_MIN;

				//KB_Vec *= 0.05f;

				pPlayer1->SetKnockbackDirection({-KB_Vec.x * KB_Vel1,0,-KB_Vec.y * KB_Vel1});
				pPlayer2->SetKnockbackDirection({ KB_Vec.x * KB_Vel2,0, KB_Vec.y * KB_Vel2});

			}

		}
	}
}

void judge::JudgeBulletandPlayer() {

	bullet* pBullet;
	player* pPlayer;

	for (int i = 0; i < BULLET_MAX; i++) {

		pBullet = bullet_manager::GetBullet(i);

		if (pBullet == NULL) continue;

		//センサーは処理しない
		if (pBullet->GetBulletType() == SENSOR_BULLET) continue;

		//
		if (stage::stage_n == BATTLEDOME_STAGE) continue;

		for (int j = 0; j < PLAYER_MAX; j++) {

			pPlayer = player_manager::GetPlayer(j);

			if (pPlayer == NULL) continue;
			if (pPlayer == pBullet->GetBulletPlayer()) continue;

			//無敵時間中なら判定しない
			if (pPlayer->invincible_time > 0) continue;

			if (pPlayer->GetPos().z > pBullet->GetPos().z - pBullet->GetSize().z && pPlayer->GetPos().z < pBullet->GetPos().z + pBullet->GetSize().z &&
				pPlayer->GetPos().x > pBullet->GetPos().x - pBullet->GetSize().x && pPlayer->GetPos().x < pBullet->GetPos().x + pBullet->GetSize().x &&
				pPlayer->GetPos().y > pBullet->GetPos().y - pBullet->GetSize().y && pPlayer->GetPos().y < pBullet->GetPos().y + pBullet->GetSize().y
				) {


				if (stage::stage_n == ACTION_RACE_STAGE) {
					pPlayer->point++;
				}
				else {
					//爆発エフェクトの生成
					effect_manager::SetExplosionEffect(pBullet->GetPos(), EXPLOSION_SIZE, 0);
				}

				if (stage::stage_n == SHOOTINGBATTLE_STAGE) {

					//SE
					PlaySound(SOUND_LABEL_SE_HITGETPOINT);

					//ポイント取得
					pBullet->GetBulletPlayer()->PointUp(pPlayer->number);	

					//無敵時間セット
					pPlayer->SetInvincibleTime();

				}
				else if(stage::stage_n == RACE_STAGE){
					//ノックバック
				
					//ベクトルを生成
					D3DXVECTOR2 KB_Vec;
					KB_Vec = PostoPosVector_Normalize({ pPlayer->GetPos().x,pPlayer->GetPos().z }, { pBullet->GetPos().x,pBullet->GetPos().z });

					KB_Vec *= -0.1f;

					pPlayer->SetKnockbackDirection({ KB_Vec.x,0,KB_Vec.y });

				}



				//衝突したら弾を消す
				bullet_manager::DestroyBullet(i);
			

				if (stage::stage_n == ACTION_RACE_STAGE) {
					float setx, sety, setz;
					setx = (rand() % 40 - 20) / 2;
					sety = (rand() % 40 - 20) / 2;
					setz = (rand() % 40 - 20) / 2;


					bullet_manager::SetPointBullet({ setx,sety,setz }, 0);
				}

			}





		}
		
	}

}


void judge::JudgeBulletandBullet() {

	bullet* pBullet1;
	bullet* pBullet2;

	for (int i = 0; i < BULLET_MAX - 1; i++) {

		pBullet1 = bullet_manager::GetBullet(i);

		if (pBullet1 == NULL) continue;

		for (int j = i + 1; j < BULLET_MAX; j++) {

			pBullet2 = bullet_manager::GetBullet(j);

			if (pBullet2 == NULL) continue;

			
			if (pBullet2->GetPos().z > pBullet1->GetPos().z - pBullet1->GetSize().z && pBullet2->GetPos().z < pBullet1->GetPos().z + pBullet1->GetSize().z &&
				pBullet2->GetPos().x > pBullet1->GetPos().x - pBullet1->GetSize().x && pBullet2->GetPos().x < pBullet1->GetPos().x + pBullet1->GetSize().x &&
				pBullet2->GetPos().y > pBullet1->GetPos().y - pBullet1->GetSize().y && pBullet2->GetPos().y < pBullet1->GetPos().y + pBullet1->GetSize().y
				) {

				//ボール同士の衝突
				if (pBullet1->GetBulletType() == BALL_BULLET || pBullet1->GetBulletType() == BALL3_BULLET) {

					if (pBullet2->GetBulletType() == BALL_BULLET || pBullet2->GetBulletType() == BALL3_BULLET) {


						////衝突時にノックバック

						//playerのノックバックベクトルを生成
						D3DXVECTOR2 KB_Vec;
						KB_Vec = PostoPosVector_Normalize({ pBullet1->GetPos().x,pBullet1->GetPos().z }, { pBullet2->GetPos().x,pBullet2->GetPos().z });

						//当たる相手の速度によってノックバック速度を設定
						float KB_Vel1, KB_Vel2;
						KB_Vel1 = MOVEBALL_SPEED;
						KB_Vel2 = MOVEBALL_SPEED;


						pBullet1->SetVelocity({ -KB_Vec.x * KB_Vel1,0, -KB_Vec.y * KB_Vel1 });
						pBullet2->SetVelocity({ KB_Vec.x * KB_Vel2,0, KB_Vec.y * KB_Vel2 });


					}

				}

			}



		}


	}

}

void judge::JudgeBulletandSensor() {

	bullet* pSensor;
	bullet* pBullet;

	for (int i = 0; i < BULLET_MAX; i++) {

		pSensor = bullet_manager::GetBullet(i);

		if (pSensor == NULL) continue;
		if (pSensor->GetBulletType() != SENSOR_BULLET) continue;
		if (pSensor->GetBulletPlayer()->number == 0) continue;

		for (int j = 0; j < BULLET_MAX; j++) {

			pBullet = bullet_manager::GetBullet(j);

			if (pBullet == NULL) continue;
			if (pBullet->GetBulletType() == SENSOR_BULLET) continue;


			if (pBullet->GetPos().z > pSensor->GetPos().z - pSensor->GetSize().z / 2 && pBullet->GetPos().z < pSensor->GetPos().z + pSensor->GetSize().z / 2 &&
				pBullet->GetPos().x > pSensor->GetPos().x - pSensor->GetSize().x / 2 && pBullet->GetPos().x < pSensor->GetPos().x + pSensor->GetSize().x / 2 &&
				pBullet->GetPos().y > pSensor->GetPos().y - pSensor->GetSize().y / 2 && pBullet->GetPos().y < pSensor->GetPos().y + pSensor->GetSize().y / 2
				) {


				if (stage::stage_n == SHOOTINGBATTLE_STAGE) {

					//弾を避ける
					player* pP;
					pP = pSensor->GetBulletPlayer();
					float setx, setz;
					D3DXVECTOR3 setvec{};

					if (pP->GetPos().x < pBullet->GetPos().x) {//弾が右にあったら左に逃げる
						setx = -1;
					}
					else {
						setx = 1;
					}

					if (pP->GetPos().z < pBullet->GetPos().z) {//弾が前にあったら後ろに逃げる
						setz = -1;
					}
					else {
						setz = 1;
					}
					setvec = D3DXVECTOR3(setx,0,setz);

					pSensor->GetBulletPlayer()->PlayerRotation(setvec);



				}
				else if (stage::stage_n == BATTLEDOME_STAGE) {

					//弾が前にあれば停止
					pSensor->GetBulletPlayer()->CPUmoveDir = NONE;

				}
				else if (stage::stage_n == RACE_STAGE) {

					//弾を避ける
					player* pP;
					pP = pSensor->GetBulletPlayer();
					float setx, setz;
					D3DXVECTOR3 setvec{};

					if (pP->GetPos().x < pBullet->GetPos().x) {//弾が右にあったら左に逃げる
						setx = -1;
					}
					else {
						setx = 1;
					}

					if (pP->GetPos().z < pBullet->GetPos().z) {//弾が前にあったら後ろに逃げる
						setz = -1;
					}
					else {
						setz = 1;
					}
					setvec = D3DXVECTOR3(setx, 0, setz);

					pSensor->GetBulletPlayer()->PlayerRotation(setvec);



				}



			}






		}


	}

}

