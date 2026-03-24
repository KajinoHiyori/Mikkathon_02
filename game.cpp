//======================================================================================
// 
// ケプラーMk3 [game.cpp]
// Author : Kajino Hiyori
//
//======================================================================================
#include "game.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "color.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_nCounterGameState = 0;	// ゲームの状態管理カウンター
GAMESTATE g_gameState = GAMESTATE_NONE;	// ゲームの状態を取得

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitGame(void)
{
	// 変数の初期化
	g_gameState = GAMESTATE_EXPLANTATION;
	g_nCounterGameState = 0;
}

//=======================================================
// ゲームの終了処理
//=======================================================
void UninitGame(void)
{

}

//=======================================================
// ゲームの更新処理
//=======================================================
void UpdateGame(void)
{
	FADE fade = GetFade();

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		SetGameState(GAMESTATE_CLEAR, 0);
	}
	else if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetGameState(GAMESTATE_OVER, 0);
	}

	if (g_nCounterGameState <= 0 && g_gameState == GAMESTATE_CLEAR && fade == FADE_NONE)
	{
		SetFade(MODE_RESULT, COLOR_WHITE);
	}
	else if (g_nCounterGameState <= 0 && g_gameState == GAMESTATE_OVER && fade == FADE_NONE)
	{
		SetFade(MODE_RESULT, COLOR_BLACK);
	}
}

//=======================================================
// ゲームの描画処理
//=======================================================
void DrawGame(void)
{

}

//=======================================================
// ゲームの状態を設定
//=======================================================
void SetGameState(GAMESTATE state, int nTime)
{
	g_gameState = state;
	g_nCounterGameState = nTime;
}

//=======================================================
// ゲームの状態を取得
//=======================================================
GAMESTATE GetGAmeState(void)
{
	return g_gameState;
}