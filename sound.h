//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGMTITLE,		    // BGM 
	SOUND_LABEL_BGMSTAGE,		    // BGM
	//SOUND_LABEL_BGMBOSS,		    // BGM
	SOUND_LABEL_SE_CURSOL,		// �J�[�\���ړ�
	SOUND_LABEL_SE_KETTEI,		// ����
	SOUND_LABEL_SE_COUNTDOWN,		// �J�E���g�_�E��
	SOUND_LABEL_SE_COUNTDOWN_END,	// �J�E���g�_�E���I��
	SOUND_LABEL_SE_EXPLOSION,	// ����
	SOUND_LABEL_SE_HITGETPOINT,	// �|�C���g�擾(�e)
	SOUND_LABEL_SE_MINIGAME_END,	// �~�j�Q�[���I��
	SOUND_LABEL_SE_PAGE,	// �y�[�W�߂���

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
