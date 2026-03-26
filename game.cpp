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
#include "player.h"
#include "camera.h"
#include "pause.h"
#include "stage.h"
#include "planet.h"
#include "asteroid.h"
#include "oil.h"
#include "meshcylinder.h"
#include "bg.h"
#include "explantation.h"
#include "effect_3d.h"
#include "particle_3d.h"
#include "explosion.h"
#include "sound.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
bool g_bPause = false;	// ポーズ状態を管理
bool g_bDisp = true;	// ポーズメニューの表示状態
int g_nCounterGameState = 0;	// ゲームの状態管理カウンター
GAMESTATE g_gameState = GAMESTATE_NONE;	// ゲームの状態を取得

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitGame(void)
{
	// プレイヤーの初期化処理
	InitPlayer();

	// グローバル変数の初期化
	g_gameState = GAMESTATE_EXPLANTATION;
	g_nCounterGameState = 0;
	g_bPause = false;
	g_bDisp = true;

	// プレイヤーの設定
	SetPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYERSTATE_APPEAR);

	// カメラの設定
	SetCameraPos(0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_PLAYER);

	// 惑星の初期化
	InitPlanet();

	// 小惑星の初期化
	InitAsteroid();

	// ポーズ状態の初期化
	InitPause();

	// 説明UIの初期化
	InitExplantation();

	// 音楽の再生
	PlaySound(BGM_GAME);
}

//=======================================================
// ゲームの終了処理
//=======================================================
void UninitGame(void)
{
	// ポーズ状態の終了処理
	UninitPause();

	// 説明UIの終了処理
	UninitExplantation();

	// 音楽の停止
	StopSound();
}

//=======================================================
// ゲームの更新処理
//=======================================================
void UpdateGame(void)
{
	// ポーズ機能
	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && g_gameState != GAMESTATE_CLEAR && g_gameState != GAMESTATE_OVER)
	{
		PlaySound(SE_ENTER);
		SetPause(PAUSE_MENU_CONTINUE);
		g_bPause = g_bPause ? false : true;
	}
	// ポーズの表示非表示
	if (g_bPause == true && GetKeyboardTrigger(DIK_F4) == true)
	{
		g_bDisp = g_bDisp ? false : true;
	}

	if (g_bPause == false)
	{ // pause状態ではない
		// プレイヤーの更新処理
		UpdatePlayer();

		// 惑星の初期化
		UpdatePlanet();

		// 小惑星の更新処理
		UpdateAsteroid();

		// 燃料の更新処理
		UpdateOil();

		// エフェクトの更新処理
		UpdateEffect3D();

		// パーティクルの更新処理
		UpdateParticle3D();

		// 爆発の更新処理
		UpdateExplosion();

		// メッシュシリンダーの更新処理
		UpdateMeshCylinder();

		// 背景の更新処理
		UpdateBG();

		// 説明UIの更新処理
		UpdateExplantation();
	}
	else if (g_bPause == true)
	{ // pause状態
		// ポーズの更新処理
		UpdatePause();
	}

	// カウンターの減少
	g_nCounterGameState--;

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		SetGameState(GAMESTATE_CLEAR, 0);
	}
	else if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetGameState(GAMESTATE_OVER, 0);
	}

	FADE fade = GetFade();
	if (g_nCounterGameState <= 0 && g_gameState == GAMESTATE_CLEAR && fade == FADE_NONE)
	{
		SetFade(MODE_RESULT, COLOR_WHITE);
	}
	else if (g_nCounterGameState <= 0 && g_gameState == GAMESTATE_OVER && fade == FADE_NONE)
	{
		SetFade(MODE_RESULT, COLOR_BLACK);
	}

	if (g_nCounterGameState <= 0)
	{
		g_nCounterGameState = 0;
	}

	PrintDebugProc("状態[%d]のゲーム状態カウンター %d\n", g_gameState, g_nCounterGameState);
}

//=======================================================
// ゲームの描画処理
//=======================================================
void DrawGame(void)
{
	// 背景の描画処理
	DrawBG();

	// プレイヤーの描画処理
	DrawPlayer();

	// 惑星の描画
	DrawPlanet();

	// 小惑星の描画処理
	DrawAsteroid();

	// メッシュシリンダーの描画処理
	DrawMeshCylinder();

	// エフェクトの描画処理
	DrawEffect3D();

	// パーティクルの描画処理
	DrawParticle3D();

	// 爆発の描画処理
	DrawExplosion();

	// 説明UIの描画処理
	DrawExplantation();

	if (g_bDisp == true && g_bPause == true)
	{
		// ポーズの描画処理
		DrawPause();
	}
}

//======================================================================================
// ポーズ状態の変更
//======================================================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
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