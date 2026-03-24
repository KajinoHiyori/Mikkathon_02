//=============================================================================
// 
// プレイヤー [player.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVEMENT				(D3DXVECTOR3(1.0f, 1.0f, 1.0f))			// 移動量
#define ROT						(D3DXVECTOR3(0.05f, 0.05f, 0.05f))		// 向き移動量
#define INERTIA_MOVE			(0.4f)									// 移動の慣性
#define PLAYER_FILE				"data\\MODEL\\rocket001.x"				// プレイヤーのファイル名

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
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void DrawPlayer(void)
{
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