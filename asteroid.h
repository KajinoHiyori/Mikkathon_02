//=============================================================================
// 
// 小惑星管理処理 [asteroid.h]
// Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#include "main.h"

// マクロ定義
#define MAX_ASTEROID	(256)	// 小惑星の最大数

// 小惑星の種類
typedef enum
{
	ASTEROIDTYPE_NONE = 0,	// 初期化用
	ASTEROIDTYPE_FLOATING,	// フィールド内で浮遊
	ASTEROIDTYPE_SARELLITE,	// 衛星
	ASTEROIDTYPE_WALL,		// プレイヤーの行動制限
	ASTEROIDTYPE_MAX
}ASTEROIDTYPE;

// 小惑星の構造体
typedef struct
{
	ASTEROIDTYPE	type;		// 小惑星の種類
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		posParent;	// 親の位置
	D3DXVECTOR3		move;		// 移動量
	D3DXVECTOR3		rot;		// 回転方向
	int				nIdx;		// インデックス
	bool			bUse;		// 使用状態
}Asteroid;

// プロトタイプ宣言
void InitAsteroid(void);
void UninitAsteroid(void);
void UpdateAsteroid(void);
void DrawAsteroid(void);
void SetAsteroid(ASTEROIDTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 posParent, D3DXVECTOR3 move);
void SetSarellite(D3DXVECTOR3 posParent, D3DXVECTOR3 move, float fRadius);
Asteroid* GetAsteroid(int nIdx);

#endif