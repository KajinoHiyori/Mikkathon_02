//==================================================================================
// 
// 惑星に関する処理[planet.h]
// Author : Soki Inamura
// 
//==================================================================================

#ifndef _PLANET_H_
#define _PLANET_H_

// マクロ定義 ==================================================

#define PLANET_SET_ASTEROID		(10)	// 小惑星付きで生成する小惑星数

// 列挙型の定義 ================================================

// 惑星の種類
typedef enum
{	
	PLANETTYPE_ENERGY = 0,	// [0]エネルギー惑星	
	PLANETTYPE_ASTEROID,	// [1]小惑星付き惑星	
	PLANETTYPE_BLACKHOLE,	// [2]ブラックホール惑星	
	PLANETTYPE_WHITEHOLE,	// [3]ホワイトホール惑星	
	PLANETTYPE_CHANGE,		// [4]重力変わる惑星	
	PLANETTYPE_REPULSIVE,	// [5]斥力惑星	
	PLANETTYPE_SMALL,		// [6]一般(引力：小)惑星	
	PLANETTYPE_MID,			// [7]一般(引力：中)惑星	
	PLANETTYPE_BIG,			// [8]一般(引力：大)惑星	
	PLANETTYPE_GOAL,		// [9]ゴール	

	PLANETTYPE_MAX			// 最大値

}PLANETTYPE;

// 構造体の定義 ================================================

// 惑星モデルの情報
typedef struct
{
	int nIdxStageModel;		// モデルインデックス

	float fHitRadius;		// 当たり半径
	float fGravity;			// 重力
	float fRadius;			// 効果範囲
	D3DXVECTOR3 addRot;		// 回転速度

	bool bUse;				// 使用状態

}PlanetModelInfo;

// 惑星の情報
typedef struct
{
	PLANETTYPE type;		// 惑星の種類
	int nIdx;				// 惑星のインデックス

	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度

	D3DXMATRIX mtxWorld;	// ワールドマトリックス

	bool bUse;				// 使用状態

}Planet;

// プロトタイプ宣言 ============================================

void InitPlanet(void);    	// 惑星の初期化処理
void UninitPlanet(void);  	// 惑星の終了処理
void UpdatePlanet(void);  	// 惑星の更新処理
void DrawPlanet(void);		// 惑星の描画処理

void SetLoadPlanetInfo		// 惑星のモデルの読み込み処理
(int nIdxStage, float fHitRadius, float fGravity, float fRadius, D3DXVECTOR3 addRot);


void SetPlanetInfo			// 惑星の設定情報の設定
(PLANETTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

void SetLayoutPlanet(void);	// 惑星の配置情報から設定


void SetPlanet				// 惑星の設定処理
(PLANETTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

bool CollisionPlanet		// 惑星の当たり判定処理
(D3DXVECTOR3* pPos, float fRadius);

Planet* GetPlanet(void);	// 惑星の情報を返す処理

int GetNumPlanet(void);		// 惑星の総数を返す処理

#endif
