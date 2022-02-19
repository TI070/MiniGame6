
#include "game_cpu.h"
#include "stage.h"
#include "game_player.h"
#include "bullet_manager.h"
#include "config.h"
#include "stage.h"
#include <stdio.h>

#define BATTLEDOME_INTERVAL (60)
#define BATTLE_INTERVAL (6)

#define SHOT_COOLTIMEMAX (60)

game_cpu::game_cpu() {


}

game_cpu::game_cpu(int type,player* pP) {

	cpuType = type;
	pPlayer = pP;

	IntervalTime = 0;

	add_vecx = add_vecz = 0;

	shot_cooltime = 0;

	TargetBullet = NULL;
	TargetBulletNum = 99;
}


void game_cpu::Initiaize() {


}

void game_cpu::Update() {

	if (pPlayer->minigame_finish) return;

	switch (stage::stage_n) {

		case SHOOTINGBATTLE_STAGE:

			if (IntervalTime >= BATTLE_INTERVAL) {

				float target_x, target_z;
				bool accel_flag = true;

				target_x = (rand() % 21) * 0.1f - 1.0f;
				target_z = (rand() % 21) * 0.1f - 1.0f;

				////外にいる時は内側を向く
				//x
				if (pPlayer->GetPos().x > BATTLE_STAGE_SIZE / 4) {

					target_x = -1;
					accel_flag = false;
				}
				else if (pPlayer->GetPos().x < -BATTLE_STAGE_SIZE / 4) {

					target_x = 1;
					accel_flag = false;
				}

				//z
				if (pPlayer->GetPos().z > BATTLE_STAGE_SIZE / 4) {

					target_z = -1;
					accel_flag = false;
				}
				else if (pPlayer->GetPos().z < -BATTLE_STAGE_SIZE / 4) {

					target_z = 1;
					accel_flag = false;
				}

				pPlayer->PlayerRotation({ target_x,0,target_z });

				IntervalTime = 0;


			}

			//射撃
			//if (shot_cooltime >= SHOT_COOLTIMEMAX) {
			if (pPlayer->ActionCooltimeCheck()) {

				D3DXVECTOR3 s_pos = pPlayer->GetPos();
				s_pos.y = 0.5f;

				int dummy;
				dummy = rand() % 4;

				if(dummy != 0) bullet_manager::SetBullet(s_pos, pPlayer, pPlayer->number);
				else bullet_manager::SetRadiationBullet(s_pos, pPlayer, pPlayer->number);
				//shot_cooltime = 0;
				pPlayer->action_cooltime = 0;
			}

			//前進
			pPlayer->Acceleration();


			break;

		case BATTLEDOME_STAGE:

			if (IntervalTime >= BATTLEDOME_INTERVAL) {
				if (rand() % 2 == 0) {

					pPlayer->CPUmoveDir = RIGHT;

				}
				else {

					pPlayer->CPUmoveDir = LEFT;
				}

				IntervalTime = 0;
				
			}
			break;

		case BATTLE_STAGE:		

			if (IntervalTime >= BATTLE_INTERVAL) {

				float target_x, target_z;
				bool accel_flag = true;

				target_x = (rand() % 21) * 0.1f - 1.0f;
				target_z = (rand() % 21) * 0.1f - 1.0f;

				////外にいる時は内側を向く
				//x
				if (pPlayer->GetPos().x > BATTLE_STAGE_SIZE / 4) {

					target_x = -1;
					accel_flag = false;
				}
				else if (pPlayer->GetPos().x < -BATTLE_STAGE_SIZE / 4) {

					target_x = 1;
					accel_flag = false;
				}

				//z
				if (pPlayer->GetPos().z > BATTLE_STAGE_SIZE / 4) {

					target_z = -1;
					accel_flag = false;
				}
				else if (pPlayer->GetPos().z < -BATTLE_STAGE_SIZE / 4) {

					target_z = 1;
					accel_flag = false;
				}

				pPlayer->PlayerRotation({target_x,0,target_z});

				IntervalTime = 0;


				//内側にいる時に加速
				if (pPlayer->ActionCooltimeCheck()) {
					if (accel_flag) {
						pPlayer->SetHighAccel();
						pPlayer->action_cooltime = 0;
					}
				}
			}

			//前進
			pPlayer->Acceleration();


			break;

		case RACE_STAGE:

			//壁に当たった時に逆方向へ移動して避ける
			add_vecx *= 0.9f;
			add_vecz *= 0.9f;

			//コースの位置ごとに向きを変化
			if (pPlayer->avoid_flag) {

			}
			else if (pPlayer->point1 == HOME_STRETCH_RIGHT) {

				if (pPlayer->GetPos().z > -4.0f) add_vecz = -0.5f;
				else if (pPlayer->GetPos().z < -7.0f) add_vecz = 0.5f;

				pPlayer->PlayerRotation({1 + add_vecx,0,0 + add_vecz });

				//詰まっているときの回避移動
				//if (pPlayer->CheckPosAndPastPos()) {
				//	pPlayer->PlayerRotation({ 0,0,-1 });
				//}
			}
			else if (pPlayer->point1 == FIRST_CORNER) {

				if (pPlayer->GetPos().x > 9.5f) add_vecx = -0.5f;
				else if (pPlayer->GetPos().x < 6.5f) add_vecx = 0.5f;


				pPlayer->PlayerRotation({ 0 + add_vecx,0,1 + add_vecz });

				//詰まっているときの回避移動
				if (pPlayer->CheckPosAndPastPos()) {
					pPlayer->PlayerRotation({ 0,0,-1 });
				}

			}
			else if (pPlayer->point1 == BACK_STRETCH) {

				if (pPlayer->GetPos().z > 5.0f) add_vecz = -0.5f;
				else if (pPlayer->GetPos().z < 2.0f) add_vecz = 0.5f;

				pPlayer->PlayerRotation({ -1 + add_vecx,0,0 + add_vecz });

				//詰まっているときの回避移動
				if (pPlayer->CheckPosAndPastPos()) {
					pPlayer->PlayerRotation({ 1,0,0 });
				}

			}
			else if (pPlayer->point1 == THIRD_CORNER) {

				if (pPlayer->GetPos().x > -6.5f) add_vecx = -0.5f;
				else if (pPlayer->GetPos().x < -9.5f) add_vecx = 0.5f;

				pPlayer->PlayerRotation({ 0 + add_vecx,0,-1 + add_vecz });

				//詰まっているときの回避移動
				if (pPlayer->CheckPosAndPastPos()) {
					pPlayer->PlayerRotation({ 0,0,1 });
				}

			}
			else if (pPlayer->point1 == HOME_STRETCH_LEFT) {

				if (pPlayer->GetPos().z > -4.0f) add_vecz = -0.5f;
				else if (pPlayer->GetPos().z < -7.0f) add_vecz = 0.5f;

				pPlayer->PlayerRotation({ 1 + add_vecx,0,0 + add_vecz });

				//詰まっているときの回避移動
				if (pPlayer->CheckPosAndPastPos()) {
					pPlayer->PlayerRotation({ -1,0,0 });
				}

			}

			//前進
			pPlayer->Acceleration();


			//射撃
			if (pPlayer->ActionCooltimeCheck()) {

				D3DXVECTOR3 s_pos = pPlayer->GetPos();
				s_pos.y = 0.5f;

				int dummy;
				dummy = rand() % 4;
				
				if (dummy != 0)bullet_manager::SetRaceMineBullet(s_pos, pPlayer, pPlayer->number);
				else bullet_manager::SetRaceBullet(s_pos, pPlayer, pPlayer->number);
				shot_cooltime = 0;
				pPlayer->action_cooltime = 0;
			}



			break;

		case SURVIVAL_STAGE:


			if (IntervalTime >= BATTLE_INTERVAL) {

				float target_x, target_z;
				bool accel_flag = true;

				target_x = (rand() % 21) * 0.1f - 1.0f;
				target_z = (rand() % 21) * 0.1f - 1.0f;

				////外にいる時は内側を向く
				//x
				if (pPlayer->GetPos().x > BATTLE_STAGE_SIZE / 4) {

					target_x = -1;
					accel_flag = false;
				}
				else if (pPlayer->GetPos().x < -BATTLE_STAGE_SIZE / 4) {

					target_x = 1;
					accel_flag = false;
				}

				//z
				if (pPlayer->GetPos().z > BATTLE_STAGE_SIZE / 4) {

					target_z = -1;
					accel_flag = false;
				}
				else if (pPlayer->GetPos().z < -BATTLE_STAGE_SIZE / 4) {

					target_z = 1;
					accel_flag = false;
				}

				pPlayer->PlayerRotation({ target_x,0,target_z });

				IntervalTime = 0;
				D3DXVECTOR3 s_pos = pPlayer->GetPos();
				s_pos.y = 0.5f;

				//弾を撃つ
				bullet_manager::SetRadiationBullet(s_pos, pPlayer, 0);

			}

			//前進
			pPlayer->Acceleration();

			break;


		case ACTION_RACE_STAGE:

			if (bullet_manager::GetBullet(TargetBulletNum) == NULL) {
				BulletSearch();
			}
			else {
				TargetBullet = bullet_manager::GetBullet(TargetBulletNum);
			}

			if (TargetBullet != NULL) {


				//pPlayer->PlayerRotation(TargetBullet->GetPos());
				D3DXVECTOR3 dir;
				dir = TargetBullet->GetPos() - pPlayer->GetPos();


				pPlayer->PlayerRotation(dir);

				char buf[64];
				sprintf(buf, "posx = %f posy = %f posz = %f\n", TargetBullet->GetPos().x, TargetBullet->GetPos().y, TargetBullet->GetPos().z);
				OutputDebugString(buf);


				if (pPlayer->GetPos().y < TargetBullet->GetPos().y) {

					D3DXVECTOR3 up;
					up = pPlayer->GetPos() + D3DXVECTOR3(0, 0.10f, 0);

					pPlayer->SetPos(up);

				}

				TargetBullet = NULL;

			}

			//前進
			pPlayer->Acceleration();


			break;

	}

	IntervalTime++;

	shot_cooltime++;

}

void game_cpu::SetPlayer(player* pP, int type) {

	pPlayer = pP;
	cpuType = type;

}

void game_cpu::BulletSearch() {

	bullet* pBullet;

	char buf[64];
	sprintf(buf, "gb = %d\n", bullet_manager::GetBullet(TargetBulletNum));
	OutputDebugString(buf);

	if (TargetBullet == NULL && bullet_manager::GetBullet(TargetBulletNum) == NULL) {

		//for (int i = 0; i < BULLET_MAX;i++) {
		//
		//	pBullet = bullet_manager::GetBullet(i);
		//
		//	if (pBullet == NULL) continue;
		//	if (pBullet->GetBulletType() != POINT_BULLET) continue;
		//
		//	if(rand() % 10)
		//
		//	TargetBullet = pBullet;
		//
		//}

		int tnum = rand() % 10;

		pBullet = bullet_manager::GetBullet(tnum);
		
		if (pBullet == NULL) return;
		if (pBullet->GetBulletType() != POINT_BULLET) return;
		
		TargetBullet = pBullet;
		TargetBulletNum = tnum;

	}

}