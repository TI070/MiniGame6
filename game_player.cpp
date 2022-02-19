
#include "shadow.h"
#include "game_player.h"
#include <d3dx9.h>
#include <stdio.h>
#include "mydirect3d.h"
#include "effect_manager.h"
#include "bullet_manager.h"
#include "stage.h"
#include "key_manager.h"
#include "game_cpu.h"
#include "texture.h"
#include "billboard.h"
#include <stdio.h>

#define ROTATION_SPEED (0.01f)
#define ACCELERATION_BY_FRAME (0.01f)
#define NEARLY_ZERO_VALUE (0.001f)
#define SPEED_MAX (5/60.0f)
#define ROLL_MAX (D3DXToRadian(45))

#define KUMA_SIZE_C (0.003f)

#define KUMA_SIZE (0.5f)

#define KNOCKBACK_TIME (20)
#define HIGHACCEL_TIME (30)


#define LONGPRESS_TIME (60)

#define JUMP_POWER (0.5f)


player::player(int number) {

	Initialize(number);

}

player::~player() {


}

void player::Initialize(int set_number) {

	//g_playerModelId = XModel_Load("asset/model/airplane000.x");

	char buf[128];
	sprintf(buf,"asset/model/kuma%d.x",set_number + 1);
	//g_playerModelId = XModel_Load("asset/model/kuma.x");
	g_playerModelId = XModel_Load(buf);

	sprintf(buf, "asset/player%d_borad.png", set_number + 1);
	playerBoardId = Texture_SetTextureLoadFile(buf);


	D3DXMatrixIdentity(&g_mtxRotaion);

	g_target_direction = g_front;
	gravity_sw = false;
	knockback_flag = false;
	speed_correct = 1.0f;
	highAccel_flag = false;
	highAccel_time = 0;
	point = 0;
	point1 = 0;
	point2 = 0;
	point3 = 0;
	point4 = 0;
	life = 0;
	exist = true;
	win = 0;
	number = set_number;
	action_button_timer = 0;

	invincible_time = 0;
	t_sw = false;
	action_cooltime = 0;

	moveable = true;
	isJump = false;
	jump_power = 0;

	minigame_finish = false;

	pGameCpu = NULL;

	avoid_flag = false;

	bullet_manager::SetSensorBullet({}, this, number);

}

void player::Finalize(void) {

	XModel_Release(&g_playerModelId,1);

}

void player::Update(void) {

	if (minigame_finish) return;
	if (!exist) return;

	//過去ポジションを保持
	pastpos = g_position;

	//
	D3DXMATRIX mtxR;

	//どっちに回転すればいいか
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross,&g_front,&g_target_direction);
	float sign = cross.y < 0.0f ? -1 : 1;
	
	//目的の方向と現在の方向の角度の差を算出
	float direction_diff = acosf(D3DXVec3Dot(&g_front, &g_target_direction));

	float rotation = 0.1f * sign;
	
	if (rotation > direction_diff) {
		rotation = direction_diff;
	}

	//前方向ベクトルを回転
	D3DXMatrixRotationY(&mtxR,rotation);
	D3DXVec3TransformNormal(&g_front,&g_front,&mtxR);

	//座標の更新
	if(!knockback_flag) g_position += g_front * g_speed * speed_correct;
	else g_position += knockback_direction;

	KnockbackUpdate();
	HighAccelUpdate();

	//重力
	Gravity();


	//速度の減衰
	if(!highAccel_flag) g_speed *= 0.98f;

	//無敵時間処理
	InvincibleTimeUpdate();

	//
	if (stage::stage_n == ACTION_RACE_STAGE) ActionRaceUpdate();

	if(number != 0 || stage::stage_n == BATTLE_STAGE) ActionCooltimeUpdate();

	if(number == 0 && (stage::stage_n == SHOOTINGBATTLE_STAGE || stage::stage_n == RACE_STAGE)) ActionCooltimeUpdate();
}

void player::CPUMove() {

	pGameCpu->Update();

}

void player::Draw(void) {

	if (!exist) return;

	//無敵時間点滅
	if (invincible_time == 0) t_sw = false;
	else if (invincible_time % 5 == 0) t_sw = !t_sw;

	D3DXMATRIX mtxPlayer,mtxS,mtxT,mtxR;

	D3DXMatrixTranslation(&mtxT,g_position.x,g_position.y + 0.5f,g_position.z);

	D3DXMatrixRotationY(&mtxR, -atan2f(g_front.z,g_front.x) - D3DXToRadian(90));
	D3DXMatrixScaling(&mtxS,KUMA_SIZE_C, KUMA_SIZE_C, KUMA_SIZE_C);

	mtxPlayer = mtxS * mtxR * mtxT;
	//XModel_Draw(g_playerModelId,&mtxPlayer);
	if(!t_sw) XModel_Draw(g_playerModelId,&mtxPlayer);

	//影の描写
	if (stage::stage_n != ACTION_RACE_STAGE) {
		if (g_position.y > 0.0f) {//とりあえず想定した高さ(0.0f)より高ければ影を表示するようにする
			shadow::Draw(D3DXVECTOR3(g_position.x, g_position.y - 3.0f, g_position.z));
		}
	}

	//UI表示
	BillBorad_Draw(playerBoardId,g_position.x, g_position.y + 1.8f, g_position.z, 0.8f,1.0f);

}


void player::SetMoveDir(const D3DXVECTOR3* pDir) {

	g_moveDir = *pDir;

}

void player::SetDirection(const D3DXVECTOR3* pDir) {

	if (D3DXVec3LengthSq(pDir) <= NEARLY_ZERO_VALUE) {
		return;

	}
	g_target_direction = *pDir;

}

void player::Acceleration() {

	//速度の更新
	g_speed += g_acceleration;

	if (g_speed > SPEED_MAX) g_speed = SPEED_MAX;
}

D3DXVECTOR3 player::GetPos() {

	return g_position;
}

D3DXVECTOR3 player::GetSize() {

	return size;
}

D3DXVECTOR3 player::GetMoveDir() {

	return g_moveDir;
}

D3DXVECTOR3 player::GetFront() {

	return g_front;
}



void player::UndoPos() {
	g_position = pastpos;

}

void player::UndoPosY() {
	g_position.y = pastpos.y;

}

void player::UndoPosZ() {
	g_position.z = pastpos.z;

}

void player::UndoPosX() {
	g_position.x = pastpos.x;

}

void player::Gravity() {

	if (gravity_sw) {

		g_position -= {0,GRAVITY_SCALE,0};
		g_position += {0,jump_power,0};

		if (jump_power > 0) jump_power -= GRAVITY_SCALE;
		else  jump_power = 0;

	}

}

void player::SetPos(D3DXVECTOR3 set_pos) {
	g_position = set_pos;

}


void player::SetKnockbackDirection(D3DXVECTOR3 dir) {

	knockback_time = KNOCKBACK_TIME;

	knockback_flag = true;
	knockback_direction = dir;

}

void player::KnockbackUpdate() {

	if (knockback_flag) {
		knockback_time--;
		if (knockback_time <= 0) {
			knockback_flag = false;
			g_speed = 0.0f;
		}
	}

}

void player::HighAccelUpdate() {

	if (highAccel_flag) {
		highAccel_time--;
		if (highAccel_time <= 0) {
			highAccel_flag = false;
			speed_correct = 1.0f;
		}

		//エフェクトを出す
		effect_manager::SetEffect(g_position,size.x,number);

	}

}

void player::SetHighAccel() {

	//落下中は実行しない

	if (g_position.y < 0) return;

	speed_correct = 2.0f;
	highAccel_flag = true;
	highAccel_time = HIGHACCEL_TIME;

}

float player::GetSpeed() {

	return speed_correct * g_speed;
}

void player::Action() {

	switch (stage::stage_n) {

		case BATTLE_STAGE:
			if (KeyManager_Trigger(KM_SPACE)) {
				if (ActionCooltimeCheck()) {
					SetHighAccel();
					action_cooltime = 0;
				}
			}
			break;

		case SHOOTINGBATTLE_STAGE:



			if (!ActionCooltimeCheck()) return;
			//長押し
			action_button_timer++;

			break;

		case ACTION_RACE_STAGE:

			g_position += {0, 0.05f, 0};

			break;


		case RACE_STAGE:

			if (!ActionCooltimeCheck()) return;

			//長押し
			action_button_timer++;
			//ActionCooltimeUpdate();

			break;

		case SURVIVAL_STAGE:

			if (KeyManager_Trigger(KM_SPACE)) {
				D3DXVECTOR3 s_pos = g_position;
				s_pos.y = 0.5f;

				bullet_manager::SetRadiationBullet(s_pos, this, 0);

			}

			break;

	}
}

void player::ActionRealse() {

	switch (stage::stage_n) {

	case BATTLE_STAGE:

		break;

	case SHOOTINGBATTLE_STAGE:

		if (!ActionCooltimeCheck()) return;

		//長押し
		if (action_button_timer > LONGPRESS_TIME) {
			D3DXVECTOR3 s_pos = g_position;
			s_pos.y = 1.5f;

			bullet_manager::SetRadiationBullet(s_pos, this, 0);

			action_button_timer = 0;
		}
		else {

			D3DXVECTOR3 s_pos = g_position;
			s_pos.y = 0.5f;

			bullet_manager::SetBullet(s_pos, this, 0);

			action_button_timer = 0;

		}
		action_cooltime = 0;
		

		break;

	case RACE_STAGE:

		if (!ActionCooltimeCheck()) return;

		// 長押し
		if (action_button_timer > LONGPRESS_TIME) {
			//地雷
			D3DXVECTOR3 s_pos = g_position;
			s_pos.y = 0.5f;

			bullet_manager::SetRaceMineBullet(s_pos, this, number);

			action_button_timer = 0;

		}
		else {
			D3DXVECTOR3 s_pos = g_position;
			s_pos.y = 0.5f;

			bullet_manager::SetRaceBullet(s_pos, this, number);

			action_button_timer = 0;

		}
		action_cooltime = 0;

		break;
	}
}

void player::PointUp(int number) {

	if (number == 0) point1++;
	else if (number == 1) point2++;
	else if (number == 2) point3++;
	else if (number == 3) point4++;

}

void player::PlayerRotation(int rotation) {

	D3DXMATRIX mtxR;

	//前方向ベクトルを回転
	D3DXMatrixRotationY(&mtxR,  D3DXToRadian(rotation));
	D3DXVec3TransformNormal(&g_front, &g_front, &mtxR);

	g_target_direction = {0,0,1};

}

void player::PlayerRotation(D3DXVECTOR3 target_dir) {

	g_target_direction = target_dir;

}



void player::Jump() {

	if (!isJump) {

		isJump = true;
		jump_power = JUMP_POWER;



	}

}

void player::SetCPU(int type) {

	pGameCpu = new game_cpu(type,this);

}

void player::StatesReset() {

	g_target_direction = g_front;
	gravity_sw = false;
	knockback_flag = false;
	speed_correct = 1.0f;
	highAccel_flag = false;
	highAccel_time = 0;
	point = 0;
	point1 = 0;
	point2 = 0;
	point3 = 0;
	point4 = 0;
	life = 0;
	exist = true;
	action_button_timer = 0;

	invincible_time = 0;
	t_sw = false;
	action_cooltime = 0;

	moveable = true;
	isJump = false;
	jump_power = 0;

	avoid_flag = false;

	minigame_finish = false;
}


void player::SetInvincibleTime() {

	invincible_time = INVINCIBLE_TIME_MAX;
}

void player::InvincibleTimeUpdate() {

	if(invincible_time > 0) invincible_time--;

}

//１個前の位置と同じならtrueを返す
bool player::CheckPosAndPastPos() {

	if (pastpos == g_position) return true;

	return false;
}

void player::SetSensor() {

	bullet_manager::SetSensorBullet({}, this, number);

}

void player::ActionCooltimeUpdate() {

	switch (stage::stage_n) {

		case BATTLE_STAGE:

		if (action_cooltime < 30) {

			action_cooltime++;
		}
		break;

		case SHOOTINGBATTLE_STAGE:

		if (action_cooltime < 60) {

			action_cooltime++;
		}
		break;

		case RACE_STAGE:

			if (action_cooltime < 60) {

				action_cooltime++;
			}
		break;
	}
}

bool player::ActionCooltimeCheck() {

	switch (stage::stage_n) {

	case BATTLE_STAGE:
		if(action_cooltime >= 30) return true;
		break;

	case SHOOTINGBATTLE_STAGE:
		if (action_cooltime >= 60) return true;
		break;

	case RACE_STAGE:
		if (action_cooltime >= 60) return true;
		break;
	}


	return false;

}


void player::ActionRaceUpdate() {

	if (!KeyManager_Press(KM_SPACE)) {
		g_position -= {0, 0.05f, 0};
	}

	if (g_position.y > ACTIONRACE_STAGE_SIZE / 2 - MASS_SIZE * 2) {

		g_position.y = ACTIONRACE_STAGE_SIZE / 2 - MASS_SIZE * 2;
	}


}

int player::GetActionButtonTimer() {

	return action_button_timer;

}
