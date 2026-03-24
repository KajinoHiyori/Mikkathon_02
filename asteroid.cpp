//=============================================================================
// 
// 小惑星管理処理 [asteroid.cpp]
// Author : Kajino Hiyori
// 
//=============================================================================
#include "asteroid.h"
#include "main.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define MAX_ASTEROIDTEXTURE	(16)	// 小惑星のテクスチャ数
#define ASTEROID_MODELPATH	("data\\MODEL\\asteroid000.x")	// 小惑星のモデルパス

// 小惑星のモデル情報
typedef struct
{
	LPD3DXMESH pMesh;									// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;								// マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 apTexture[MAX_ASTEROIDTEXTURE];	// テクスチャへのポインタ
	DWORD dwNumMat;										// マテリアルの数
}Asteroid_Model;

// グローバル変数
Asteroid_Model g_asteroidModel;	// モデルの情報を格納
Asteroid	g_aAsteroid[MAX_ASTEROID];	// 小惑星の情報を管理

//=======================================================
// 小惑星の初期化処理
//=======================================================
void InitAsteroid(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ
	D3DXMATERIAL* pMat;

	// モデルの読み込み
	if (g_asteroidModel.pMesh == NULL)
	{// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(ASTEROID_MODELPATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_asteroidModel.pBuffMat, NULL, &g_asteroidModel.dwNumMat, &g_asteroidModel.pMesh)))
		{// NULLチェック
			return;
		}

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_asteroidModel.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_asteroidModel.dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_asteroidModel.apTexture[nCntMat]);
			}
		}
	}

	// 小惑星構造体の初期化

}

//=======================================================
// 小惑星の終了処理
//=======================================================
void UninitAsteroid(void)
{
	// メッシュの破棄
	if (g_asteroidModel.pMesh != NULL)
	{
		g_asteroidModel.pMesh->Release();
		g_asteroidModel.pMesh = NULL;
	}

	// マテリアルの破棄
	if (g_asteroidModel.pBuffMat != NULL)
	{
		g_asteroidModel.pBuffMat->Release();
		g_asteroidModel.pBuffMat = NULL;
	}

	// テクスチャの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_ASTEROIDTEXTURE; nCntPlayer++)
	{
		if (g_asteroidModel.apTexture[nCntPlayer] != NULL)
		{
			g_asteroidModel.apTexture[nCntPlayer]->Release();
			g_asteroidModel.apTexture[nCntPlayer] = NULL;
		}
	}
}

//=======================================================
// 小惑星の更新処理
//=======================================================
void UpdateAsteroid(void)
{
	
}

//=======================================================
// 小惑星の描画処理
//=======================================================
void DrawAsteroid(void)
{

}

//=======================================================
// 小惑星の設定処理[通常]
//=======================================================
void SetAsteroid(ASTEROIDTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 posParent, D3DXVECTOR3 move)
{

}

//=======================================================
// 小惑星の設定処理[衛星]
//=======================================================
void SetSarellite(D3DXVECTOR3 posParent, D3DXVECTOR3 move, float fRadius)
{

}