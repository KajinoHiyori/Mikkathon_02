//======================================================================================
// 
// ケプラーMk3 [game.h]
// Author : Kajino Hiyori
// 
//======================================================================================
#ifndef _GAME_H_
#define _GAME_H_

// ゲームの状態管理
typedef enum
{
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_EXPLANTATION,	// 説明状態
	GAMESTATE_CLEAR,		// ゲームクリア
	GAMESTATE_OVER,			// ゲームオーバー
	GAMESTATE_MAX
}GAMESTATE;

// プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nTime);	// nTimeはフェードするまでの継続時間でGAMESTATE_CLEAR, GAMESTATE_OVERの際にのみ指定が必要
void SetEnablePause(bool bPause);
GAMESTATE GetGAmeState(void);
#endif
