//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGMTITLE,		    // BGM 
	SOUND_LABEL_BGMSTAGE,		    // BGM
	//SOUND_LABEL_BGMBOSS,		    // BGM
	SOUND_LABEL_SE_CURSOL,		// カーソル移動
	SOUND_LABEL_SE_KETTEI,		// 決定
	SOUND_LABEL_SE_COUNTDOWN,		// カウントダウン
	SOUND_LABEL_SE_COUNTDOWN_END,	// カウントダウン終了
	SOUND_LABEL_SE_EXPLOSION,	// 爆発
	SOUND_LABEL_SE_HITGETPOINT,	// ポイント取得(弾)
	SOUND_LABEL_SE_MINIGAME_END,	// ミニゲーム終了
	SOUND_LABEL_SE_PAGE,	// ページめくり

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
