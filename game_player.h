#pragma once
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <d3dx9.h>
#include "xmodel.h"

#define KNOCKBACK_SPEED_MIN (0.02f)

#define PLAYER_COLOR_RED    255,0,0,255
#define PLAYER_COLOR_GREEN  0,255,0,255
#define PLAYER_COLOR_BLUE   0,0,255,255
#define PLAYER_COLOR_YELLOW 255,255,0,255


#define PLAYER1_COLOR PLAYER_COLOR_BLUE
#define PLAYER2_COLOR PLAYER_COLOR_RED
#define PLAYER3_COLOR PLAYER_COLOR_GREEN
#define PLAYER4_COLOR PLAYER_COLOR_YELLOW

#define GRAVITY_SCALE (0.05f)

#define INVINCIBLE_TIME_MAX (60)


enum PLAYER_ACTION {

	ACCEL_ACTION,
	SHOT_ACTION

};

class game_cpu;
class wall;
class bullet;

class player {

private:

	int g_playerModelId = XMODEL_INVALID_ID;
	int playerBoardId;

	D3DXVECTOR3 g_front = { 0.0f,0.0f,-1.0f };//前方向
	D3DXVECTOR3 g_move = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 g_position = { 0.0f,2.0f,0.0f };
	D3DXVECTOR3 g_moveDir = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 size = { 0.5f,0.5f,0.5f };

	D3DXVECTOR3 g_target_direction = g_front;// { 0.0f, 0.0f, 0.0f };//行きたい方向

	float g_acceleration = 0.01f;//加速度
	float g_speed = 0.00f;
	float g_roll = 0.00f;
	D3DXVECTOR3 pastpos = g_position;

	float g_movingSpeed = 0.02f;
	float g_rotationSpeed = 0.00f;
	float sign = 1.0f;
	D3DXMATRIX g_mtxRotaion;

	int action_button_timer;
	game_cpu* pGameCpu;

	int t_sw;//点滅スイッチ

public:

	player(int number);
	~player();

	void Initialize(int set_number);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	void CPUMove();


	void SetMoveDir(const D3DXVECTOR3* pDir);
	void SetDirection(const D3DXVECTOR3* pDir);
	void Acceleration();
	void Action();//ゲームごとに違う挙動
	void ActionRealse();//ゲームごとに違う挙動

	D3DXVECTOR3 GetPos();
	void SetPos(D3DXVECTOR3 set_pos);
	D3DXVECTOR3 GetMoveDir();
	D3DXVECTOR3 GetSize();
	D3DXVECTOR3 GetFront();

	void UndoPos();
	void UndoPosY();
	void UndoPosZ();
	void UndoPosX();

	void Gravity();

	bool gravity_sw;//重力を作用させるかどうか
	bool exist;

	//ノックバック
	bool knockback_flag;//ノックバック中かどうか
	int knockback_time;
	D3DXVECTOR3 knockback_direction;
	void SetKnockbackDirection(D3DXVECTOR3 dir);
	void KnockbackUpdate();

	//加速
	float speed_correct;
	void SetHighAccel();
	bool highAccel_flag;
	int highAccel_time;
	void HighAccelUpdate();

	float GetSpeed();
	void PointUp(int number);
	//ゲーム判定
	int life;
	int point;//ポイントを区別する必要のないときに使う値
	int point1;//ポイントを区別する必要があるときに使う値
	int point2;
	int point3;
	int point4;

	//ジャンプ
	void Jump();

	int win;
	bool isJump;
	float jump_power;

	int number;

	void PlayerRotation(int rotation);
	void PlayerRotation(D3DXVECTOR3 target_dir);

	bool moveable;

	bool minigame_finish;

	void SetCPU(int type);

	int CPUmoveDir;


	void StatesReset();

	void SetInvincibleTime();
	void InvincibleTimeUpdate();

	int invincible_time = 0;

	bool CheckPosAndPastPos();

	void SetSensor();


	int action_cooltime;
	void ActionCooltimeUpdate();
	bool ActionCooltimeCheck();
	void ActionRaceUpdate();

	wall* pPlayerWall;

	bool avoid_flag;

	int GetActionButtonTimer();

};

#endif