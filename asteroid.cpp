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
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"

// マクロ定義=====================================================================================================================================================
#define MAX_ASTEROIDTEXTURE	(16)	// 小惑星のテクスチャ数
#define ASTEROID_MODELPATH	("data\\MODEL\\asteroid000.x")	// 小惑星のモデルパス
#define RAND_HEIGHT			((float)(rand() % (int)(g_aAsteroid[nCntAsteroid].fRadius * 0.5f) - g_aAsteroid[nCntAsteroid].fRadius * 0.25f))	// 惑星配置の高度
#define RAND_ANGLE			((float)(rand() % 629 - 314) / 100.0f)	// 角度のランダム値
#define ASTEROID_KEY		(120)	// キー数
#define FLOATING			(0.5f)	// 移動間隔
#define ROTATE				(0.05f)	// 回転速度
#define ASTEROID_HEIGHT		(30.0f)	// 小惑星の高さ
#define ASTEROID_SIZE		(30.0f)	// 小惑星の大きさ
#define MAX_PLANET			(256)	// 惑星の総数
#define PLANET_SIZE			(50.0f)	// 惑星の大きさ
#define ASTEROID_ROT		(D3DXVECTOR3(RAND_ANGLE, RAND_ANGLE, RAND_ANGLE))
#define INIT_VECTOR3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// D3DXVECTOR3の初期化
// ランダム配置[game]用マクロ定義
#define GAME_LINE		(1000)	// ゲーム開始座標[int]
#define END_LINE		(45000)	// ゴールライン[int]
#define AREA_DEPTH		(1000)	// 惑星のランダムの奥行き
#define AREA_SIZE		(2000)	// 小惑星の配置範囲
#define MAX_DEPTH		(END_LINE / GAME_LINE + 1)	// 小惑星配置範囲
#define AREA_ASTEROID	(40)	// 区間ごとに配置される小惑星の総数
#define RAND_X			((float)(rand() % AREA_SIZE - AREA_SIZE/2))	// 小惑星のランダム配置X[game]
#define RAND_Y			((float)(rand() % AREA_SIZE - AREA_SIZE/2))	// 小惑星のランダム配置Y[game]
#define RAND_Z			((float)(rand() % AREA_DEPTH) + nCntDepth * AREA_DEPTH + GAME_LINE)	// 小惑星のランダム配置Z[game]
#define ASTEROID_RAND	(D3DXVECTOR3(RAND_X, RAND_Y, RAND_Z))	// 小惑星のランダム配置座標[game]
// ランダム配置[result]用マクロ定義
#define RESULTRAND_MAX	(70)		// 小惑星の総数
#define RESULT_RADIUS	(700)	// リザルト用ランラム配置の半径
#define RESULTRAND_X	(sinf(RAND_ANGLE) * RESULT_RADIUS)	// 小惑星のランダム配置X[result]
#define RESULTRAND_Y	((float)(rand() % RESULT_RADIUS - RESULT_RADIUS/2))	// 小惑星のランダム配置Y[result]
#define RESULTRAND_Z	(cosf(RAND_ANGLE) * RESULT_RADIUS)	// 小惑星のランダム配置Z[result]
#define RESULTRAND_POS	(D3DXVECTOR3(RESULTRAND_X, RESULTRAND_Y, RESULTRAND_Z))	// 小惑星のランダム配置座標[result]

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
	for (int nCntAsteroid = 0; nCntAsteroid < MAX_ASTEROID; nCntAsteroid++)
	{
		g_aAsteroid[nCntAsteroid].type			= ASTEROIDTYPE_NONE;				// 小惑星の種類
		g_aAsteroid[nCntAsteroid].pos			= INIT_VECTOR3;	// 位置
		g_aAsteroid[nCntAsteroid].posParent		= INIT_VECTOR3;	// 親の位置
		g_aAsteroid[nCntAsteroid].move			= INIT_VECTOR3;	// 移動量
		g_aAsteroid[nCntAsteroid].rot			= INIT_VECTOR3;	// 回転方向
		g_aAsteroid[nCntAsteroid].fRadius		= 0.0f;		// 親惑星との距離
		g_aAsteroid[nCntAsteroid].fMove			= 0.0f;		// 移動方向
		g_aAsteroid[nCntAsteroid].fAngle		= 0.0f;		// 回転度数
		g_aAsteroid[nCntAsteroid].nIdx			= -1;		// インデックス
		g_aAsteroid[nCntAsteroid].bUse			= false;	// 使用状態
		// 演出面
		g_aAsteroid[nCntAsteroid].floatingX		= FLOATINGX_ADD;	// 浮遊状態[X]
		g_aAsteroid[nCntAsteroid].floatingY		= FLOATINGY_ADD;	// 浮遊状態[Y]
		g_aAsteroid[nCntAsteroid].floatingZ		= FLOATINGZ_ADD;	// 浮遊状態[Z]
		g_aAsteroid[nCntAsteroid].nKeyX			= 0;				// 浮遊時間[X]
		g_aAsteroid[nCntAsteroid].nKeyY			= 0;				// 浮遊時間[Y]
		g_aAsteroid[nCntAsteroid].nKeyZ			= 0;				// 浮遊時間[Z]
		g_aAsteroid[nCntAsteroid].nNumKey		= ASTEROID_KEY;		// 浮遊間隔を管理
	}

	MODE mode = GetMode();
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面

		break;

	case MODE_GAME:	// ゲーム画面
		SetAsteroid(ASTEROIDTYPE_FLOATING, D3DXVECTOR3(-300.0f, 0.0f, -2000.0f), INIT_VECTOR3);
		SetAsteroid(ASTEROIDTYPE_FLOATING, D3DXVECTOR3(500.0f, -20.0f, -2100.0f), INIT_VECTOR3);
		// 小惑星のランダム配置処理
		SetRandomAsteroid();
		break;

	case MODE_RESULT:	// リザルト画面
		SetResultAsteroid();
		break;
	}
	
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
	int nNum = 0;
	for (int nCntAsteroid = 0; nCntAsteroid < MAX_ASTEROID; nCntAsteroid++)
	{
		if (g_aAsteroid[nCntAsteroid].bUse == false)
		{ // 使われていない場合処理を繰り返す
			continue;
		}

		float fMove = 0.0f;	// 衛星の回転量を追加
		nNum++;	// 使用数をカウント
		// 小惑星の種類ごとの処理
		switch (g_aAsteroid[nCntAsteroid].type)
		{
		case ASTEROIDTYPE_NONE:	// 初期化用 = error
			g_aAsteroid[nCntAsteroid].bUse = false;
			break;

		case ASTEROIDTYPE_SARELLITE:	// 衛星
			g_aAsteroid[nCntAsteroid].fAngle += g_aAsteroid[nCntAsteroid].fMove;	// 回転量を増加
			g_aAsteroid[nCntAsteroid].fAngle = AngleNormalization(g_aAsteroid[nCntAsteroid].fAngle);
			g_aAsteroid[nCntAsteroid].pos.x = g_aAsteroid[nCntAsteroid].posParent.x + sinf(g_aAsteroid[nCntAsteroid].fAngle) * g_aAsteroid[nCntAsteroid].fRadius;
			g_aAsteroid[nCntAsteroid].pos.z = g_aAsteroid[nCntAsteroid].posParent.z + cosf(g_aAsteroid[nCntAsteroid].fAngle) * g_aAsteroid[nCntAsteroid].fRadius;
			break;

		default:	// フィールド内で浮遊 / プレイヤーの行動制限

			break;
		}

		// 移動量の更新
		g_aAsteroid[nCntAsteroid].pos += g_aAsteroid[nCntAsteroid].move;

		// 浮遊感の演出============================================================
		// X軸方向の浮遊感
		switch (g_aAsteroid[nCntAsteroid].floatingX)
		{
		case FLOATINGX_ADD:	// X方向加算
			g_aAsteroid[nCntAsteroid].pos.x += FLOATING;
			g_aAsteroid[nCntAsteroid].nKeyX++;
			if (g_aAsteroid[nCntAsteroid].nKeyX >= g_aAsteroid[nCntAsteroid].nNumKey)
			{
				g_aAsteroid[nCntAsteroid].nKeyX = 0;
				g_aAsteroid[nCntAsteroid].floatingX = FLOATINGX_DIS;
			}
			break;

		case FLOATINGX_DIS:	// X方向減算
			g_aAsteroid[nCntAsteroid].pos.x -= FLOATING;
			g_aAsteroid[nCntAsteroid].nKeyX++;
			if (g_aAsteroid[nCntAsteroid].nKeyX >= g_aAsteroid[nCntAsteroid].nNumKey)
			{
				g_aAsteroid[nCntAsteroid].nKeyX = 0;
				g_aAsteroid[nCntAsteroid].floatingX = FLOATINGX_ADD;
			}
			break;
		}
		// Y軸方向の浮遊感
		switch (g_aAsteroid[nCntAsteroid].floatingY)
		{
		case FLOATINGY_ADD:	// Y方向加算
			g_aAsteroid[nCntAsteroid].pos.y += FLOATING;
			g_aAsteroid[nCntAsteroid].nKeyY++;
			if (g_aAsteroid[nCntAsteroid].nKeyY >= g_aAsteroid[nCntAsteroid].nNumKey)
			{
				g_aAsteroid[nCntAsteroid].nKeyY = 0;
				g_aAsteroid[nCntAsteroid].floatingY = FLOATINGY_DIS;
			}
			break;

		case FLOATINGY_DIS:	// Y方向減算
			g_aAsteroid[nCntAsteroid].pos.y -= FLOATING;
			g_aAsteroid[nCntAsteroid].nKeyY++;
			if (g_aAsteroid[nCntAsteroid].nKeyY >= g_aAsteroid[nCntAsteroid].nNumKey)
			{
				g_aAsteroid[nCntAsteroid].nKeyY = 0;
				g_aAsteroid[nCntAsteroid].floatingY = FLOATINGY_ADD;
			}
			break;
		}
		// Z軸方向の浮遊感
		switch (g_aAsteroid[nCntAsteroid].floatingZ)
		{
		case FLOATINGZ_ADD:	// Z方向加算
			g_aAsteroid[nCntAsteroid].pos.z += FLOATING;
			g_aAsteroid[nCntAsteroid].nKeyZ++;
			if (g_aAsteroid[nCntAsteroid].nKeyZ >= g_aAsteroid[nCntAsteroid].nNumKey)
			{
				g_aAsteroid[nCntAsteroid].nKeyZ = 0; 
				g_aAsteroid[nCntAsteroid].floatingZ = FLOATINGZ_DIS;
			}
			break;

		case FLOATINGZ_DIS:	// Z方向減算
			g_aAsteroid[nCntAsteroid].pos.z -= FLOATING;
			g_aAsteroid[nCntAsteroid].nKeyZ++;
			if (g_aAsteroid[nCntAsteroid].nKeyZ >= g_aAsteroid[nCntAsteroid].nNumKey)
			{
				g_aAsteroid[nCntAsteroid].nKeyZ = 0;
				g_aAsteroid[nCntAsteroid].floatingZ = FLOATINGZ_ADD;
			}
			break;
		}

		g_aAsteroid[nCntAsteroid].rot.x += ROTATE;
		g_aAsteroid[nCntAsteroid].rot.z += ROTATE;
		g_aAsteroid[nCntAsteroid].rot.x = AngleNormalization(g_aAsteroid[nCntAsteroid].rot.x);
		g_aAsteroid[nCntAsteroid].rot.z = AngleNormalization(g_aAsteroid[nCntAsteroid].rot.z);
	}

	PrintDebugProc("使用数 %d\n", nNum);
}

//=======================================================
// 小惑星の描画処理
//=======================================================
void DrawAsteroid(void)
{
	for (int nCntAsteroid = 0; nCntAsteroid < MAX_ASTEROID; nCntAsteroid++)
	{
		if (g_aAsteroid[nCntAsteroid].bUse == false)
		{// 使ってない場合スキップ
			continue;
		}

		// ローカル変数宣言
		LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ
		D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
		D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
		D3DXMATERIAL* pMat;					// マテリアルデータへのポインタ

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aAsteroid[nCntAsteroid].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAsteroid[nCntAsteroid].rot.y, g_aAsteroid[nCntAsteroid].rot.x, g_aAsteroid[nCntAsteroid].rot.z);
		D3DXMatrixMultiply(&g_aAsteroid[nCntAsteroid].mtxWorld, &g_aAsteroid[nCntAsteroid].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aAsteroid[nCntAsteroid].pos.x, g_aAsteroid[nCntAsteroid].pos.y, g_aAsteroid[nCntAsteroid].pos.z);
		D3DXMatrixMultiply(&g_aAsteroid[nCntAsteroid].mtxWorld, &g_aAsteroid[nCntAsteroid].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aAsteroid[nCntAsteroid].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_asteroidModel.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_asteroidModel.dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_asteroidModel.apTexture[nCntMat]);

			// モデルパーツの描画
			g_asteroidModel.pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================================
// 小惑星の設定処理[通常]
//=======================================================
void SetAsteroid(ASTEROIDTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	Planet* pPlanet = GetPlanet();
	for (int nCntPlanet = 0; nCntPlanet < MAX_PLANET; nCntPlanet++, pPlanet++)
	{
		if (pPlanet->bUse == false)
		{
			continue;
		}

		float fDistance = 0.0f;	// 距離を格納
		bool bHeight = false;	// 高度内にいるかを判定
		// 2点間の距離を求める
		fDistance = sqrtf(((pos.x - pPlanet->pos.x) * (pos.x - pPlanet->pos.x)) + ((pos.z - pPlanet->pos.z) * (pos.z - pPlanet->pos.z))) * 0.5f;
		// 高度の判定を行う
		if (pos.y <= pPlanet->pos.y + PLANET_SIZE && pos.y >= pPlanet->pos.y - PLANET_SIZE)
		{ // 一定高度に収まっている場合フラグを立てる
			bHeight = true;
		}

		// すぐ近くに惑星がある場合、小惑星を配置しない
		if (bHeight == true && fDistance <= ASTEROID_SIZE)
		{
			return;
		}
	}

	for (int nCntAsteroid = 0; nCntAsteroid < MAX_ASTEROID; nCntAsteroid++)
	{
		if (g_aAsteroid[nCntAsteroid].bUse == true)
		{ // 使用われている場合は処理を繰り返す
			continue;
		}
		// 各種情報を格納
		g_aAsteroid[nCntAsteroid].type		= type;			// 小惑星の種類
		g_aAsteroid[nCntAsteroid].pos		= pos;			// 位置
		g_aAsteroid[nCntAsteroid].posParent = INIT_VECTOR3;	// 親の位置
		g_aAsteroid[nCntAsteroid].move		= move;			// 移動量
		g_aAsteroid[nCntAsteroid].rot		= ASTEROID_ROT;	// 回転方向
		g_aAsteroid[nCntAsteroid].fRadius	= 0.0f;			// 親惑星との距離
		g_aAsteroid[nCntAsteroid].fMove		= 0.0f;			// 移動方向
		g_aAsteroid[nCntAsteroid].fAngle	= RAND_ANGLE;	// 回転度数
		g_aAsteroid[nCntAsteroid].nIdx		= nCntAsteroid;	// インデックス
		g_aAsteroid[nCntAsteroid].bUse		= true;			// 使用状態
		// 演出面
		g_aAsteroid[nCntAsteroid].floatingX = (FLOATINGX)(rand() % 2);	// 浮遊状態[X]
		g_aAsteroid[nCntAsteroid].floatingY = (FLOATINGY)(rand() % 2);	// 浮遊状態[Y]
		g_aAsteroid[nCntAsteroid].floatingZ = (FLOATINGZ)(rand() % 2);	// 浮遊状態[Z]
		g_aAsteroid[nCntAsteroid].nKeyX = rand() % ASTEROID_KEY;		// 浮遊時間[X]
		g_aAsteroid[nCntAsteroid].nKeyY = rand() % ASTEROID_KEY;		// 浮遊時間[Y]
		g_aAsteroid[nCntAsteroid].nKeyZ = rand() % ASTEROID_KEY;		// 浮遊時間[Z]
		g_aAsteroid[nCntAsteroid].nNumKey = ASTEROID_KEY;				// 浮遊間隔を管理
		break;
	}
}

//=======================================================
// 小惑星の設定処理[衛星]
//=======================================================
void SetSarellite(D3DXVECTOR3 posParent, D3DXVECTOR3 move, float fRadius, float fMove)
{
	for (int nCntAsteroid = 0; nCntAsteroid < MAX_ASTEROID; nCntAsteroid++)
	{
		if (g_aAsteroid[nCntAsteroid].bUse == true)
		{ // 使用われている場合は処理を繰り返す
			continue;
		}
		// 各種情報を格納
		g_aAsteroid[nCntAsteroid].type = ASTEROIDTYPE_SARELLITE;	// 小惑星の種類
		g_aAsteroid[nCntAsteroid].posParent = posParent;			// 親の位置
		g_aAsteroid[nCntAsteroid].move = move;						// 移動量
		g_aAsteroid[nCntAsteroid].rot = ASTEROID_ROT;				// 回転方向
		g_aAsteroid[nCntAsteroid].fRadius = fRadius;				// 親惑星との距離
		g_aAsteroid[nCntAsteroid].fMove = fMove;					// 移動方向
		g_aAsteroid[nCntAsteroid].fAngle = RAND_ANGLE;				// 回転度数
		g_aAsteroid[nCntAsteroid].nIdx = nCntAsteroid;				// インデックス
		g_aAsteroid[nCntAsteroid].bUse = true;						// 使用状態
		g_aAsteroid[nCntAsteroid].pos = D3DXVECTOR3(posParent.x + sinf(g_aAsteroid[nCntAsteroid].fAngle) * g_aAsteroid[nCntAsteroid].fRadius, posParent.y + RAND_HEIGHT, posParent.z + cosf(g_aAsteroid[nCntAsteroid].fAngle) * g_aAsteroid[nCntAsteroid].fRadius);	// 位置

		// 演出面
		g_aAsteroid[nCntAsteroid].floatingX = (FLOATINGX)(rand() % 2);	// 浮遊状態[X]
		g_aAsteroid[nCntAsteroid].floatingY = (FLOATINGY)(rand() % 2);	// 浮遊状態[Y]
		g_aAsteroid[nCntAsteroid].floatingZ = (FLOATINGZ)(rand() % 2);	// 浮遊状態[Z]
		g_aAsteroid[nCntAsteroid].nKeyX = rand() % ASTEROID_KEY;		// 浮遊時間[X]
		g_aAsteroid[nCntAsteroid].nKeyY = rand() % ASTEROID_KEY;		// 浮遊時間[Y]
		g_aAsteroid[nCntAsteroid].nKeyZ = rand() % ASTEROID_KEY;		// 浮遊時間[Z]
		g_aAsteroid[nCntAsteroid].nNumKey = ASTEROID_KEY;		// 浮遊間隔を管理
		break;
	}
}

//=======================================================
// 小惑星の情報を返す[idx指定]
//=======================================================
Asteroid* GetAsteroid(int nIdx)
{
	return &g_aAsteroid[nIdx];
}

//=======================================================
// 小惑星の情報を返す[idx指定なし]
//=======================================================
Asteroid* GetAllAsteroid(void)
{
	return &g_aAsteroid[0];
}

//=======================================================
// 小惑星とプレイヤーとの当たり判定
//=======================================================
bool CollisionAsteroid(D3DXVECTOR3 pos, bool bBreak)
{
	Player* pPlayer = GetPlayer();
	GAMESTATE gameState = GetGAmeState();

	for (int nCntAsteroid = 0; nCntAsteroid < MAX_ASTEROID; nCntAsteroid++)
	{
		if (g_aAsteroid[nCntAsteroid].bUse == false)
		{
			continue;
		}

		float fDistance = 0.0f;	// 距離を格納
		bool bHeight = false;	// 高度内にいるかを判定
		// 2点間の距離を求める
		fDistance = sqrtf(((pos.x - g_aAsteroid[nCntAsteroid].pos.x) * (pos.x - g_aAsteroid[nCntAsteroid].pos.x)) + ((pos.z - g_aAsteroid[nCntAsteroid].pos.z) * (pos.z - g_aAsteroid[nCntAsteroid].pos.z))) * 0.5f;
		// 高度の判定を行う
		if (pos.y <= g_aAsteroid[nCntAsteroid].pos.y + ASTEROID_HEIGHT && pos.y >= g_aAsteroid[nCntAsteroid].pos.y - ASTEROID_HEIGHT)
		{ // 一定高度に収まっている場合フラグを立てる
			bHeight = true;
		}

		if (bHeight == true && fDistance <= ASTEROID_SIZE)
		{
			if (bBreak == true)
			{ // 破壊フラグが立っている
				g_aAsteroid[nCntAsteroid].bUse = false;
				PlaySound(SE_ROCKBREAK);
			}
			else if (bBreak == false)
			{ // 破壊フラグが立っていない
				pPlayer->bUse = false;
				PlaySound(SE_BOMB);

				if (gameState == GAMESTATE_NONE)
				{
					SetGameState(GAMESTATE_OVER, 60);
				}
			}
			PrintDebugProc("当たっている\n");
			return true;
		}
	}
	return false;
}

//=======================================================
// 小惑星のランダム配置処理[game]
//=======================================================
void SetRandomAsteroid(void)
{
	for (int nCntDepth = 0; nCntDepth < MAX_DEPTH; nCntDepth++)
	{
		for (int nCntArea = 0; nCntArea < AREA_ASTEROID; nCntArea++)
		{
			// 小惑星を配置
			SetAsteroid(ASTEROIDTYPE_FLOATING, ASTEROID_RAND, INIT_VECTOR3);
		}
	}
}

//=======================================================
// 小惑星のランダム配置処理[result]
//=======================================================
void SetResultAsteroid(void)
{
	for (int nCntAsteroid = 0; nCntAsteroid < RESULTRAND_MAX; nCntAsteroid++)
	{
		// 小惑星を配置
		SetAsteroid(ASTEROIDTYPE_FLOATING, RESULTRAND_POS, INIT_VECTOR3);
	}
}