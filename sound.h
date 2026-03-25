//===========================================================================================
//
// サウンド処理 [sound.h]
// Author : Kajino Hiyori
//
//===========================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#if 0
//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	BGM_TITLE = 0,	// タイトル
	BGM_TUTORIAL,	// チュートリアル
	BGM_GAME,		// ゲーム
	BGM_CLEAR,		// ゲームクリア
	BGM_OVER,		// ゲームオーバー
	BGM_RANKING,	// ランキング
	BGM_COINRUSH,	// コインラッシュ
	SE_SELECT,		// セレクト
	SE_ENTER,		// 決定
	SE_PAUSE,		// ポーズ
	SE_COIN,		// コイン獲得
	SE_KEY,			// カギ獲得
	SE_JUMP,		// ジャンプ
	SE_LANDING,		// 着地
	SE_ADDTIME,		// 時間追加
	SE_FALLCOIN,	// コイン落下
	SE_GROWUP,		// 成長
	SE_CAMERA,		// カメラ操作
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
#endif