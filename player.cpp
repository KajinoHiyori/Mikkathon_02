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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVEMENT				(D3DXVECTOR3(1.0f, 1.0f, 1.0f))			// 移動量
#define ROT						(D3DXVECTOR3(0.05f, 0.05f, 0.05f))		// 向き移動量
#define INERTIA_MOVE			(0.4f)									// 移動の慣性
#define INERTIA_ANGLE			(0.1f)									// 角度の慣性
#define MAX_OIL					(100.0f)								// 最大燃料
#define MINUS_OIL				(0.001f)								// 燃料の減り方
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
	g_Player.fSpeedZ = MOVEMENT.z;
	g_Player.state = PLAYERSTATE_APPEAR;
	g_Player.nCounterState = 0;
	g_Player.fOil = MAX_OIL * 0.5f;
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
	}

	if (g_Player.state != PLAYERSTATE_APPEAR)
	{// 出現状態のときは移動できない
		int nValueH, nValueV;

		// パッド移動
		if (GetJoypadLeftStickValue(&nValueH, &nValueV) == true)
		{// パッドの移動優先
			g_Player.fAngleZ = atan2f((float)(nValueH), (float)(nValueV));

			g_Player.move.x += sinf(pCamera->rot.y) * MOVEMENT.x/* * sinf((D3DX_PI * 0.5f) + pCamera->fAngle)*/;
			g_Player.move.y += cosf(pCamera->rot.y) * MOVEMENT.y;
			//g_Player.move.z += cosf(fAngle + pCamera->rot.y) * MOVEMENT.z /** sinf((D3DX_PI * 0.5f) + pCamera->fAngle)*/;
		}
		else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)		// キーボード移動
		{// 上に移動
			if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
			{// 左上に移動
				g_Player.move.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * MOVEMENT.x;
				g_Player.move.y += cosf((D3DX_PI * 0.75f)) * -MOVEMENT.y;
				//g_Player.move.z += cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * MOVEMENT.z;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
			{// 右上に移動
				g_Player.move.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * MOVEMENT.x;
				g_Player.move.y += cosf((D3DX_PI * 0.75f)) * -MOVEMENT.y;
				//g_Player.move.z += cosf(D3DX_PI * 0.25f + pCamera->rot.y) * MOVEMENT.z;
			}
			else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			{// 上に移動
				g_Player.move.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * MOVEMENT.x;
				g_Player.move.y += cosf((D3DX_PI * 1.0f)) * -MOVEMENT.y;
				//g_Player.move.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * MOVEMENT.z;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{// 下に移動
			if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
			{// 左下に移動
				g_Player.move.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * MOVEMENT.x;
				g_Player.move.y += cosf((D3DX_PI * 0.25f)) * -MOVEMENT.y;
				//g_Player.move.z += cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * MOVEMENT.z;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
			{// 右下に移動
				g_Player.move.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * MOVEMENT.x;
				g_Player.move.y += cosf((D3DX_PI * 0.25f)) * -MOVEMENT.y;
				//g_Player.move.z += cosf(D3DX_PI * 0.75f + pCamera->rot.y) * MOVEMENT.z;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{// 下に移動
				g_Player.move.x += sinf(D3DX_PI * 1.0f + pCamera->rot.y) * MOVEMENT.x;
				g_Player.move.y += cosf((D3DX_PI * 0.0f)) * -MOVEMENT.y;
				//g_Player.move.z += cosf(D3DX_PI * 1.0f + pCamera->rot.y) * MOVEMENT.z;
			}
		}
		else if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
		{// 左に移動
			g_Player.move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * MOVEMENT.x;
			g_Player.move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * MOVEMENT.z;

			g_Player.fAngleZ = pCamera->rot.y + (D3DX_PI * 0.5f);
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
		{// 右に移動
			g_Player.move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * MOVEMENT.x;
			g_Player.move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * MOVEMENT.z;

			g_Player.fAngleZ = pCamera->rot.y - (D3DX_PI * 0.5f);
		}
	}

	g_Player.fSpeedZ = (MAX_OIL - g_Player.fOil) / (MAX_OIL * 0.5f * MOVEMENT.z);		// 燃料50%で既定の速度くらい
	g_Player.move.z += g_Player.fSpeedZ;

	// 慣性
	g_Player.pos += g_Player.move;
	g_Player.move.x += (0.0f - g_Player.move.x) * INERTIA_MOVE;
	g_Player.move.y += (0.0f - g_Player.move.y) * INERTIA_MOVE;
	g_Player.move.z += (0.0f - g_Player.move.z) * INERTIA_MOVE;

	PrintDebugProc("プレイヤーのpos : ( %f %f %f )\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

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

#ifndef _DEBUG
	g_Player.fOil -= MINUS_OIL;
#endif

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
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

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
void SetPlayer(D3DXVECTOR3 pos)
{
	g_Player.pos = pos;
	g_Player.move = FIRST_POS;
	g_Player.rot = FIRST_POS;
	g_Player.fAngleZ = 0.0f;
	g_Player.fSpeedZ = MOVEMENT.z;
	g_Player.state = PLAYERSTATE_APPEAR;
	g_Player.nCounterState = 180;
	g_Player.fOil = MAX_OIL * 0.5f;
	g_Player.bUse = true;
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
Player* GetPlayer(void)
{
	return &g_Player;
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