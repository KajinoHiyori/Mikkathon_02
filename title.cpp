//======================================================================================
// 
// ケプラーMk3 [fade.cpp]
// Author : Kajino Hiyori
//
//======================================================================================
#include "title.h"
#include "fade.h"
#include "main.h"
#include "input.h"
#include "color.h"
#include "camera.h"

#include "title_ui.h"
#include "bg.h"
#include "meshcylinder.h"

//=======================================================
// タイトルの初期化処理
//=======================================================
void InitTitle(void)
{
	SetCameraPos(0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_STOP);

	// タイトルUIの初期化処理
	InitTitleUI();
}

//=======================================================
// タイトルの終了処理
//=======================================================
void UninitTitle(void)
{
	// タイトルUIの終了処理
	UninitTitleUI();
}

//=======================================================
// タイトルの更新処理
//=======================================================
void UpdateTitle(void)
{
	// タイトルUIの更新処理
	UpdateTitleUI();

	// メッシュシリンダーの更新処理
	UpdateMeshCylinder();

	// 背景の更新処理
	UpdateBG();

	FADE fade = GetFade();
	if (fade == FADE_NONE &&
		GetTitleFinish() == true)
	{
		SetFade(MODE_GAME, COLOR_WHITE);
	}
}

//=======================================================
// タイトルの描画処理
//=======================================================
void DrawTitle(void)
{
	// 背景の描画処理
	DrawBG();

	// メッシュシリンダーの描画処理
	DrawMeshCylinder();

	// タイトルUIの描画処理
	DrawTitleUI();
}