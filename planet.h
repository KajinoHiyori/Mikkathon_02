//==================================================================================
// 
// 惑星に関する処理[planet.h]
// Author : Soki Inamura
// 
//==================================================================================

#ifndef _PLANET_H_
#define _PLANET_H_

// 列挙型の定義 ================================================

// 惑星の種類
typedef enum
{
	PLANETTYPE_NONE = 0,	// [0]普通惑星	

	PLANETTYPE_MAX			// 最大値

}PLANETTYPE;

// 構造体の定義 ================================================

// 惑星モデルの情報
typedef struct
{
	int nIdxStageModel;		// モデルインデックス

	float fRadius;			// 半径

	bool bUse;				// 使用状態

}PlanetModelInfo;

// 惑星の情報
typedef struct
{
	PLANETTYPE type;		// 惑星の種類

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

void SetPlanet				// 惑星の設定処理
(PLANETTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

bool CollisionPlanet		// 惑星の当たり判定処理
(D3DXVECTOR3 pos1, float fRadius1,
	D3DXVECTOR3 pos2, float fRadius2);

void SetLoadPlanetInfo		// 惑星のモデルの読み込み処理
(int nIdxStage, float fRadius);



#endif
