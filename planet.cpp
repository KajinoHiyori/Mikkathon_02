//==================================================================================
// 
// 惑星に関する処理[planet.cpp]
// Author : Soki Inamura
// 
//==================================================================================
#include "main.h"

#include "planet.h"
#include "stage.h"

#include "debugproc.h"

// マクロ定義 ==================================================

#define MAX_NUM_PLANET			(256)
#define MAX_TYPE_PLANETINFO		(32)

// グローバル宣言 ==============================================

PlanetModelInfo g_aPlanetInfo[MAX_TYPE_PLANETINFO];	// 惑星モデルの情報
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
			g_aPlanetInfo[nCntPlanetInfo].nIdxStageModel = -1;	// ステージモデルのインデックスを初期化
			g_aPlanetInfo[nCntPlanetInfo].fHitRadius = 0.0f;	// 当たり半径を初期化
			g_aPlanetInfo[nCntPlanetInfo].fGravity = 0.0f;		// 重力を初期化
			g_aPlanetInfo[nCntPlanetInfo].fRadius = 0.0f;		// 半径を初期化
			g_aPlanetInfo[nCntPlanetInfo].addRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 加算角度を初期化
			g_aPlanetInfo[nCntPlanetInfo].bUse = false;			// 使用していない状態に設定
		}

		// 惑星の情報の初期化
		for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
		{
			g_aPlanet[nCntPlanet].type = PLANETTYPE_MAX;
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
	for (int nCntPlanet = 0; nCntPlanet < MAX_TYPE_PLANETINFO; nCntPlanet++)
	{
		if (g_aPlanet[nCntPlanet].bUse == true)
		{// 使用している場合

			switch (g_aPlanet[nCntPlanet].type)
			{
			case PLANETTYPE_ENERGY:		// エネルギー惑星
				break;

			case PLANETTYPE_ASTEROID:	// 小惑星付き惑星
				break;

			case PLANETTYPE_BLACKHOLE:	// ブラックホール惑星
				break;

			case PLANETTYPE_WHITEHOLE:	// ホワイトホール惑星
				break;

			case PLANETTYPE_CHANGE:		// 重力変わる惑星
				break;

			case PLANETTYPE_REPULSIVE:	// 斥力惑星	
				break;

			case PLANETTYPE_SMALL:		// 一般(引力：小)惑星
				break;

			case PLANETTYPE_MID:		// 一般(引力：中)惑星
				break;

			case PLANETTYPE_BIG:		// 一般(引力：大)惑星
				break;

			case PLANETTYPE_GOAL:		// ゴール
				break;
			}

			// 角度を加算
			g_aPlanet[nCntPlanet].rot += g_aPlanetInfo[g_aPlanet[nCntPlanet].type].addRot;
		}
	}
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
			if (g_aPlanetInfo[g_aPlanet[nCntPlanet].type].nIdxStageModel != -1)
			{// モデルがある

				pSageModelInfo = GetStageModelInfo(g_aPlanetInfo[g_aPlanet[nCntPlanet].type].nIdxStageModel);

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
void SetLoadPlanetInfo(int nIdxStage, float fHitRadius, float fGravity, float fRadius, D3DXVECTOR3 addRot)
{
	// 惑星モデル情報の設定
	for (int nCntModel = 0; nCntModel < MAX_TYPE_PLANETINFO; nCntModel++)
	{
		if (g_aPlanetInfo[nCntModel].bUse == false)
		{
			g_aPlanetInfo[nCntModel].nIdxStageModel = nIdxStage;	// 
			g_aPlanetInfo[nCntModel].fHitRadius = fHitRadius;		// 
			g_aPlanetInfo[nCntModel].fGravity = fGravity;			// 
			g_aPlanetInfo[nCntModel].fRadius = fRadius;				// 
			g_aPlanetInfo[nCntModel].addRot = addRot;				// 
			g_aPlanetInfo[nCntModel].bUse = true;					// 

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
			case PLANETTYPE_ENERGY:		// エネルギー惑星
				break;

			case PLANETTYPE_ASTEROID:	// 小惑星付き惑星
				break;

			case PLANETTYPE_BLACKHOLE:	// ブラックホール惑星
				break;

			case PLANETTYPE_WHITEHOLE:	// ホワイトホール惑星
				break;

			case PLANETTYPE_CHANGE:		// 重力変わる惑星
				break;

			case PLANETTYPE_REPULSIVE:	// 斥力惑星	
				break;

			case PLANETTYPE_SMALL:		// 一般(引力：小)惑星
				break;

			case PLANETTYPE_MID:		// 一般(引力：中)惑星
				break;

			case PLANETTYPE_BIG:		// 一般(引力：大)惑星
				break;

			case PLANETTYPE_GOAL:		// ゴール
				break;
			}

			break;	// for文を抜ける
		}
	}
}

//========================================================================
// 惑星の当たり判定処理
//========================================================================
bool CollisionPlanet(D3DXVECTOR3 *pPos, float fRadius)
{
	float fWidth, fHeight, fDipth;
	float fLengthXZ, fDestLength;
	float fAngleXZ, fDestAngle;
	float fNomDistance;

	for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
	{
		if (g_aPlanet[nCntPlanet].bUse == true)
		{// 使用している場合
			
			// 離れ具合を求める
			fWidth = g_aPlanet[nCntPlanet].pos.x - pPos->x;
			fHeight = g_aPlanet[nCntPlanet].pos.y - pPos->y;
			fDipth = g_aPlanet[nCntPlanet].pos.z - pPos->z;

			// XZの距離を求める
			fLengthXZ = sqrtf(fWidth * fWidth + fDipth * fDipth);

			// 対象との距離を求める
			fDestLength = sqrtf(fLengthXZ * fLengthXZ + fHeight * fHeight);

			if (g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fRadius >= fDestLength)
			{// 惑星の効果範囲に入った

				// 距離の割合
				fNomDistance = fDestLength / g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fRadius;

				// 角度を求める
				fAngleXZ = atan2f(fWidth, fDipth);
				fDestAngle = atan2f(fLengthXZ, fHeight);
#if 0
				pPos->x += sinf(fAngleXZ) * (g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity - g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity * fNomDistance);
				pPos->z += cosf(fAngleXZ) * (g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity - g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity * fNomDistance);
				pPos->y += cosf(fDestAngle) * (g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity - g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity * fNomDistance);
#else
				pPos->x += sinf(fAngleXZ) * g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity;
				pPos->z += cosf(fAngleXZ) * g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity;
				pPos->y += cosf(fDestAngle) * g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fGravity;
#endif
				// 変更後の離れ具合を求める
				fWidth = g_aPlanet[nCntPlanet].pos.x - pPos->x;
				fHeight = g_aPlanet[nCntPlanet].pos.y - pPos->y;
				fDipth = g_aPlanet[nCntPlanet].pos.z - pPos->z;

				// 変更後のXZの距離を求める
				fLengthXZ = sqrtf(fWidth * fWidth + fDipth * fDipth);

				// 変更後の対象との距離を求める
				fDestLength = sqrtf(fLengthXZ * fLengthXZ + fHeight * fHeight);

				if (g_aPlanetInfo[g_aPlanet[nCntPlanet].type].fHitRadius + fRadius > fDestLength)
				{
					return true;	// 当たった事を返す
				}
			}
		}
	}

	return false;
}


