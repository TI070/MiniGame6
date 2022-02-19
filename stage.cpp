
#include "stage.h"
#include "camera.h"
#include "wall_manager.h"
#include "player_manager.h"
#include "bullet_manager.h"
#include "ui_states.h"
#include "game_system.h"
#include "key_manager.h"
#include "scene_manager.h"
#include "effect_manager.h"
#include "bullet.h"
#include "sound.h"
#include "menu.h"
#include "title.h"
#include <stdio.h>




#define BATTLEDOME_TIME (3600)//(3600)
#define SURVIVAL_TIME (1800)//(3600)

#define DOWN_LIMIT (-10)

#define BATTLEDOME_BALLINTIME (60)

#define HOME_STRETCH_RIGHT_ENDLINE_X (5)
#define FIRST_CORNER_ENDLINE_Z (2.5f)
#define BACK_STRETCH_X (-5)
#define THIRD_CORNER_ENDLINE_Z (-2.5f)
#define HOME_STRETCH_LEFT_ENDLINE_X (0)

int stage::stage_n;
player* stage::pPlayer[4];
int stage::game_timer;
int stage::sub_timer;
bool stage::minigame_finish;

void stage::Initialize() {

	//�v���C���[1�̐���
	player_manager::SetPlayer(KUMA);
	//�v���C���[2�̐���
	player_manager::SetPlayer(KUMA);
	//�v���C���[3�̐���
	player_manager::SetPlayer(KUMA);
	//�v���C���[4�̐���
	player_manager::SetPlayer(KUMA);



	//�|�C���^�擾
	pPlayer[0] = player_manager::GetPlayer(0);
	pPlayer[1] = player_manager::GetPlayer(1);
	pPlayer[2] = player_manager::GetPlayer(2);
	pPlayer[3] = player_manager::GetPlayer(3);


}

void stage::SetStage(int stage_num) {



	//����
	game_timer = 0;
	sub_timer = 0;

	stage_n = stage_num;

	minigame_finish = false;

	//������
	game_system::Initiaize();

	SetCameraVec();

	switch (stage_num)
	{
	case SHOOTINGBATTLE_STAGE:

		//�X�e�[�W�̃Z�b�g
		wall_manager::SetWall(D3DXVECTOR3(0, 0, 0), SHOOTINGBATTLE_STAGE_SIZE, SHOOTINGBATTLE_STAGE_SIZE, XZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(SHOOTINGBATTLE_STAGE_SIZE/2, 2.5f, 0), SHOOTINGBATTLE_STAGE_SIZE/2, SHOOTINGBATTLE_STAGE_SIZE, YZ_FIELD);//�E��
		wall_manager::SetTransparentWall(D3DXVECTOR3(-SHOOTINGBATTLE_STAGE_SIZE/2, 2.5f, 0), SHOOTINGBATTLE_STAGE_SIZE/2, SHOOTINGBATTLE_STAGE_SIZE, YZ_FIELD);//����
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, 2.5f, SHOOTINGBATTLE_STAGE_SIZE/2), SHOOTINGBATTLE_STAGE_SIZE, SHOOTINGBATTLE_STAGE_SIZE/2, XY_FIELD);//����
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, 2.5f, -SHOOTINGBATTLE_STAGE_SIZE/2), SHOOTINGBATTLE_STAGE_SIZE, SHOOTINGBATTLE_STAGE_SIZE/2, XY_FIELD);//��O��

		wall_manager::SetCubeWall(D3DXVECTOR3(0, 0, 0), MASS_SIZE, MASS_SIZE);
		wall_manager::SetCubeWall(D3DXVECTOR3(-5, 0,-5), MASS_SIZE, MASS_SIZE);
		wall_manager::SetCubeWall(D3DXVECTOR3(-5, 0, 5), MASS_SIZE, MASS_SIZE);
		wall_manager::SetCubeWall(D3DXVECTOR3( 5, 0,-5), MASS_SIZE, MASS_SIZE);
		wall_manager::SetCubeWall(D3DXVECTOR3( 5, 0, 5), MASS_SIZE, MASS_SIZE);

		//�w�i�̃Z�b�g
		//�n��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, SHOOTINGBATTLE_STAGE_SIZE), SHOOTINGBATTLE_STAGE_SIZE * 5, SHOOTINGBATTLE_STAGE_SIZE * 1, XZ_FIELD,GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5, 0, 0), SHOOTINGBATTLE_STAGE_SIZE * 1.5f, SHOOTINGBATTLE_STAGE_SIZE * 3, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(-SHOOTINGBATTLE_STAGE_SIZE - MASS_SIZE * 5, 0, 0), SHOOTINGBATTLE_STAGE_SIZE * 1.5f, SHOOTINGBATTLE_STAGE_SIZE * 3, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, -SHOOTINGBATTLE_STAGE_SIZE), SHOOTINGBATTLE_STAGE_SIZE * 5, SHOOTINGBATTLE_STAGE_SIZE * 1, XZ_FIELD, GRASS);//��

		//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), SHOOTINGBATTLE_STAGE_SIZE * 7, SHOOTINGBATTLE_STAGE_SIZE * 3, XY_FIELD, SKY);//��

		//�_
		wall_manager::SetBgWall(D3DXVECTOR3(0, MASS_SIZE * 10, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 2, MASS_SIZE, XY_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 17, MASS_SIZE * 8, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 2, XY_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 24, MASS_SIZE * 11, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 1, XY_FIELD, CLOUD);//��


		//�J�����̃Z�b�g
		SetCameraMode(FOLLOW_CAMERA);
		Camera_SetAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		Camera_SetPosition(D3DXVECTOR3(0.0f, 3.5f, -10.0f));

		//�v���C���[�̃Z�b�g
		pPlayer[0]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, 4.0f));
		pPlayer[1]->SetPos(D3DXVECTOR3(4.0f, 0.04f, -4.0f));
		pPlayer[2]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, -4.0f));
		pPlayer[3]->SetPos(D3DXVECTOR3(4.0f, 0.04f, 4.0f));

		for(int i = 0;i < 4 ; i++) pPlayer[i]->moveable = true;

		//�G���E�̃Z�b�g
		for (int i = 1; i < 4; i++) pPlayer[i]->SetSensor();

		break;


	case BATTLE_STAGE:

		//�X�e�[�W�̃Z�b�g
		wall_manager::SetWall(D3DXVECTOR3(0, 0, 0), BATTLE_STAGE_SIZE, BATTLE_STAGE_SIZE, XZ_FIELD);//��

		//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, -10, 0), MASS_SIZE * 100, MASS_SIZE * 130, XZ_FIELD, SKY);//��

		//�_
		wall_manager::SetBgWall(D3DXVECTOR3(-MASS_SIZE * 10, -9, -MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 2, XZ_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3( MASS_SIZE * 7, -9, -MASS_SIZE * -6), MASS_SIZE * 4, MASS_SIZE * 3, XZ_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 18, -9,  MASS_SIZE * 20), MASS_SIZE * 5, MASS_SIZE * 3, XZ_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 11, -9, MASS_SIZE * 26), MASS_SIZE * 5, MASS_SIZE * 2, XZ_FIELD, CLOUD);//��


		//�d�͂̃Z�b�g
		player_manager::GravitySwOn(0);
		player_manager::GravitySwOn(1);
		player_manager::GravitySwOn(2);
		player_manager::GravitySwOn(3);

		//�J�����̃Z�b�g
		Camera_SetPosition(D3DXVECTOR3(0.0f, 9.5f, -9.5f));
		Camera_SetAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetCameraMode(FIXED_CAMERA);

		//�v���C���[�̃Z�b�g
		pPlayer[0]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, 4.0f));
		pPlayer[1]->SetPos(D3DXVECTOR3( 4.0f, 0.04f,-4.0f));
		pPlayer[2]->SetPos(D3DXVECTOR3(-4.0f, 0.04f,-4.0f));
		pPlayer[3]->SetPos(D3DXVECTOR3( 4.0f, 0.04f, 4.0f));

		for (int i = 0; i < 4; i++) pPlayer[i]->moveable = true;

		//�G���E�̃Z�b�g
		for (int i = 1; i < 4; i++) pPlayer[i]->SetSensor();

		//�v���C���[�̃X�e�[�^�X�Z�b�g
		pPlayer[0]->life = 3;
		pPlayer[1]->life = 3;
		pPlayer[2]->life = 3;
		pPlayer[3]->life = 3;


		break;

	case ACTION_RACE_STAGE:

		//�X�e�[�W�̃Z�b�g
		wall_manager::SetWall(D3DXVECTOR3(0,  ACTIONRACE_STAGE_SIZE / 2 + MASS_SIZE / 2, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(0, -ACTIONRACE_STAGE_SIZE / 2, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(ACTIONRACE_STAGE_SIZE / 2 + MASS_SIZE / 2, 0, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, YZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(-ACTIONRACE_STAGE_SIZE / 2, 0, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, YZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(0, 0, ACTIONRACE_STAGE_SIZE / 2 + MASS_SIZE / 2), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XY_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(0, 0, -ACTIONRACE_STAGE_SIZE / 2), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XY_FIELD);//��

		//����
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, ACTIONRACE_STAGE_SIZE / 2 - MASS_SIZE, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, -ACTIONRACE_STAGE_SIZE / 2 + MASS_SIZE, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(ACTIONRACE_STAGE_SIZE / 2 - MASS_SIZE, 0, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, YZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(-ACTIONRACE_STAGE_SIZE / 2 + MASS_SIZE, 0, 0), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, YZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, 0, ACTIONRACE_STAGE_SIZE / 2 - MASS_SIZE), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XY_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, 0, -ACTIONRACE_STAGE_SIZE / 2 + MASS_SIZE), ACTIONRACE_STAGE_SIZE, ACTIONRACE_STAGE_SIZE, XY_FIELD);//��


		ActionRaceStageSet();


		//�v���C���[�̃Z�b�g
		pPlayer[0]->SetPos(D3DXVECTOR3(-3.0f, 0.04f, -ACTIONRACE_STAGE_SIZE / 2 + 3.0f));
		pPlayer[1]->SetPos(D3DXVECTOR3(-1.0f, 0.04f, -ACTIONRACE_STAGE_SIZE / 2 + 3.0f));
		pPlayer[2]->SetPos(D3DXVECTOR3( 1.0f, 0.04f, -ACTIONRACE_STAGE_SIZE / 2 + 3.0f));
		pPlayer[3]->SetPos(D3DXVECTOR3( 3.0f, 0.04f, -ACTIONRACE_STAGE_SIZE / 2 + 3.0f));

		for (int i = 0; i < 4; i++) {
			pPlayer[i]->PlayerRotation({0,0,-1});
			pPlayer[i]->moveable = true;
		}

		//�G���E�̃Z�b�g
		//for (int i = 1; i < 4; i++) pPlayer[i]->SetSensor();

		//�^�C�}�[�Z�b�g
		//game_timer = BATTLEDOME_TIME;
		game_timer = 40 * 60;

		//�d�͂̃Z�b�g
		//player_manager::GravitySwOn(0);
		//player_manager::GravitySwOn(1);
		//player_manager::GravitySwOn(2);
		//player_manager::GravitySwOn(3);

		//�J�����̃Z�b�g
		SetCameraMode(FOLLOW_CAMERA);
		//Camera_SetPosition(pPlayer[0]->GetPos() + D3DXVECTOR3(0.0f, 0.5f, -0.5f));
		Camera_SetPosition({0,1.0f,-1.5f});
		Camera_SetAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;

	case BATTLEDOME_STAGE:
		//�X�e�[�W�̃Z�b�g
		wall_manager::SetWall(D3DXVECTOR3(0, 0, 0), BATTLEDOME_STAGE_SIZE, BATTLEDOME_STAGE_SIZE, XZ_FIELD);//��
		//wall_manager::SetWall(D3DXVECTOR3(-BATTLEDOME_STAGE_SIZE / 2, 1, 0), BATTLEDOME_STAGE_SIZE / 3.0f, 1, XY_FIELD);//��


		wall_manager::SetWall(D3DXVECTOR3(-BATTLEDOME_STAGE_SIZE / 2.0f + BATTLEDOME_STAGE_SIZE / 6.0f, 0.5f, -2.5f), BATTLEDOME_STAGE_SIZE / 3.0f, 1, XY_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(-BATTLEDOME_STAGE_SIZE / 2.0f + BATTLEDOME_STAGE_SIZE / 6.0f, 0.5f,  2.5f), BATTLEDOME_STAGE_SIZE / 3.0f, 1, XY_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3( BATTLEDOME_STAGE_SIZE / 2.0f - BATTLEDOME_STAGE_SIZE / 6.0f, 0.5f, -2.5f), BATTLEDOME_STAGE_SIZE / 3.0f, 1, XY_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3( BATTLEDOME_STAGE_SIZE / 2.0f - BATTLEDOME_STAGE_SIZE / 6.0f, 0.5f,  2.5f), BATTLEDOME_STAGE_SIZE / 3.0f, 1, XY_FIELD);//��
																																								
		wall_manager::SetWall(D3DXVECTOR3(-2.5f, 0.5f, -BATTLEDOME_STAGE_SIZE / 2.0f + BATTLEDOME_STAGE_SIZE / 6.0f), 1, BATTLEDOME_STAGE_SIZE / 3.0f, YZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(2.5f, 0.5f, -BATTLEDOME_STAGE_SIZE / 2.0f + BATTLEDOME_STAGE_SIZE / 6.0f), 1, BATTLEDOME_STAGE_SIZE / 3.0f, YZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(-2.5f, 0.5f, BATTLEDOME_STAGE_SIZE / 2.0f - BATTLEDOME_STAGE_SIZE / 6.0f), 1, BATTLEDOME_STAGE_SIZE / 3.0f, YZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(2.5f, 0.5f, BATTLEDOME_STAGE_SIZE / 2.0f - BATTLEDOME_STAGE_SIZE / 6.0f), 1, BATTLEDOME_STAGE_SIZE / 3.0f,    YZ_FIELD);//��


		//�v���C���[�̃Z�b�g
		pPlayer[0]->exist = false;
		pPlayer[1]->exist = false;
		pPlayer[2]->exist = false;
		pPlayer[3]->exist = false;


		//�v���C���[1
		wall_manager::SetMoveWall(D3DXVECTOR3( 0, 0.5f, -5), 2, 1, XY_FIELD,pPlayer[0]);

		//�v���C���[2
		wall_manager::SetMoveWall(D3DXVECTOR3( 0, 0.5f,  5), 2, 1, XY_FIELD, pPlayer[1]);

		//�v���C���[3
		wall_manager::SetMoveWall(D3DXVECTOR3( 5, 0.5f,  0), 1, 2, YZ_FIELD, pPlayer[2]);

		//�v���C���[4
		wall_manager::SetMoveWall(D3DXVECTOR3(-5, 0.5f,  0), 1, 2, YZ_FIELD, pPlayer[3]);


		pPlayer[0]->SetPos(D3DXVECTOR3(0, 0.5f, -5));
		pPlayer[1]->SetPos(D3DXVECTOR3(0, 0.5f, 5));
		pPlayer[2]->SetPos(D3DXVECTOR3(5, 0.5f, 0));
		pPlayer[3]->SetPos(D3DXVECTOR3(-5, 0.5f, 0));
		//�G���E�̃Z�b�g
		for (int i = 1; i < 4; i++) pPlayer[i]->SetSensor();

		for (int i = 0; i < 4; i++) pPlayer[i]->moveable = true;


		//�J�����̃Z�b�g
		Camera_SetPosition(D3DXVECTOR3(0.0f, 15.0f, 0.0f));
		Camera_SetAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetCameraMode(FIXED_CAMERA);

		//�v���C���[�̃Z�b�g
		//pPlayer[0]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, 4.0f));
		//pPlayer[1]->SetPos(D3DXVECTOR3(4.0f, 0.04f, -4.0f));
		//pPlayer[2]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, -4.0f));
		//pPlayer[3]->SetPos(D3DXVECTOR3(4.0f, 0.04f, 4.0f));

		//�^�C�}�[�Z�b�g
		game_timer = BATTLEDOME_TIME;

		break;


	case SURVIVAL_STAGE:

		//�X�e�[�W�̃Z�b�g
		for (int i = 0; i < 15; i++) {

			for (int j = 0; j < 15; j++) {

				wall_manager::SetPaintWall(D3DXVECTOR3(-7.0f + 1.0f * i, 0, -7.0f + 1.0f * j), MASS_SIZE,MASS_SIZE, XZ_FIELD);//��

			}

		}
		//wall_manager::SetWall(D3DXVECTOR3( 0.0f, 0, -3.0), MASS_SIZE * 15, MASS_SIZE * 5, XZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3( 0.0f, 0, 0.0), SURVIVAL_STAGE_SIZE, SURVIVAL_STAGE_SIZE, XZ_FIELD);//��
		wall_manager::SetTransparentWall(D3DXVECTOR3(SURVIVAL_STAGE_SIZE / 2, 2.5f, 0), SURVIVAL_STAGE_SIZE / 2, SURVIVAL_STAGE_SIZE, YZ_FIELD);//�E��
		wall_manager::SetTransparentWall(D3DXVECTOR3(-SURVIVAL_STAGE_SIZE / 2, 2.5f, 0), SURVIVAL_STAGE_SIZE / 2, SURVIVAL_STAGE_SIZE, YZ_FIELD);//����
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, 2.5f, SURVIVAL_STAGE_SIZE / 2), SURVIVAL_STAGE_SIZE, SURVIVAL_STAGE_SIZE / 2, XY_FIELD);//����
		wall_manager::SetTransparentWall(D3DXVECTOR3(0, 2.5f, -SURVIVAL_STAGE_SIZE / 2), SURVIVAL_STAGE_SIZE, SURVIVAL_STAGE_SIZE / 2, XY_FIELD);//��O��



		//�w�i�̃Z�b�g
		//�n��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, SURVIVAL_STAGE_SIZE), SURVIVAL_STAGE_SIZE * 5, SURVIVAL_STAGE_SIZE * 1, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(SURVIVAL_STAGE_SIZE + MASS_SIZE * 5, 0, 0), SURVIVAL_STAGE_SIZE * 1.5f + MASS_SIZE * 3, SURVIVAL_STAGE_SIZE * 3, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(-SURVIVAL_STAGE_SIZE - MASS_SIZE * 5, 0, 0), SURVIVAL_STAGE_SIZE * 1.5f + MASS_SIZE * 3, SURVIVAL_STAGE_SIZE * 3, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, -SURVIVAL_STAGE_SIZE), SURVIVAL_STAGE_SIZE * 5, SURVIVAL_STAGE_SIZE * 1, XZ_FIELD, GRASS);//��

		//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, SURVIVAL_STAGE_SIZE + MASS_SIZE * 5), SURVIVAL_STAGE_SIZE * 7, SURVIVAL_STAGE_SIZE * 3, XY_FIELD, SKY);//��

		//�_
		wall_manager::SetBgWall(D3DXVECTOR3(0, MASS_SIZE * 10, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 2, MASS_SIZE, XY_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 17, MASS_SIZE * 8, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 2, XY_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 24, MASS_SIZE * 11, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 1, XY_FIELD, CLOUD);//��



		//wall_manager::SetWall(D3DXVECTOR3(0.0f, 0.5f, -0.5), MASS_SIZE * 15, MASS_SIZE * 1, XY_FIELD);//��
		//�^�C�}�[�Z�b�g
		game_timer = SURVIVAL_TIME;


		//�v���C���[�̃Z�b�g
		pPlayer[0]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, 4.0f));
		pPlayer[1]->SetPos(D3DXVECTOR3(4.0f, 0.04f, -4.0f));
		pPlayer[2]->SetPos(D3DXVECTOR3(-4.0f, 0.04f, -4.0f));
		pPlayer[3]->SetPos(D3DXVECTOR3(4.0f, 0.04f, 4.0f));

		for (int i = 0; i < 4; i++) pPlayer[i]->moveable = true;
		//�G���E�̃Z�b�g
		for (int i = 1; i < 4; i++) pPlayer[i]->SetSensor();

		//�v���C���[�̃X�e�[�^�X�Z�b�g
		pPlayer[0]->life = 1;
		pPlayer[1]->life = 1;
		pPlayer[2]->life = 1;
		pPlayer[3]->life = 1;

		//�d�͂̃Z�b�g
		player_manager::GravitySwOn(0);
		player_manager::GravitySwOn(1);
		player_manager::GravitySwOn(2);
		player_manager::GravitySwOn(3);

		//�J�����̃Z�b�g
		Camera_SetPosition(D3DXVECTOR3(0.0f, 9.5f, -13.5f));
		SetCameraMode(FIXED_CAMERA);
		Camera_SetAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;

	case RACE_STAGE:
		//�X�e�[�W�̃Z�b�g
		wall_manager::SetWall(D3DXVECTOR3(0.0f, 0, -5.0), MASS_SIZE * 20, MASS_SIZE * 5, XZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(0.0f, 0,  5.0), MASS_SIZE * 20, MASS_SIZE * 5, XZ_FIELD);//��

		wall_manager::SetWall(D3DXVECTOR3(7.5f, 0, 0.0), MASS_SIZE * 5, MASS_SIZE * 15, XZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(-7.5f, 0, 0.0), MASS_SIZE * 5, MASS_SIZE * 15, XZ_FIELD);//��


		//����
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				wall_manager::SetCubeWall(D3DXVECTOR3(-4.5f + i * 1.0f, 0, 1.5f - j * 1.0f), MASS_SIZE, MASS_SIZE);
			}
		}

		//���E��
		wall_manager::SetWall(D3DXVECTOR3(10.0f, 0.5f, 0.0), MASS_SIZE * 1.0f, MASS_SIZE * 15.0f, YZ_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(-10.0f, 0.5f, 0.0), MASS_SIZE * 1.0f, MASS_SIZE * 15.0f, YZ_FIELD);//��

		//�O���
		wall_manager::SetWall(D3DXVECTOR3(0.0f, 0.5f,  7.5f), MASS_SIZE * 20.0f, MASS_SIZE * 1.0f, XY_FIELD);//��
		wall_manager::SetWall(D3DXVECTOR3(0.0f, 0.5f, -7.5f), MASS_SIZE * 20.0f, MASS_SIZE * 1.0f, XY_FIELD);//��


		//�w�i�̃Z�b�g
		//�n��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, SURVIVAL_STAGE_SIZE), SURVIVAL_STAGE_SIZE * 5, SURVIVAL_STAGE_SIZE * 1, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(SURVIVAL_STAGE_SIZE + MASS_SIZE * 5, 0, 0), SURVIVAL_STAGE_SIZE * 1.0f + MASS_SIZE * 3, SURVIVAL_STAGE_SIZE * 3, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(-SURVIVAL_STAGE_SIZE - MASS_SIZE * 5, 0, 0), SURVIVAL_STAGE_SIZE * 1.0f + MASS_SIZE * 3, SURVIVAL_STAGE_SIZE * 3, XZ_FIELD, GRASS);//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, -SURVIVAL_STAGE_SIZE), SURVIVAL_STAGE_SIZE * 5, SURVIVAL_STAGE_SIZE * 1, XZ_FIELD, GRASS);//��

		//��
		wall_manager::SetBgWall(D3DXVECTOR3(0, 0, SURVIVAL_STAGE_SIZE + MASS_SIZE * 5), SURVIVAL_STAGE_SIZE * 7, SURVIVAL_STAGE_SIZE * 3, XY_FIELD, SKY);//��

		//�_
		wall_manager::SetBgWall(D3DXVECTOR3(0, MASS_SIZE * 10, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 2, MASS_SIZE, XY_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 17, MASS_SIZE * 8, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 2, XY_FIELD, CLOUD);//��
		wall_manager::SetBgWall(D3DXVECTOR3(MASS_SIZE * 24, MASS_SIZE * 11, SHOOTINGBATTLE_STAGE_SIZE + MASS_SIZE * 5), MASS_SIZE * 3, MASS_SIZE * 1, XY_FIELD, CLOUD);//��





		//�v���C���[�̃Z�b�g
		pPlayer[0]->SetPos(D3DXVECTOR3(-0.0f, 0.04f, -7.0f));
		pPlayer[1]->SetPos(D3DXVECTOR3( 0.0f, 0.04f, -6.0f));
		pPlayer[2]->SetPos(D3DXVECTOR3(-0.0f, 0.04f, -5.0f));
		pPlayer[3]->SetPos(D3DXVECTOR3( 0.0f, 0.04f, -4.0f));

		for (int i = 0; i < 4; i++) {
			pPlayer[i]->PlayerRotation({1,0,0});
		}
		//�G���E�̃Z�b�g
		for (int i = 1; i < 4; i++) pPlayer[i]->SetSensor();

		//�J�����̃Z�b�g
		Camera_SetPosition(D3DXVECTOR3(0.0f, 10.0f, -12.0f));
		SetCameraMode(FIXED_CAMERA);
		Camera_SetAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;

	}



	


}


void stage::Draw() {

	switch (stage_n)
	{
	case BATTLE_STAGE:
		ui_states::BattleDraw();
		break;
	case SHOOTINGBATTLE_STAGE:
		ui_states::ShootingBattleDraw();
		break;
	case BATTLEDOME_STAGE:
		ui_states::BattleDomeDraw();
		break;
	case RACE_STAGE:
		ui_states::RaceDraw();
		break;
	case ACTION_RACE_STAGE:
		ui_states::ActionRaceDraw();
		break;

	case SURVIVAL_STAGE:
		ui_states::SurvivalDraw();
		break;
	}

}

void stage::Update() {

	if (minigame_finish) {

		if (KeyManager_Trigger(KM_ENTER)) {

			//���j���[�ɑJ��
			scene_manager::SetCurrentScene(MENU);

			//BGM
			PlaySound(SOUND_LABEL_BGMTITLE);


			//�����ʂ����[�h����
			if (menu::GetMode() == WINPLAY) {

				//�����Ă����玟�̃Q�[����
				if (game_system::win_player == 0) {
					menu::SelectPlus(1);
				}
				else {//��������^�C�g��

					scene_manager::SetCurrentScene(TITLE);
					return;
				}

				if (menu::GetSelectGame() == 0) {//���ׂăN���A

					//�N���A��ʂɑJ��
					scene_manager::SetCurrentScene(RESULT);

				}

			}


		}
		return;
	}

	switch (stage_n)
	{
		case BATTLE_STAGE:
			FallDeathCheck();
			break;

		case BATTLEDOME_STAGE:

			BattleDomePointCheck();

			TimerCheck();

			if (sub_timer > BATTLEDOME_BALLINTIME) {

				//��m����3�|�C���g���̒e���o��
				int dummy;
				dummy = rand() % 11;

				dummy /= 10;

				bullet_manager::SetBallBullet({0,0.5f,0}, dummy);
				sub_timer = 0;
			}
			break;

		case ACTION_RACE_STAGE:

			for (int i = 0; i < 4; i++) {
				pPlayer[i]->Acceleration();
			}

			if (KeyManager_Press(KM_SPACE)) {
				pPlayer[0]->Action();
			}

			TimerCheck();

			break;

		case SURVIVAL_STAGE:

			if (KeyManager_Trigger(KM_SPACE)) {
				pPlayer[0]->Action();
			}

			TimerCheck();

			break;

		case RACE_STAGE:

			LapCheck();

			break;

	}

	if (Finish_Check()) {
		for (int i = 0; i < 4; i++)pPlayer[i]->minigame_finish = true;
		minigame_finish = true;

		//bgm
		StopSound(SOUND_LABEL_BGMSTAGE);

		//se
		PlaySound(SOUND_LABEL_SE_MINIGAME_END);

	}


	sub_timer++;
}

void stage::FallDeathCheck() {

	for (int i = 0; i < 4; i++) {

		if (pPlayer[i]->GetPos().y < DOWN_LIMIT) FallDeath(pPlayer[i],i);


	}


}

void stage::FallDeath(player* pP,int i) {

	pP->life--;

	switch (stage_n)
	{
	case BATTLE_STAGE:

		if (pP->life > 0) {//���C�t���c���Ă���Ȃ畜��
			D3DXVECTOR3 set_pos;
			if (i == 0) set_pos = D3DXVECTOR3(-4.0f, 0.04f, 4.0f);
			else if (i == 1) set_pos = D3DXVECTOR3(4.0f, 0.04f, -4.0f);
			else if (i == 2) set_pos = D3DXVECTOR3(-4.0f, 0.04f, -4.0f);
			else if (i == 3) set_pos = D3DXVECTOR3(4.0f, 0.04f, 4.0f);

			pP->SetPos(set_pos);
		}
		else {//�c���Ă�����Α��݂�����
			pP->SetPos(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
			pP->exist = false;
		}
		break;
	}

}

bool stage::Finish_Check() {

	bool PminFlag[4] = {};

	switch (stage_n)
	{
		case SHOOTINGBATTLE_STAGE:

			//���s
			if (pPlayer[0]->point2 >= 2 && pPlayer[0]->point3 >= 2 && pPlayer[0]->point4 >= 2) {

				if (OtherDeathCheck(0)) {

					game_system::PlayerWin(pPlayer[0], 0);
					return true;

				}

			}
			else if (pPlayer[1]->point1 >= 2 && pPlayer[1]->point3 >= 2 && pPlayer[1]->point4 >= 2) {

				if (OtherDeathCheck(1)) {

					game_system::PlayerWin(pPlayer[1], 1);
					return true;
				}
			}
			else if (pPlayer[2]->point1 >= 2 && pPlayer[2]->point2 >= 2 && pPlayer[2]->point4 >= 2) {

				if (OtherDeathCheck(2)) {

					game_system::PlayerWin(pPlayer[2], 2);
					return true;
				}
			}
			else if (pPlayer[3]->point1 >= 2 && pPlayer[3]->point2 >= 2 && pPlayer[3]->point3 >= 2) {

				if (OtherDeathCheck(3)) {

					game_system::PlayerWin(pPlayer[3], 3);
					return true;
				}
			}


			break;

		case BATTLE_STAGE:
			
			//���s
			if (pPlayer[0]->life > 0) {

				if (OtherDeathCheck(0)){

					game_system::PlayerWin(pPlayer[0],0);
					return true;

				}

			}
			else if (pPlayer[1]->life > 0) {

				if (OtherDeathCheck(1)) {

					game_system::PlayerWin(pPlayer[1], 1);
					return true;
				}
			}
			else if (pPlayer[2]->life > 0) {

				if (OtherDeathCheck(2)) {

					game_system::PlayerWin(pPlayer[2], 2);
					return true;
				}
			}
			else if (pPlayer[3]->life > 0) {

				if (OtherDeathCheck(3)) {

					game_system::PlayerWin(pPlayer[3], 3);
					return true;
				}
			}

			break;

		case BATTLEDOME_STAGE:


			if (game_timer <= 0) {

				int min = 999;


				for (int i = 0; i < 4; i++) {

					if (pPlayer[i]->point < min) {

						//�ŏ��t���O�𗧂čŏ��l���X�V
						PminFlag[i] = true;
						min = pPlayer[i]->point;

						for (int j = i - 1; j >= 0; j--) PminFlag[j] = false;
					}
					else if (pPlayer[i]->point == min) {
						PminFlag[i] = true;

					}

				}
				
				if (PminFlag[0]) game_system::PlayerWin(pPlayer[0], 0);
				else if (PminFlag[1]) game_system::PlayerWin(pPlayer[1], 1);
				else if (PminFlag[2]) game_system::PlayerWin(pPlayer[2], 2);
				else if (PminFlag[3]) game_system::PlayerWin(pPlayer[3], 3);

				return true;

			}


			break;


		case ACTION_RACE_STAGE:

			if (game_timer <= 0) {

				int max = 0;


				for (int i = 0; i < 4; i++) {

					if (pPlayer[i]->point > max) {

						//�ŏ��t���O�𗧂čŏ��l���X�V
						PminFlag[i] = true;
						max = pPlayer[i]->point;

						for (int j = i - 1; j >= 0; j--) PminFlag[j] = false;
					}
					else if (pPlayer[i]->point == max) {
						PminFlag[i] = true;

					}

				}

				if (PminFlag[0]) game_system::PlayerWin(pPlayer[0], 0);
				else if (PminFlag[1]) game_system::PlayerWin(pPlayer[1], 1);
				else if (PminFlag[2]) game_system::PlayerWin(pPlayer[2], 2);
				else if (PminFlag[3]) game_system::PlayerWin(pPlayer[3], 3);

				return true;

			}


			break;

		case RACE_STAGE:

			for (int i = 0; i < 4; i++) {

				if (pPlayer[i]->point >= 5) {

					game_system::PlayerWin(pPlayer[i], i);

					return true;
				}
			}

			break;

		case SURVIVAL_STAGE:

			if (game_timer <= 0) {

				int max = 0;


				for (int i = 0; i < 4; i++) {

					if (wall_manager::CheckPaintColor(i + 1) > max) {

						//�ŏ��t���O�𗧂čŏ��l���X�V
						PminFlag[i] = true;
						max = wall_manager::CheckPaintColor(i + 1);

						for (int j = i - 1; j >= 0; j--) PminFlag[j] = false;
					}
					else if (pPlayer[i]->point == max) {
						PminFlag[i] = true;

					}

				}

				if (PminFlag[0]) game_system::PlayerWin(pPlayer[0], 0);
				else if (PminFlag[1]) game_system::PlayerWin(pPlayer[1], 1);
				else if (PminFlag[2]) game_system::PlayerWin(pPlayer[2], 2);
				else if (PminFlag[3]) game_system::PlayerWin(pPlayer[3], 3);

				return true;

			}


			break;
	}

	return false;
}

bool stage::OtherDeathCheck(int i) {

	switch (i)
	{
		case 0:

			//���s
			if (pPlayer[1]->life > 0) return false;
			if (pPlayer[2]->life > 0) return false;
			if (pPlayer[3]->life > 0) return false;

			return true;

			break;

		case 1:

			//���s
			if (pPlayer[0]->life > 0) return false;
			if (pPlayer[2]->life > 0) return false;
			if (pPlayer[3]->life > 0) return false;

			return true;

			break;

		case 2:

			//���s
			if (pPlayer[0]->life > 0) return false;
			if (pPlayer[1]->life > 0) return false;
			if (pPlayer[3]->life > 0) return false;

			return true;

			break;

		case 3:

			//���s
			if (pPlayer[0]->life > 0) return false;
			if (pPlayer[1]->life > 0) return false;
			if (pPlayer[2]->life > 0) return false;

			return true;

			break;
	}

	return false;//�����ɂ͗��Ȃ��͂�
}

void stage::BattleDomePointCheck() {

	bullet* pBullet;

	for (int i = 0; i < BULLET_MAX; i++) {

		pBullet = bullet_manager::GetBullet(i);

		if (pBullet == NULL) continue;

		
		if (pBullet->GetPos().x >= BATTLEDOME_STAGE_SIZE / 2.0f) {
			if(pBullet->GetBulletType() == BALL_BULLET) pPlayer[3]->point++;
			else  pPlayer[3]->point += 3;
			bullet_manager::DestroyBullet(i);
		}
		else if (pBullet->GetPos().x <= -BATTLEDOME_STAGE_SIZE / 2.0f) {
			if (pBullet->GetBulletType() == BALL_BULLET) pPlayer[2]->point++;
			else  pPlayer[2]->point += 3;
			bullet_manager::DestroyBullet(i);
		}
		else if (pBullet->GetPos().z >= BATTLEDOME_STAGE_SIZE / 2.0f) {
			if (pBullet->GetBulletType() == BALL_BULLET) pPlayer[1]->point++;
			else  pPlayer[1]->point += 3;
			bullet_manager::DestroyBullet(i);
		}
		else if (pBullet->GetPos().z <= -BATTLEDOME_STAGE_SIZE / 2.0f) {
			if (pBullet->GetBulletType() == BALL_BULLET) pPlayer[0]->point++;
			else  pPlayer[0]->point += 3;
			bullet_manager::DestroyBullet(i);
		}


	}

}

void stage::TimerCheck() {

	game_timer--;
}

void stage::ActionRaceStageSet() {

	////�����_���Ń|�C���g���Z�b�g


	for (int j = 0; j < 10; j++) {

		float setx, sety,setz;
		setx = (rand() % 40 - 20) / 2;
		sety = (rand() % 40 - 20) / 2;
		setz = (rand() % 40 - 20) / 2;


		bullet_manager::SetPointBullet({setx,sety,setz},0);

	}



}


void stage::UnInit() {

	wall_manager::AllDestroy();
	bullet_manager::AllDestroyBullet();
	effect_manager::AllDestroyEffect();

	for (int i = 0; i < 4;i++) pPlayer[i]->StatesReset();

}

void stage::LapCheck() {

	for (int i = 0; i < 4; i++) {

		////���b�v�|�C���g���Z



		//�z�[���X�g���b�`�@�E
		if (pPlayer[i]->point1 == HOME_STRETCH_RIGHT) {

			if (pPlayer[i]->GetPos().x >= HOME_STRETCH_RIGHT_ENDLINE_X) {
				pPlayer[i]->point1 = FIRST_CORNER;
			}
		}
		else if (pPlayer[i]->point1 == FIRST_CORNER) {//��P�R�[�i�[�`
			
			if (pPlayer[i]->GetPos().z >= FIRST_CORNER_ENDLINE_Z) {
				pPlayer[i]->point1 = BACK_STRETCH;
			}
			
		}
		else if (pPlayer[i]->point1 == BACK_STRETCH) {//�o�b�N�X�g���b�`

			if (pPlayer[i]->GetPos().x <= BACK_STRETCH_X) {
				pPlayer[i]->point1 = THIRD_CORNER;
			}

		}
		else if (pPlayer[i]->point1 == THIRD_CORNER) {//��R�R�[�i�[�`

			if (pPlayer[i]->GetPos().z <= THIRD_CORNER_ENDLINE_Z) {
				pPlayer[i]->point1 = HOME_STRETCH_LEFT;
			}

		}
		else if (pPlayer[i]->point1 == HOME_STRETCH_LEFT) {//�z�[���X�g���b�`�@��

			if (pPlayer[i]->GetPos().x >= HOME_STRETCH_LEFT_ENDLINE_X) {
				pPlayer[i]->point1 = HOME_STRETCH_RIGHT;
				pPlayer[i]->point++;
			}

		}

	}


}