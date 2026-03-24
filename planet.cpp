//==================================================================================
// 
// 惑星に関する処理[planet.cpp]
// Author : Soki Inamura
// 
//==================================================================================
#include "main.h"

#include "planet.h"
#include "stage.h"

// マクロ定義 ==================================================

#define MAX_NUM_PLANET			(256)
#define MAX_TYPE_PLANETINFO		(32)

// グローバル宣言 ==============================================

PlanetModelInfo g_aPlanetModelInfo[MAX_TYPE_PLANETINFO];	// 惑星モデルの情報
Planet g_aPlanet[MAX_NUM_PLANET];							// 惑星の情報

//========================================================================
// 惑星の初期化処理
//========================================================================
void InitPlanet(void)
{
	// 惑星のモデル情報の初期化
	if (GetLoadStage() == false)
	{// 読み込まれていない場合

		for (int nCntPlanetInfo = 0; nCntPlanetInfo < MAX_TYPE_PLANETINFO; nCntPlanetInfo++)
		{
			// モデル情報
			g_aPlanetModelInfo[nCntPlanetInfo].nIdxStageModel = -1;	// ステージモデルのインデックスを初期化
			g_aPlanetModelInfo[nCntPlanetInfo].fRadius = 0.0f;		// 半径を初期化
			g_aPlanetModelInfo[nCntPlanetInfo].bUse = false;		// 使用していない状態に設定
		}

		// 惑星の情報の初期化
		for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
		{
			g_aPlanet[nCntPlanet].type = PLANETTYPE_NONE;
			g_aPlanet[nCntPlanet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlanet[nCntPlanet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlanet[nCntPlanet].bUse = false;
		}
	}
}

//========================================================================
// 惑星の終了処理
//========================================================================
void UninitPlanet(void)
{
	
}

//========================================================================
// 惑星の更新処理
//========================================================================
void UpdatePlanet(void)
{

}

//========================================================================
// 惑星の描画処理
//========================================================================
void DrawPlanet(void)
{
	// 変数宣言 ================================================

	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	StageModelInfo* pSageModelInfo;

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルのポインタ

	// =========================================================

	for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
	{
		if (g_aPlanet[nCntPlanet].bUse == true)
		{// 使用している場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlanet[nCntPlanet].mtxWorld);	// ワールドマトリックスの初期値を設定

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aPlanet[nCntPlanet].rot.y, g_aPlanet[nCntPlanet].rot.x, g_aPlanet[nCntPlanet].rot.z);

			D3DXMatrixMultiply(&g_aPlanet[nCntPlanet].mtxWorld, &g_aPlanet[nCntPlanet].mtxWorld, &mtxRot);

			// 位置を設定
			D3DXMatrixTranslation(&mtxTrans,
				g_aPlanet[nCntPlanet].pos.x, g_aPlanet[nCntPlanet].pos.y, g_aPlanet[nCntPlanet].pos.z);

			D3DXMatrixMultiply(&g_aPlanet[nCntPlanet].mtxWorld, &g_aPlanet[nCntPlanet].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlanet[nCntPlanet].mtxWorld);

			// 現在のマテリアルを所得
			pDevice->GetMaterial(&matDef);

			// モデル情報を獲得
			if (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].nIdxStageModel != 1)
			{// モデルがある

				pSageModelInfo = GetStageModelInfo(g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].nIdxStageModel);

				// マテリアルデータへのポインタを所得
				pMat = (D3DXMATERIAL*)pSageModelInfo->pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)pSageModelInfo->dwNumMat; nCntMat++)
				{// マテリアルの数分繰り返す

					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// テクスチャの設定
					pDevice->SetTexture(0, pSageModelInfo->apTexture[nCntMat]);

					// モデルパーツの描画
					pSageModelInfo->pMesh->DrawSubset(nCntMat);
				}
			}
			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}

	}
}

//========================================================================
// 惑星のモデルの読み込み処理
//========================================================================
void SetLoadPlanetInfo(int nIdxStage, float fRadius)
{
	// 惑星モデル情報の設定
	for (int nCntModel = 0; nCntModel < MAX_TYPE_PLANETINFO; nCntModel++)
	{
		if (g_aPlanetModelInfo[nCntModel].bUse == false)
		{
			g_aPlanetModelInfo[nCntModel].nIdxStageModel = nIdxStage;
			g_aPlanetModelInfo[nCntModel].fRadius = fRadius;
			g_aPlanetModelInfo[nCntModel].bUse = true;

			break;
		}
	}
}

//========================================================================
// 惑星の設定処理
//========================================================================
void SetPlanet(PLANETTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
	{
		if (g_aPlanet[nCntPlanet].bUse == false)
		{// 使用していない場合

			g_aPlanet[nCntPlanet].type = type;
			g_aPlanet[nCntPlanet].pos = pos;
			g_aPlanet[nCntPlanet].rot = rot;
			g_aPlanet[nCntPlanet].bUse = true;

			// 種類別の設定
			switch (g_aPlanet[nCntPlanet].type)
			{
			case PLANETTYPE_NONE:
				break;
			}

			break;
		}
	}
}

//========================================================================
// 惑星の当たり判定処理
//========================================================================
bool CollisionPlanet(D3DXVECTOR3 pos1, float fRadius1,
	D3DXVECTOR3 pos2, float fRadius2)
{
	return false;
}


