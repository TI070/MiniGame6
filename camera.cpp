#include <d3dx9.h>
#include <stdio.h>
#include "mydirect3d.h"
#include "camera.h"
#include "key_manager.h"
#include "game_player.h"
#include "player_manager.h"
#include "config.h"
#include "stage.h"

static D3DXVECTOR3 g_vFront(0.0f, -0.5f, 1.0f);
static D3DXVECTOR3 g_vRight(1.0f,  0.0f, 0.0f);
static D3DXVECTOR3 g_vUp(0.0f, 1.0f, 0.0f);
static D3DXVECTOR3 g_Position(0.0f, 0.0f, -5.0f);
static D3DXVECTOR3 g_At(0.0f, 0.0f, 0.0f);
static float g_MovingSpeed = 0.0f;
static D3DXVECTOR3 g_MovingSpeed_V3(0.0f, 0.0f, 0.0f);
static D3DXMATRIX mtxView;

static int camera_mode;

#define MOVING_SPEED_MAX (0.1f)
#define MOVING_ACCEL (0.08f)
#define MOVING_ATTENUATION_RATE (0.8f)

float g_RotationalSpeed = 0.0f;

int g_CurrentScreenId;

void Camera_Initialize() {

	g_vFront = D3DXVECTOR3(0.0f,-0.5f,1.0f);
	g_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	
	//�O�����x�N�g���ƉE�����x�N�g���ƒ��p�ɂȂ������x�N�g�����Z�o
	//�O�ςŋ��߂邱�Ƃ��ł���
	D3DXVec3Cross(&g_vUp,&g_vFront,&g_vRight);

	//g_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	//�e�x�N�g���𐳋K������
	D3DXVec3Normalize(&g_vFront,&g_vFront);
	D3DXVec3Normalize(&g_vRight, &g_vRight);
	D3DXVec3Normalize(&g_vUp, &g_vUp);


	g_Position = D3DXVECTOR3(0.0f, 3.5f, -10.0f);
	g_MovingSpeed = 0.1f;
	g_MovingSpeed_V3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//CameraPos_x = CameraPos_y = CameraPos_z = 0;
	g_RotationalSpeed = 0.03f;

	D3DXMatrixIdentity(&mtxView);

	camera_mode = 0;
}




void SetCamera() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	//�r���[���W�ϊ��s��̍쐬
	D3DXMATRIX mtxView;
	//D3DXVECTOR3 eye(2.0f, 10.0f, 3.0f);//�J�������W
	//D3DXVECTOR3 eye(2.0f, 4.0f, -8.0f);//�J�������W
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);//���Ă���ꏊ(�����_)
	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);//������x�N�g��
	at = g_Position + g_vFront;

	D3DXMatrixLookAtLH(&mtxView, &g_Position, &at, &g_vUp);//(LH)���t�g�n���h�� ������W�n�Ōv�Z

	//�f�o�C�X�Ƀr���[�ϊ��s���ݒ肷��
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//�v���W�F�N�V����(�p�[�X�y�N�e�B�u)�ϊ��s��̍쐬
	D3DXMATRIX mtxProjection;
	float fovy = D3DXToRadian(60);//����p(�Б�)
	float aspect = 4.0f / 3.0f;//�A�X�y�N�g��
	float z_near = 0.1f;
	float z_far = 100.0f;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, fovy, aspect, z_near, z_far);

	//�f�o�C�X�Ƀv���W�F�N�V�����ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);


}

void Camera_SetMatrixToDevice() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	//�r���[���W�ϊ��s��̍쐬
	//D3DXMATRIX mtxView;

	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);//���Ă���ꏊ(�����_)


	//�f�o�C�X�Ƀr���[�ϊ��s���ݒ肷��
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//�v���W�F�N�V����(�p�[�X�y�N�e�B�u)�ϊ��s��̍쐬
	D3DXMATRIX mtxProjection;
	float fovy = D3DXToRadian(60);//����p(�Б�)
	float aspect = 4.0f / 3.0f;//�A�X�y�N�g��
	float z_near = 0.1f;
	float z_far = 100.0f;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, fovy, aspect, z_near, z_far);

	//�f�o�C�X�Ƀv���W�F�N�V�����ϊ��s���ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);




}


void Camera_Update(bool move_on) {

	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
	if (move_on) {
		if (KeyManager_Press(KM_UP)) {
			D3DXVECTOR3 v = g_vFront;
			v.y = 0.0f;
			D3DXVec3Normalize(&v, &v);
			//g_Position += v * g_MovingSpeed;
			//g_MovingSpeed_V3 += v * MOVING_ACCEL;
			dir += v;
		}
		if (KeyManager_Press(KM_DOWN)) {
			D3DXVECTOR3 v = g_vFront;
			v.y = 0.0f;
			D3DXVec3Normalize(&v, &v);
			//g_Position -= v * g_MovingSpeed;
			//g_MovingSpeed_V3 -= v * MOVING_ACCEL;
			dir -= v;
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_RIGHT)) {

			//g_Position += g_vRight * g_MovingSpeed;
			//g_MovingSpeed_V3 += g_vRight * MOVING_ACCEL;
			dir += g_vRight;
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_LEFT)) {

			//g_Position -= g_vRight * g_MovingSpeed;
			//g_MovingSpeed_V3 -= g_vRight * MOVING_ACCEL;
			dir -= g_vRight;
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_ATTACK)) {
			D3DXVECTOR3 v(0.0f, g_vUp.y, 0.0f);
			D3DXVec3Normalize(&v, &v);
			//g_Position += v * g_MovingSpeed;
			dir += v;
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_BOMB)) {
			D3DXVECTOR3 v(0.0f, g_vUp.y, 0.0f);
			D3DXVec3Normalize(&v, &v);
			//g_Position -= v * g_MovingSpeed;
			dir -= v;
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYLEFT)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR,&g_vUp,g_RotationalSpeed);

			D3DXMatrixRotationY(&mtxR, -g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...���W�ϊ�����ƂS�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z)
			//D3DXVec3TransformCoord();...���W�ϊ������3�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z,w�������폜)
			//D3DXVec3TransformNormal();���W�ϊ������3�����x�N�g���ŏo��(w=0��ǉ����ĉ��Z,w�������폜)
										//���s�ړ�������0�ɂȂ邽�߁A��]��g��������������s�ړ����������Ȃ��Ƃ��Ɏg���B�B�B�炵��
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYRIGHT)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
			D3DXMatrixRotationY(&mtxR, g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...���W�ϊ�����ƂS�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z)
			//D3DXVec3TransformCoord();...���W�ϊ������3�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z,w�������폜)
			//D3DXVec3TransformNormal();���W�ϊ������3�����x�N�g���ŏo��(w=0��ǉ����ĉ��Z,w�������폜)
										//���s�ړ�������0�ɂȂ邽�߁A��]��g��������������s�ړ����������Ȃ��Ƃ��Ɏg���B�B�B�炵��
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYUP)) {
			D3DXMATRIX mtxR;
			D3DXMatrixRotationAxis(&mtxR, &g_vRight, -g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);

			//D3DXVec3Transform();...���W�ϊ�����ƂS�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z)
			//D3DXVec3TransformCoord();...���W�ϊ������3�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z,w�������폜)
			//D3DXVec3TransformNormal();���W�ϊ������3�����x�N�g���ŏo��(w=0��ǉ����ĉ��Z,w�������폜)
										//���s�ړ�������0�ɂȂ邽�߁A��]��g��������������s�ړ����������Ȃ��Ƃ��Ɏg���B�B�B�炵��
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYDOWN)) {
			D3DXMATRIX mtxR;
			D3DXMatrixRotationAxis(&mtxR, &g_vRight, g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);

			//D3DXVec3Transform();...���W�ϊ�����ƂS�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z)
			//D3DXVec3TransformCoord();...���W�ϊ������3�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z,w�������폜)
			//D3DXVec3TransformNormal();���W�ϊ������3�����x�N�g���ŏo��(w=0��ǉ����ĉ��Z,w�������폜)
										//���s�ړ�������0�ɂȂ邽�߁A��]��g��������������s�ړ����������Ȃ��Ƃ��Ɏg���B�B�B�炵��
		}
		if (KeyManager_Press(KM_ALT)) {
			D3DXVECTOR3 g_At = g_Position + g_vFront * 2.5f;

			if (KeyManager_Press(KM_KEYLEFT)) {
				D3DXMATRIX mtxR;
				//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
				D3DXMatrixRotationY(&mtxR, -g_RotationalSpeed);

				D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
				D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
				D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);
			}
			if (KeyManager_Press(KM_KEYRIGHT)) {
				D3DXMATRIX mtxR;
				//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
				D3DXMatrixRotationY(&mtxR, g_RotationalSpeed);

				D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
				D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
				D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);
			}
			if (KeyManager_Press(KM_KEYUP)) {
				D3DXMATRIX mtxR;
				D3DXMatrixRotationAxis(&mtxR, &g_vRight, -g_RotationalSpeed);

				D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
				D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);

			}
			if (KeyManager_Press(KM_KEYDOWN)) {
				D3DXMATRIX mtxR;
				D3DXMatrixRotationAxis(&mtxR, &g_vRight, g_RotationalSpeed);

				D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
				D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);

			}



			g_Position = g_At - g_vFront * 2.5f;


		}
	}

	//����
	if (stage::stage_n == ACTION_RACE_STAGE) {
		if (KeyManager_Press(KM_Q)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR,&g_vUp,g_RotationalSpeed);

			D3DXMatrixRotationY(&mtxR, -g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...���W�ϊ�����ƂS�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z)
			//D3DXVec3TransformCoord();...���W�ϊ������3�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z,w�������폜)
			//D3DXVec3TransformNormal();���W�ϊ������3�����x�N�g���ŏo��(w=0��ǉ����ĉ��Z,w�������폜)
										//���s�ړ�������0�ɂȂ邽�߁A��]��g��������������s�ړ����������Ȃ��Ƃ��Ɏg���B�B�B�炵��
		}
		if (KeyManager_Press(KM_E)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
			D3DXMatrixRotationY(&mtxR, g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...���W�ϊ�����ƂS�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z)
			//D3DXVec3TransformCoord();...���W�ϊ������3�����x�N�g���ŏo��(w=1��ǉ����ĉ��Z,w�������폜)
			//D3DXVec3TransformNormal();���W�ϊ������3�����x�N�g���ŏo��(w=0��ǉ����ĉ��Z,w�������폜)
										//���s�ړ�������0�ɂȂ邽�߁A��]��g��������������s�ړ����������Ȃ��Ƃ��Ɏg���B�B�B�炵��
		}
	}



	//�����x�̌v�Z
	D3DXVec3Normalize(&dir,&dir);
	g_MovingSpeed_V3 += dir * MOVING_ACCEL;

	//�ړ��̗}��
	if (D3DXVec3Length(&g_MovingSpeed_V3) > MOVING_SPEED_MAX) {
		D3DXVec3Normalize(&g_MovingSpeed_V3, &g_MovingSpeed_V3);
		g_MovingSpeed_V3 *= MOVING_SPEED_MAX;
	}
	//�ړ�
	g_Position += g_MovingSpeed_V3;
	//���x�̌���
	g_MovingSpeed_V3 *= MOVING_ATTENUATION_RATE;

	//�����_�Z�o
	//at = g_Position + g_vFront;
	//g_At = g_Position + g_vFront * 2.5 + GamePlayer_GetPos();
	
	D3DXVECTOR3 d_pos = g_Position;
	//�Ǐ]����ꍇ�͂�����g��
	if (camera_mode == FOLLOW_CAMERA) {

		if (stage::stage_n == ACTION_RACE_STAGE) {
			D3DXVECTOR3 plus_pos = player_manager::GetPlayer(0)->GetPos();
			//plus_pos.y = 0.5;
			D3DXVECTOR3 v = g_vFront;
			v.y = 0.0f;
			D3DXVECTOR3 r = g_vRight;

			
			g_At = v * 1.5 + plus_pos;
			d_pos = plus_pos - v * 3.0;// + r * 2.5 + plus_pos;
			//g_At = g_Position + g_vFront * 2.5 + plus_pos;

		}
		else {
			D3DXVECTOR3 plus_pos = player_manager::GetPlayer(0)->GetPos();
			//plus_pos.y = 0.5;

			g_At = g_Position + g_vFront * 2.5 + plus_pos;
			d_pos = g_Position + plus_pos;
		}
	}
	else {
		//�Œ�J�����̏ꍇ
		d_pos = g_Position;
		
	}
	D3DXMatrixLookAtLH(&mtxView, &d_pos, &g_At, &g_vUp);//(LH)���t�g�n���h�� ������W�n�Ōv�Z


	//D3DXMatrixLookAtLH(&mtxView, &g_Position, &at, &g_vUp);//(LH)���t�g�n���h�� ������W�n�Ōv�Z
	//D3DXVECTOR3 d_pos = g_Position + GamePlayer_GetPos();

}

const D3DXVECTOR3* Camera_GetFrontVector() {

	return &g_vFront;
}

const D3DXVECTOR3* Camera_GetRightVector() {

	return &g_vRight;
}

const D3DXVECTOR3* Camera_GetPosition() {

	return &g_Position;
}

const D3DXVECTOR3* Camera_GetAt() {

	return &g_At;
}

const D3DXMATRIX& Camera_GetViewMatrix() {

	return mtxView;
}

void Camera_SetPosition(D3DXVECTOR3 set_pos) {

	g_Position = set_pos;
}

void Camera_SetAt(D3DXVECTOR3 set_at) {

	g_At = set_at;
}

// �e��ʂ̃r���[�|�[�g�ݒ�
bool SetUpViewport(int id)
{
	// �X�N���[���ԍ��̍X�V
	g_CurrentScreenId = id;

	// �r���[�|�[�g�p�����[�^
	D3DVIEWPORT9 view_port;
	//Size screen_size = GetSplitScreenSize();
	//Vec2 offset = GetScreenOriginPos(screen_id);
	// �r���[�|�[�g�̍�����W
	//view_port.X = offset.X;
	//view_port.Y = offset.Y;

	// �r���[�|�[�g�̕�
	view_port.Width = SCREEN_WIDTH;
	// �r���[�|�[�g�̍���
	view_port.Height = SCREEN_HEIGHT;
	// �r���[�|�[�g�[�x�ݒ�
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	// �r���[�|�[�g�ݒ�
	if (FAILED(pDevice->SetViewport(&view_port)))
	{
		return false;
	}

	return true;
}

void SetCameraMode(int mode) {

	camera_mode = mode;
}

int GetCameraMode() {

	return camera_mode;
}

void SetCameraVec() {

	g_vFront = D3DXVECTOR3(0.0f, -0.5f, 1.0f);
	g_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&g_vUp, &g_vFront, &g_vRight);

}