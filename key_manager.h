#pragma once
#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include <d3d9.h>
#include <d3dx9.h> 

//アプリケーションで使う操作を列挙
typedef enum KeyKind_tag {
	
	KM_UP,
	KM_DOWN,
	KM_LEFT,
	KM_RIGHT,
	KM_ENTER,
	KM_CANCEL,
	KM_CHANGE,
	KM_ALT,
	KM_KEYUP,
	KM_KEYDOWN,
	KM_KEYLEFT,
	KM_KEYRIGHT,
	KM_WIRE,
	KM_MODECHANGE,
	KM_E,
	KM_MAX,
	KM_Q = KM_CANCEL,
	KM_ATTACK = KM_ENTER,
	KM_BOMB = KM_CANCEL,
	KM_SPACE = KM_ALT

}KeyKind;


void KeyManager_Initilalize();

void KeyManager_Finalize();

void KeyManager_Update();


bool KeyManager_Press(KeyKind KM);//押してたら
bool KeyManager_Trigger(KeyKind KM);//押した瞬間
bool KeyManager_Release(KeyKind KM);//離した瞬間

void KeyManager_RecordStart(int frame_max);
void KeyManager_RecordEnd();
void KeyManager_RecordPlay();
bool KeyManager_IsRecordPlay();
void KeyManager_RecordLoad();

#endif


