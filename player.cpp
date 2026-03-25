//=============================================================================
// 
// プレイヤー [player.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include "asteroid.h"
#include "explosion.h"
#include "particle_3d.h"
#include "color.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVEMENT				(D3DXVECTOR3(1.0f, 1.0f, 1.0f))			// 移動量
#define ROT						(D3DXVECTOR3(0.05f, 0.05f, 0.05f))		// 向き移動量
#define INERTIA_MOVE			(0.4f)									// 移動の慣性
#define INERTIA_ANGLE			(0.1f)									// 角度の慣性
#define MINUS_OIL				(0.05f)									// 燃料の減り方
#define BREAK_OIL				(20.0f)									// 壊せる燃料の量
#define AUTO_SPEED				(1.0f)									// 自動で進むスピード
#define HIGH_SPPED				(35.0f)									// スピードが速くなる
#define LOW_SPPED				(70.0f)									// スピードが遅くなる
#define FIRE_COUNT				(60)									// 火花の間隔
#define SMOKE_COUNT				(20)									// 煙の間隔
#define SPACE_ROTATE			(0.01f)									// 宇宙を漂っているときの回転
#define PLAYER_FILE				"data\\MODEL\\rocket000.x"				// プレイヤーのファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player g_Player;				// プレイヤーの情報
Player_Model g_PlayerModel;		// プレイヤーのモデル		

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
void InitPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ
	D3DXMATERIAL* pMat;

	// プレイヤーの情報の初期化
	g_Player.pos = FIRST_POS;
	g_Player.move = FIRST_POS;
	g_Player.rot = FIRST_POS;
	g_Player.fAngleZ = 0.0f;
	g_Player.Speed = FIRST_POS;
	g_Player.state = PLAYERSTATE_APPEAR;
	g_Player.nCounterState = 0;
	g_Player.fOil = MAX_OIL * 0.5f;
	g_Player.planetType = (PLANETTYPE)-1;
	g_Player.bClear = false;
	g_Player.bBreak = false;
	g_Player.bUse = false;

	if (g_PlayerModel.pMesh == NULL)
	{// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(PLAYER_FILE,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_PlayerModel.pBuffMat,
			NULL,
			&g_PlayerModel.dwNumMat,
			&g_PlayerModel.pMesh)))
		{// NULLチェック
			return;
		}

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_PlayerModel.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_PlayerModel.dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_PlayerModel.apTexture[nCntMat]);
			}
		}
	}
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void UninitPlayer(void)
{
	// メッシュの破棄
	if (g_PlayerModel.pMesh != NULL)
	{
		g_PlayerModel.pMesh->Release();
		g_PlayerModel.pMesh = NULL;
	}

	// マテリアルの破棄
	if (g_PlayerModel.pBuffMat != NULL)
	{
		g_PlayerModel.pBuffMat->Release();
		g_PlayerModel.pBuffMat = NULL;
	}

	// テクスチャの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_TEXTURE; nCntPlayer++)
	{
		if (g_PlayerModel.apTexture[nCntPlayer] != NULL)
		{
			g_PlayerModel.apTexture[nCntPlayer]->Release();
			g_PlayerModel.apTexture[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_Player.bUse == false)
	{// 使っていない状態のときはスキップ
		return;
	}

	PrintDebugProc("playerの使用状態はtrue\n");
	Camera* pCamera = GetCamera();

	switch (g_Player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_Player.nCounterState--;

		if (g_Player.nCounterState < 0)
		{// 動けるようになる
			g_Player.state = PLAYERSTATE_WAIT;
			g_Player.nCounterState = 0;
		}

		break;

	case PLAYERSTATE_WAIT:
		break;

	case PLAYERSTATE_RESULT:
		g_Player.nCounterState++;

		if (g_Player.bClear == false)
		{// ゲームオーバー中の演出

			if (g_Player.nCounterState % FIRE_COUNT == (rand() % FIRE_COUNT))
			{// 火花
				SetParticle3D(rand() % 3 + 1, 1, g_Player.pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), FIRST_POS, 4.0f, 10, 1.2f, 0.0f, EFFECTTYPE_NORMAL, PATICLETYPE_NOMAL, 0);
				SetParticle3D(rand() % 3 + 1, 1, g_Player.pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), FIRST_POS, 4.0f, 10, 1.2f, 0.0f, EFFECTTYPE_NORMAL, PATICLETYPE_NOMAL, 0);
			}

			if (g_Player.nCounterState % SMOKE_COUNT == 0)
			{// 煙
				SetExplosion(g_Player.pos, D3DXVECTOR3((float)(rand() % 2), 1.0f, (float)(rand() % 2)), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 20.0f, EXPLOSIONTYPE_1);
				SetExplosion(g_Player.pos, D3DXVECTOR3((float)(rand() % 2), 1.0f, (float)(rand() % 2)), D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), 20.0f, EXPLOSIONTYPE_1);
			}

			if (g_Player.planetType == -1)
			{// 惑星にぶつかって死んだわけではない→宇宙を漂っている演出
				g_Player.rot.x += SPACE_ROTATE;
				g_Player.rot.y += SPACE_ROTATE;
				g_Player.rot.z += SPACE_ROTATE;

				CorrectAngle(&g_Player.rot.x, g_Player.rot.x);
				CorrectAngle(&g_Player.rot.y, g_Player.rot.y);
				CorrectAngle(&g_Player.rot.z, g_Player.rot.z);
			}
		}

		break;
	}

	if (g_Player.state != PLAYERSTATE_RESULT)
	{
		if (g_Player.state != PLAYERSTATE_APPEAR)
		{// 出現状態のときは移動できない
			float fValueH, fValueV;

			// パッド移動
			if (GetJoypadLeftStickValue(&fValueH, &fValueV) == true)
			{// パッドの移動優先
				g_Player.fAngleZ = atan2f((float)(-fValueH), (float)(fValueV));

				g_Player.move.x += fValueH * (MOVEMENT.x + g_Player.Speed.x);
				g_Player.move.y += fValueV * (MOVEMENT.y + g_Player.Speed.y);

				//g_Player.move.z += cosf(fAngle + pCamera->rot.y) * MOVEMENT.z /** sinf((D3DX_PI * 0.5f) + pCamera->fAngle)*/;
			}
			else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)		// キーボード移動
			{// 上に移動
				if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
				{// 左上に移動
					g_Player.move.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);
					g_Player.move.y += cosf((D3DX_PI * 0.75f)) * -(MOVEMENT.y + g_Player.Speed.y);
					//g_Player.move.z += cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * MOVEMENT.z;

					g_Player.fAngleZ = pCamera->rot.y + (D3DX_PI * 0.5f);
				}
				else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
				{// 右上に移動
					g_Player.move.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);
					g_Player.move.y += cosf((D3DX_PI * 0.75f)) * -(MOVEMENT.y + g_Player.Speed.y);
					//g_Player.move.z += cosf(D3DX_PI * 0.25f + pCamera->rot.y) * MOVEMENT.z;

					g_Player.fAngleZ = pCamera->rot.y - (D3DX_PI * 0.5f);
				}
				else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
				{// 上に移動
					g_Player.move.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);
					g_Player.move.y += cosf((D3DX_PI * 1.0f)) * -(MOVEMENT.y + g_Player.Speed.y);
					//g_Player.move.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * MOVEMENT.z;

					g_Player.fAngleZ = 0.0f;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{// 下に移動
				if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
				{// 左下に移動
					g_Player.move.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);
					g_Player.move.y += cosf((D3DX_PI * 0.25f)) * -(MOVEMENT.y + g_Player.Speed.y);
					//g_Player.move.z += cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * MOVEMENT.z;

					g_Player.fAngleZ = pCamera->rot.y + (D3DX_PI * 0.5f);
				}
				else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
				{// 右下に移動
					g_Player.move.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);
					g_Player.move.y += cosf((D3DX_PI * 0.25f)) * -(MOVEMENT.y + g_Player.Speed.y);
					//g_Player.move.z += cosf(D3DX_PI * 0.75f + pCamera->rot.y) * MOVEMENT.z;

					g_Player.fAngleZ = pCamera->rot.y - (D3DX_PI * 0.5f);
				}
				else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
				{// 下に移動
					g_Player.move.x += sinf(D3DX_PI * 1.0f + pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);
					g_Player.move.y += cosf((D3DX_PI * 0.0f)) * -(MOVEMENT.y + g_Player.Speed.y);
					//g_Player.move.z += cosf(D3DX_PI * 1.0f + pCamera->rot.y) * MOVEMENT.z;

					g_Player.fAngleZ = 0.0f;
				}
			}
			else if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
			{// 左に移動
				g_Player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);

				g_Player.fAngleZ = pCamera->rot.y + (D3DX_PI * 0.5f);
			}
			else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
			{// 右に移動
				g_Player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * (MOVEMENT.x + g_Player.Speed.x);

				g_Player.fAngleZ = pCamera->rot.y - (D3DX_PI * 0.5f);
			}
			else
			{// 動いてないとき
				g_Player.fAngleZ = 0.0f;
			}
		}

		SetEffect3D(3, D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z - 8.0f), FIRST_POS, 0.0f, 7.0f, -1.0f, COLOR_RED, EFFECTTYPE_NORMAL);
		SetEffect3D(3, D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z - 8.0f), FIRST_POS, 0.0f, 7.0f, -1.0f, COLOR_ORANGE, EFFECTTYPE_NORMAL);

		if (g_Player.fOil <= HIGH_SPPED)
		{// 速い
			g_Player.Speed.x = 1.5f;
			g_Player.Speed.y = 1.5f;
			g_Player.Speed.z = 1.5f;
		}
		else if (g_Player.fOil <= LOW_SPPED)
		{// 普通
			g_Player.Speed.x = 0.0f;
			g_Player.Speed.y = 0.0f;
			g_Player.Speed.z = 0.0f;
		}
		else
		{// 遅い
			g_Player.Speed.x = -0.5f;
			g_Player.Speed.y = -0.5f;
			g_Player.Speed.z = -0.5f;
		}

		g_Player.move.z += AUTO_SPEED + g_Player.Speed.z;

		// 慣性
		g_Player.pos += g_Player.move;
		g_Player.move.x += (0.0f - g_Player.move.x) * INERTIA_MOVE;
		g_Player.move.y += (0.0f - g_Player.move.y) * INERTIA_MOVE;
		g_Player.move.z += (0.0f - g_Player.move.z) * INERTIA_MOVE;

		PrintDebugProc("プレイヤーのpos : ( %f %f %f )\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		PrintDebugProc("プレイヤーのmove : ( %f %f %f )\n", g_Player.move.x, g_Player.move.y, g_Player.move.z);

		if (CollisionAsteroid(g_Player.pos, g_Player.bBreak) == true)
		{// 小惑星との当たり判定による演出
			SetExplosion(g_Player.pos, FIRST_POS, COLOR_WHITE, 20.0f, EXPLOSIONTYPE_0);
			SetVibration(10000, 12000, 10);

			if (g_Player.bBreak == false)
			{// ロケットの破壊エフェクト
				SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
				SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
				SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(0.9f, 0.5f, 0.5f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
				SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(0.5f, 0.8f, 0.9f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
			}
			else
			{// 岩の破壊エフェクト
				SetParticle3D(2, 5, g_Player.pos, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), FIRST_POS, 6.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCK, PATICLETYPE_NOMAL, 0);
			}
		}

		if (CollisionPlanet(&g_Player.pos, 1.0f) == true)
		{// 惑星との当たり判定による演出
			SetExplosion(g_Player.pos, FIRST_POS, COLOR_WHITE, 20.0f, EXPLOSIONTYPE_0);
			SetVibration(10000, 12000, 10);

			SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
			SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
			SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(0.9f, 0.5f, 0.5f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
			SetParticle3D(1, 5, g_Player.pos, D3DXCOLOR(0.5f, 0.8f, 0.9f, 1.0f), FIRST_POS, 4.0f, 10, 2.0f, 0.0f, EFFECTTYPE_ROCKET, PATICLETYPE_NOMAL, 0);
		}

		if (g_Player.fAngleZ > D3DX_PI * 0.25f)
		{// 角度の最大
			g_Player.fAngleZ = D3DX_PI * 0.25f;
		}
		else if (g_Player.fAngleZ < D3DX_PI * -0.25f)
		{// 角度の最小
			g_Player.fAngleZ = D3DX_PI * -0.25f;
		}

		// 向きを調整
		CorrectAngle(&g_Player.fAngleZ, g_Player.fAngleZ - g_Player.rot.z);

		if (g_Player.rot.z != g_Player.fAngleZ)
		{// 目標地点につくまで慣性で角度を足す
			g_Player.rot.z += (g_Player.fAngleZ - g_Player.rot.z) * INERTIA_ANGLE;

			// 向きを調整
			CorrectAngle(&g_Player.rot.z, g_Player.rot.z);
		}

#if 0
		g_Player.fOil -= MINUS_OIL;
#endif

		if (g_Player.fOil <= BREAK_OIL)
		{// 小惑星を壊せる
			g_Player.bBreak = true;
		}
		else
		{// 壊せない
			g_Player.bBreak = false;
		}

		if (g_Player.fOil > MAX_OIL)
		{// 燃料の最大
			g_Player.fOil = MAX_OIL;
		}
		else if (g_Player.fOil < 0.0f)
		{// 燃料の最小(ゲームオーバー)
			g_Player.fOil = 0.0f;
			g_Player.bUse = false;
		}
	}
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void DrawPlayer(void)
{
	if (g_Player.bUse == false)
	{// 使ってない場合スキップ
		return;
	}

	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;					// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_PlayerModel.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_PlayerModel.dwNumMat; nCntMat++)
	{
		// 色保存用
		D3DXCOLOR MatCol = pMat[nCntMat].MatD3D.Diffuse;

		if (g_Player.state == PLAYERSTATE_RESULT && g_Player.bClear == false)
		{// 色を変更(焦げたような色)
			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(MatCol.r * 0.5f, MatCol.g * 0.5f, MatCol.b * 0.5f, 1.0f);
		}

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// 色を戻す
		pMat[nCntMat].MatD3D.Diffuse = MatCol;

		// テクスチャの設定
		pDevice->SetTexture(0, g_PlayerModel.apTexture[nCntMat]);

		// モデルパーツの描画
		g_PlayerModel.pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// プレイヤーの設定
//=============================================================================
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYERSTATE state)
{
	g_Player.pos = pos;
	g_Player.move = FIRST_POS;
	g_Player.rot = rot;
	g_Player.fAngleZ = 0.0f;
	g_Player.Speed = FIRST_POS;
	g_Player.state = state;
	g_Player.nCounterState = 30;
	g_Player.bBreak = false;
	g_Player.bUse = true;

	if (state == PLAYERSTATE_RESULT)
	{// リザルト用の設定
		if (GetGAmeState() == GAMESTATE_CLEAR)
		{// ゲームクリア
			g_Player.rot = D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f);
			g_Player.bClear = true;

			SetPlanet(PLANETTYPE_GOAL, D3DXVECTOR3(0.0f, -40.0f, 0.0f), FIRST_POS);
		}
		else if (GetGAmeState() == GAMESTATE_OVER)
		{// ゲームオーバー
			g_Player.bClear = false;

			if (g_Player.planetType != -1)
			{// 惑星にぶつかって死んだ
				SetPlanet(g_Player.planetType, D3DXVECTOR3(0.0f, -40.0f, 0.0f), FIRST_POS);
			}
		}
	}
	else if (state == PLAYERSTATE_APPEAR)
	{// 触れていた惑星の種類を初期化
		g_Player.planetType = (PLANETTYPE)-1;
	}
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
Player* GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 燃料追加
//=============================================================================
void AddOil(float fAddOil)
{
	g_Player.fOil += fAddOil;

	if (g_Player.fOil > MAX_OIL)
	{// 燃料の最大
		g_Player.fOil = MAX_OIL;
	}
}

//=============================================================================
// 角度を調整
//=============================================================================
void CorrectAngle(float* fAngle, float fAngleCmp)
{
	if (fAngleCmp > D3DX_PI)
	{
		*fAngle -= D3DX_PI * 2;
	}
	else if (fAngleCmp < -D3DX_PI)
	{
		*fAngle += D3DX_PI * 2;
	}
}