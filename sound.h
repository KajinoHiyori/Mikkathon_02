//===========================================================================================
//
// サウンド処理 [sound.h]
// Author : Kajino Hiyori
//
//===========================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	BGM_TITLE = 0,	// タイトル
	BGM_GAME,		// ゲーム
	BGM_CLEAR,		// ゲームクリア
	BGM_OVER,		// ゲームオーバー
	SE_SELECT,		// セレクト
	SE_ENTER,		// 決定
	SE_BOMB,		// プレイヤー死亡
	SE_ROCKBREAK,	// 小惑星破壊
	SE_HOLE,		// ブラックホール/ホワイトホール
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
