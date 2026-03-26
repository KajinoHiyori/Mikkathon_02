//==================================================================================
// 
// 惑星に関する処理[planet.cpp]
// Author : Soki Inamura
// 
//==================================================================================
#include "main.h"

#include "planet.h"
#include "stage.h"

#include "game.h"

#include "particle_3d.h"
#include "player.h"
#include "asteroid.h"

#include "debugproc.h"

// マクロ定義 ==================================================

#define MAX_NUM_PLANET			(256)
#define MAX_TYPE_PLANETINFO		(32)

#define TIMEWAIT_CHANGE			(60 * 0.1f)	// 重力が切り替わる惑星の重力が切り替わるまでの時間

#define MAX_CHANGE_GRAVITY		(5.0f)
#define MIN_CHANGE_GRAVITY		(2.5f)

// グローバル宣言 ==============================================

PlanetModelInfo g_aPlanetModelInfo[MAX_TYPE_PLANETINFO];	// 惑星モデルの情報
Planet g_aPlanetLayoutInfo[MAX_NUM_PLANET];					// 惑星の配置情報
Planet g_aPlanet[MAX_NUM_PLANET];							// 惑星の情報

int g_nNumWhiteHoalPlanet;									// ホワイトホールの総数
int g_aIdxWhiteHoalPlanet[MAX_NUM_PLANET];					// ホワイトホールのインデックス

int g_nNumPlanet;

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
			g_aPlanetModelInfo[nCntPlanetInfo].nIdxStageModel = -1;						// ステージモデルのインデックスを初期化
			g_aPlanetModelInfo[nCntPlanetInfo].fHitRadius = 0.0f;						// 当たり半径を初期化
			g_aPlanetModelInfo[nCntPlanetInfo].fGravity = 0.0f;							// 重力を初期化
			g_aPlanetModelInfo[nCntPlanetInfo].fRadius = 0.0f;							// 半径を初期化
			g_aPlanetModelInfo[nCntPlanetInfo].addRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 加算角度を初期化
			g_aPlanetModelInfo[nCntPlanetInfo].bUse = false;							// 使用していない状態に設定
		}

		// 惑星の情報の初期化
		for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
		{
			g_aPlanetLayoutInfo[nCntPlanet].type = PLANETTYPE_MAX;
			g_aPlanetLayoutInfo[nCntPlanet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlanetLayoutInfo[nCntPlanet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlanetLayoutInfo[nCntPlanet].nIdx = -1;
			g_aPlanetLayoutInfo[nCntPlanet].bUse = false;
		}

		g_nNumPlanet = 0;	// 惑星の総数を初期化

		g_nNumWhiteHoalPlanet = 0;												// ホワイトホールの総数
		memset(&g_aIdxWhiteHoalPlanet[0], -1, sizeof g_aIdxWhiteHoalPlanet);	// ホワイトホールのインデックス
	}
	else
	{
		for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
		{
			if (g_aPlanet[nCntPlanet].type == PLANETTYPE_ASTEROID)
			{
				for (int nCntSatrellite = 0; nCntSatrellite < PLANET_SET_ASTEROID; nCntSatrellite++)
				{
					SetSarellite(g_aPlanet[nCntPlanet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5.0f, 1.0f);
				}
			}
		}
	}

	// 惑星の初期化
	for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
	{
		g_aPlanet[nCntPlanet].type = PLANETTYPE_MAX;
		g_aPlanet[nCntPlanet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlanet[nCntPlanet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlanet[nCntPlanet].nIdx = -1;
		g_aPlanet[nCntPlanet].bUse = false;
	}

	if (GetMode() == MODE_GAME)
	{
		// 惑星の配置
		SetLayoutPlanet(D3DXVECTOR3(0.0f,0.0f,0.0f));
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

				g_aPlanet[nCntPlanet].nIdx++;	// カウンタとして値をインクリメント

				// 重力の切り替え
				if (g_aPlanet[nCntPlanet].nIdx == TIMEWAIT_CHANGE)
				{// 待機時間になった

					// 重力の設定
					g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity = (float)(rand() % (int)(MAX_CHANGE_GRAVITY * 10) / 10.0f) + MIN_CHANGE_GRAVITY;

					g_aPlanet[nCntPlanet].nIdx = 0;	// 値を初期化
				}

				SetPlanetParticle3D(g_aPlanet[nCntPlanet].type, g_aPlanet[nCntPlanet].pos, g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity);

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
			g_aPlanet[nCntPlanet].rot += g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].addRot;
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
			if (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].nIdxStageModel != -1)
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
void SetLoadPlanetInfo(int nIdxStage, float fHitRadius, float fGravity, float fRadius, D3DXVECTOR3 addRot)
{
	// 惑星モデル情報の設定
	for (int nCntModel = 0; nCntModel < MAX_TYPE_PLANETINFO; nCntModel++)
	{
		if (g_aPlanetModelInfo[nCntModel].bUse == false)
		{
			g_aPlanetModelInfo[nCntModel].nIdxStageModel = nIdxStage;	// 
			g_aPlanetModelInfo[nCntModel].fHitRadius = fHitRadius;		// 
			g_aPlanetModelInfo[nCntModel].fGravity = fGravity;			// 
			g_aPlanetModelInfo[nCntModel].fRadius = fRadius;				// 
			g_aPlanetModelInfo[nCntModel].addRot = addRot;				// 
			g_aPlanetModelInfo[nCntModel].bUse = true;					// 

			break;
		}
	}
}

//========================================================================
// 惑星の設定情報の設定処理
//========================================================================
void SetPlanetInfo(PLANETTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
	{
		if (g_aPlanetLayoutInfo[nCntPlanet].bUse == false)
		{// 使用していない場合

			g_aPlanetLayoutInfo[nCntPlanet].type = type;
			g_aPlanetLayoutInfo[nCntPlanet].pos = pos;
			g_aPlanetLayoutInfo[nCntPlanet].rot = rot;
			g_aPlanetLayoutInfo[nCntPlanet].nIdx = -1;
			g_aPlanetLayoutInfo[nCntPlanet].bUse = true;

			g_nNumPlanet++;	// 惑星の総数をインクリメント

			break;
		}
	}
}

//========================================================================
// 惑星の配置情報の設定
///========================================================================
void SetLayoutPlanet(D3DXVECTOR3 startPos)
{
	

	// 配置情報を設定
	for (int nCntPlanet = 0; nCntPlanet < MAX_NUM_PLANET; nCntPlanet++)
	{
#if 1
		if (g_aPlanetLayoutInfo[nCntPlanet].bUse == true)
		{
			// 惑星の設定
			SetPlanet(g_aPlanetLayoutInfo[nCntPlanet].type, startPos + g_aPlanetLayoutInfo[nCntPlanet].pos, g_aPlanetLayoutInfo[nCntPlanet].rot);
		}
#else
		if (nCntPlanet < 100)
		{
			D3DXVECTOR3 setPos;

			setPos.x = (float)(rand() % (int)((10000.0f * 2) * 10) - (int)(5000.0f * 10)) / 10;
			setPos.y = (float)(rand() % (int)((10000.0f * 2) * 10) - (int)(5000.0f * 10)) / 10;
			
			setPos.z = (float)(rand() % (int)(50000.0f * 2) * 10) / 10;

			int nSetType = rand() % PLANETTYPE_GOAL;

			// 惑星の設定
			SetPlanet((PLANETTYPE)nSetType, setPos + startPos, D3DXVECTOR3(0.0f,0.0f,0.0f));

		}
		else
		{
			break;
		}

#endif

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
			g_aPlanet[nCntPlanet].nIdx = -1;
			g_aPlanet[nCntPlanet].bUse = true;

			// 種類別の設定
			switch (g_aPlanet[nCntPlanet].type)
			{
			case PLANETTYPE_ENERGY:		// エネルギー惑星
				break;

			case PLANETTYPE_ASTEROID:	// 小惑星付き惑星

				for (int nCntSatrellite = 0; nCntSatrellite < PLANET_SET_ASTEROID; nCntSatrellite++)
				{
					SetSarellite(g_aPlanet[nCntPlanet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3.0f, 0.1f);
				}

				break;

			case PLANETTYPE_BLACKHOLE:	// ブラックホール惑星

				SetParticle3D(10, -1, g_aPlanet[nCntPlanet].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7.0f, 100, 5.0f, -0.1f, EFFECTTYPE_NORMAL, PATICLETYPE_HOLE, -1, false, D3DXVECTOR3(0.0f,0.0f,0.0f), 0.0f);

				if (g_nNumWhiteHoalPlanet > 0)
				{
					g_aPlanet[nCntPlanet].nIdx = g_aIdxWhiteHoalPlanet[rand() % g_nNumWhiteHoalPlanet];
				}

				break;

			case PLANETTYPE_WHITEHOLE:	// ホワイトホール惑星

				SetParticle3D(10, -1, g_aPlanet[nCntPlanet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7.0f, 100, 0.0f, 0.5f, EFFECTTYPE_NORMAL, PATICLETYPE_HOLE, -1, false, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);

				g_aIdxWhiteHoalPlanet[g_nNumWhiteHoalPlanet] = nCntPlanet;

				g_nNumWhiteHoalPlanet++;

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

			SetPlanetParticle3D(g_aPlanet[nCntPlanet].type, g_aPlanet[nCntPlanet].pos, g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity);

			break;	// for文を抜ける
		}
	}
}

//========================================================================
// 惑星の当たり判定処理
//========================================================================
bool CollisionPlanet(D3DXVECTOR3 *pPos, float fRadius)
{
	// 変数宣言 ================================================

	float fWidth, fHeight, fDipth;
	float fLengthXZ, fDestLength;
	float fAngleXZ, fDestAngle;
	float fNomDistance;

	Player* pPlayer = GetPlayer();

	// =========================================================

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

			PrintDebugProc("\nDEST_LENGTH %f",fDestLength);

			if (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fRadius >= fDestLength)
			{// 惑星の効果範囲に入った

				PrintDebugProc("\nIN_PLANETRADIUS!!!!!\n");

				// 距離の割合
				fNomDistance = fDestLength / g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fRadius;

				// 角度を求める
				fAngleXZ = atan2f(fWidth, fDipth);
				fDestAngle = atan2f(fLengthXZ, fHeight);

				if(g_aPlanet[nCntPlanet].type == PLANETTYPE_BLACKHOLE)
				{ 
					pPos->x += sinf(fAngleXZ) * (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity - g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity * fNomDistance);
					pPos->z += cosf(fAngleXZ) * (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity - g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity * fNomDistance);
					pPos->y += cosf(fDestAngle) * (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity - g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity * fNomDistance);
				}
				else
				{
					pPos->x += sinf(fAngleXZ) * g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity;
					pPos->z += cosf(fAngleXZ) * g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity;
					pPos->y += cosf(fDestAngle) * g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fGravity;
				}

				// 変更後の離れ具合を求める
				fWidth = g_aPlanet[nCntPlanet].pos.x - pPos->x;
				fHeight = g_aPlanet[nCntPlanet].pos.y - pPos->y;
				fDipth = g_aPlanet[nCntPlanet].pos.z - pPos->z;

				// 変更後のXZの距離を求める
				fLengthXZ = sqrtf(fWidth * fWidth + fDipth * fDipth);

				// 変更後の対象との距離を求める
				fDestLength = sqrtf(fLengthXZ * fLengthXZ + fHeight * fHeight);

				// 惑星と衝突したかの判定
				if (g_aPlanetModelInfo[g_aPlanet[nCntPlanet].type].fHitRadius + fRadius > fDestLength)
				{// 衝突した

					if (g_aPlanet[nCntPlanet].type == PLANETTYPE_BLACKHOLE)
					{// ぶつかった惑星がブラックホールの場合
						
						// ホワイトホールの位置を設定
						pPos->x = g_aPlanet[g_aPlanet[nCntPlanet].nIdx].pos.x;
						pPos->y = g_aPlanet[g_aPlanet[nCntPlanet].nIdx].pos.y;
						pPos->z = g_aPlanet[g_aPlanet[nCntPlanet].nIdx].pos.z;
					}

					if (g_aPlanet[nCntPlanet].type != PLANETTYPE_BLACKHOLE 
					 && g_aPlanet[nCntPlanet].type != PLANETTYPE_WHITEHOLE)
					{// ブラックホール, ホワイトホールではない

						pPlayer->planetType = g_aPlanet[nCntPlanet].type;	// ぶつかった惑星の種類を返す
						pPlayer->bUse = false;								// プレイヤーの使用していない状態に設定
					}

					// 惑星にぶつかった時の判定
					if (pPlayer->bUse == false)
					{// ぶつかって消えた場合
						
						// ぶつかった対象
						if (pPlayer->planetType == PLANETTYPE_GOAL)
						{// ゴールの惑星に着陸した

							SetGameState(GAMESTATE_CLEAR, 0);
						}
						else
						{// それ以外の惑星

							SetGameState(GAMESTATE_OVER, 0);
						}
					}

					return true;	// 当たった事を返す
				}
				else
				{// 衝突していない

					if (g_aPlanet[nCntPlanet].type != PLANETTYPE_BLACKHOLE
					 && g_aPlanet[nCntPlanet].type != PLANETTYPE_WHITEHOLE)
					{// ブラックホール, ホワイトホールではない

						// エネルギーの回復
						if (fNomDistance > 0.5f)
						{// 効果範囲の半分より外にいる

							pPlayer->fOil += (g_aPlanet[nCntPlanet].type == PLANETTYPE_ENERGY) ? 0.10f : 0.05f * (1.0f - fNomDistance);
						}
						else
						{// 効果範囲の半分より内にいる

							pPlayer->fOil += (g_aPlanet[nCntPlanet].type == PLANETTYPE_ENERGY) ? 0.25f : 0.05f * 1.25f;
						}
					}
				}
			}
		}
	}

	return false;
}

//========================================================================
// 惑星モデルの情報を返す処理
//========================================================================
PlanetModelInfo* GetPlanetInfo(void)
{
	return &g_aPlanetModelInfo[0];
}

//========================================================================
// 惑星モデルの当たり半径を返す処理
//========================================================================
float GetPlanetInfoHitRadius(PLANETTYPE type)
{
	return g_aPlanetModelInfo[type].fHitRadius;
}

//========================================================================
// 惑星の情報を返す処理
//========================================================================
Planet* GetPlanet(void)
{
	return &g_aPlanet[0];
}

//========================================================================
// 惑星の総数を返す処理
//========================================================================
int GetNumPlanet(void)
{
	return g_nNumPlanet;
}