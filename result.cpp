//======================================================================================
// 
// ケプラーMk3 [result.cpp]
// Author : Kajino Hiyori
//
//======================================================================================
#include "result.h"
#include "fade.h"
#include "main.h"
#include "input.h"
#include "color.h"
#include "camera.h"

#include "result_ui.h"
#include "bg.h"
#include "player.h"
#include "planet.h"
#include "effect_3d.h"
#include "particle_3d.h"
#include "explosion.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_nCounterState = 0;	// ゲームの状態管理カウンター

//=======================================================
// リザルトの初期化処理
//=======================================================
void InitResult(void)
{
	// 変数の初期化
	g_nCounterState = 0;

	// カメラの位置設定
	SetCameraPos(0, D3DXVECTOR3(0.0f, 200.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_POINT);

	// プレイヤーの初期化
	SetPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.4f, D3DX_PI * 0.2f, D3DX_PI * 0.2f), PLAYERSTATE_RESULT);

	// 惑星の初期化
	InitPlanet();

	// リザルトUIの初期化
	InitResultUI();
}

//=======================================================
// リザルトの終了処理
//=======================================================
void UninitResult(void)
{
	// リザルトUIの終了処理
	UninitResultUI();
}

//=======================================================
// リザルトの更新処理
//=======================================================
void UpdateResult(void)
{
	// 背景の更新処理
	UpdateBG();

	// プレイヤーの更新処理
	UpdatePlayer();

	// 惑星の更新処理
	UpdatePlanet();

	// エフェクトの更新処理
	UpdateEffect3D();

	// パーティクルの更新処理
	UpdateParticle3D();

	// 爆発の更新処理
	UpdateExplosion();

	// リザルトUIの更新処理
	UpdateResultUI();

	FADE fade = GetFade();
	if (fade == FADE_NONE && ((GetKeyboardTrigger(DIK_RETURN) == true) || GetJoypadTrigger(JOYKEY_A) == true || GetJoypadTrigger(JOYKEY_START) == true))
	{
		SetFade(MODE_TITLE, COLOR_BLACK);
	}
}

//=======================================================
// リザルトの描画処理
//=======================================================
void DrawResult(void)
{
	// 背景の描画処理
	DrawBG();

	// プレイヤーの描画処理
	DrawPlayer();

	// 惑星の描画処理
	DrawPlanet();

	// エフェクトの描画処理
	DrawEffect3D();

	// パーティクルの描画処理
	DrawParticle3D();

	// 爆発の描画処理
	DrawExplosion();

	// リザルトUIの描画処理
	DrawResultUI();
}