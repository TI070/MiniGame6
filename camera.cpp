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
	
	//前方向ベクトルと右方向ベクトルと直角になる上方向ベクトルを算出
	//外積で求めることができる
	D3DXVec3Cross(&g_vUp,&g_vFront,&g_vRight);

	//g_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	//各ベクトルを正規化する
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

	//ビュー座標変換行列の作成
	D3DXMATRIX mtxView;
	//D3DXVECTOR3 eye(2.0f, 10.0f, 3.0f);//カメラ座標
	//D3DXVECTOR3 eye(2.0f, 4.0f, -8.0f);//カメラ座標
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);//見ている場所(注視点)
	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);//上方向ベクトル
	at = g_Position + g_vFront;

	D3DXMatrixLookAtLH(&mtxView, &g_Position, &at, &g_vUp);//(LH)レフトハンドル 左手座標系で計算

	//デバイスにビュー変換行列を設定する
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//プロジェクション(パースペクティブ)変換行列の作成
	D3DXMATRIX mtxProjection;
	float fovy = D3DXToRadian(60);//視野角(片側)
	float aspect = 4.0f / 3.0f;//アスペクト比
	float z_near = 0.1f;
	float z_far = 100.0f;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, fovy, aspect, z_near, z_far);

	//デバイスにプロジェクション変換行列を設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);


}

void Camera_SetMatrixToDevice() {

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	//ビュー座標変換行列の作成
	//D3DXMATRIX mtxView;

	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);//見ている場所(注視点)


	//デバイスにビュー変換行列を設定する
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	//プロジェクション(パースペクティブ)変換行列の作成
	D3DXMATRIX mtxProjection;
	float fovy = D3DXToRadian(60);//視野角(片側)
	float aspect = 4.0f / 3.0f;//アスペクト比
	float z_near = 0.1f;
	float z_far = 100.0f;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, fovy, aspect, z_near, z_far);

	//デバイスにプロジェクション変換行列を設定
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

			//D3DXVec3Transform();...座標変換すると４次元ベクトルで出力(w=1を追加して演算)
			//D3DXVec3TransformCoord();...座標変換すると3次元ベクトルで出力(w=1を追加して演算,w成分を削除)
			//D3DXVec3TransformNormal();座標変換すると3次元ベクトルで出力(w=0を追加して演算,w成分を削除)
										//平行移動成分が0になるため、回転や拡大をしたいが平行移動をしたくないときに使う。。。らしい
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYRIGHT)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
			D3DXMatrixRotationY(&mtxR, g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...座標変換すると４次元ベクトルで出力(w=1を追加して演算)
			//D3DXVec3TransformCoord();...座標変換すると3次元ベクトルで出力(w=1を追加して演算,w成分を削除)
			//D3DXVec3TransformNormal();座標変換すると3次元ベクトルで出力(w=0を追加して演算,w成分を削除)
										//平行移動成分が0になるため、回転や拡大をしたいが平行移動をしたくないときに使う。。。らしい
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYUP)) {
			D3DXMATRIX mtxR;
			D3DXMatrixRotationAxis(&mtxR, &g_vRight, -g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);

			//D3DXVec3Transform();...座標変換すると４次元ベクトルで出力(w=1を追加して演算)
			//D3DXVec3TransformCoord();...座標変換すると3次元ベクトルで出力(w=1を追加して演算,w成分を削除)
			//D3DXVec3TransformNormal();座標変換すると3次元ベクトルで出力(w=0を追加して演算,w成分を削除)
										//平行移動成分が0になるため、回転や拡大をしたいが平行移動をしたくないときに使う。。。らしい
		}
		if (!KeyManager_Press(KM_ALT) && KeyManager_Press(KM_KEYDOWN)) {
			D3DXMATRIX mtxR;
			D3DXMatrixRotationAxis(&mtxR, &g_vRight, g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);

			//D3DXVec3Transform();...座標変換すると４次元ベクトルで出力(w=1を追加して演算)
			//D3DXVec3TransformCoord();...座標変換すると3次元ベクトルで出力(w=1を追加して演算,w成分を削除)
			//D3DXVec3TransformNormal();座標変換すると3次元ベクトルで出力(w=0を追加して演算,w成分を削除)
										//平行移動成分が0になるため、回転や拡大をしたいが平行移動をしたくないときに使う。。。らしい
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

	//特殊
	if (stage::stage_n == ACTION_RACE_STAGE) {
		if (KeyManager_Press(KM_Q)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR,&g_vUp,g_RotationalSpeed);

			D3DXMatrixRotationY(&mtxR, -g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...座標変換すると４次元ベクトルで出力(w=1を追加して演算)
			//D3DXVec3TransformCoord();...座標変換すると3次元ベクトルで出力(w=1を追加して演算,w成分を削除)
			//D3DXVec3TransformNormal();座標変換すると3次元ベクトルで出力(w=0を追加して演算,w成分を削除)
										//平行移動成分が0になるため、回転や拡大をしたいが平行移動をしたくないときに使う。。。らしい
		}
		if (KeyManager_Press(KM_E)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
			D3DXMatrixRotationY(&mtxR, g_RotationalSpeed);

			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
			D3DXVec3TransformNormal(&g_vRight, &g_vRight, &mtxR);

			//D3DXVec3Transform();...座標変換すると４次元ベクトルで出力(w=1を追加して演算)
			//D3DXVec3TransformCoord();...座標変換すると3次元ベクトルで出力(w=1を追加して演算,w成分を削除)
			//D3DXVec3TransformNormal();座標変換すると3次元ベクトルで出力(w=0を追加して演算,w成分を削除)
										//平行移動成分が0になるため、回転や拡大をしたいが平行移動をしたくないときに使う。。。らしい
		}
	}



	//加速度の計算
	D3DXVec3Normalize(&dir,&dir);
	g_MovingSpeed_V3 += dir * MOVING_ACCEL;

	//移動の抑制
	if (D3DXVec3Length(&g_MovingSpeed_V3) > MOVING_SPEED_MAX) {
		D3DXVec3Normalize(&g_MovingSpeed_V3, &g_MovingSpeed_V3);
		g_MovingSpeed_V3 *= MOVING_SPEED_MAX;
	}
	//移動
	g_Position += g_MovingSpeed_V3;
	//速度の減衰
	g_MovingSpeed_V3 *= MOVING_ATTENUATION_RATE;

	//注視点算出
	//at = g_Position + g_vFront;
	//g_At = g_Position + g_vFront * 2.5 + GamePlayer_GetPos();
	
	D3DXVECTOR3 d_pos = g_Position;
	//追従する場合はこれを使う
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
		//固定カメラの場合
		d_pos = g_Position;
		
	}
	D3DXMatrixLookAtLH(&mtxView, &d_pos, &g_At, &g_vUp);//(LH)レフトハンドル 左手座標系で計算


	//D3DXMatrixLookAtLH(&mtxView, &g_Position, &at, &g_vUp);//(LH)レフトハンドル 左手座標系で計算
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

// 各画面のビューポート設定
bool SetUpViewport(int id)
{
	// スクリーン番号の更新
	g_CurrentScreenId = id;

	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;
	//Size screen_size = GetSplitScreenSize();
	//Vec2 offset = GetScreenOriginPos(screen_id);
	// ビューポートの左上座標
	//view_port.X = offset.X;
	//view_port.Y = offset.Y;

	// ビューポートの幅
	view_port.Width = SCREEN_WIDTH;
	// ビューポートの高さ
	view_port.Height = SCREEN_HEIGHT;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	// ビューポート設定
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