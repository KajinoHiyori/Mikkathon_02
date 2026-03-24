//=============================================================================
// 
// 小惑星管理処理 [asteroid.h]
// Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _ASTEROID_H_
#define _ASTEROID_H_

// 小惑星の種類
typedef enum
{
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_EXPLANTATION,	// 説明状態
	GAMESTATE_CLEAR,		// ゲームクリア
	GAMESTATE_OVER,			// ゲームオーバー
	GAMESTATE_MAX
}GAMESTATE;

// プロトタイプ宣言
void InitAsteroid(void);
void UninitAsteroid(void);
void UpdateAsteroid(void);
void DrawAsteroid(void);
GAMESTATE GetGAmeState(void);
#endif