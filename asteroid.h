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

// 小惑星の浮遊状態を管理
typedef enum
{
	FLOATINGX_ADD = 0,	// 右方向に浮遊
	FLOATINGX_DIS,		// 左方向に浮遊
}FLOATINGX;

typedef enum
{
	FLOATINGY_ADD = 0,	// 上方向に浮遊
	FLOATINGY_DIS,		// 下方向に浮遊
}FLOATINGY;

typedef enum
{
	FLOATINGZ_ADD = 0,	// 奥方向に浮遊
	FLOATINGZ_DIS,		// 手前方向に浮遊
}FLOATINGZ;

// 小惑星の構造体
typedef struct
{
	ASTEROIDTYPE	type;		// 小惑星の種類
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		posParent;	// 親の位置
	D3DXVECTOR3		move;		// 移動量
	D3DXVECTOR3		rot;		// 回転
	float			fRadius;	// 親惑星との距離
	float			fMove;		// 回転速度及び方向
	int				nIdx;		// インデックス
	bool			bUse;		// 使用状態
	// 演出面
	FLOATINGX		floatingX;	// 浮遊状態[X]
	FLOATINGY		floatingY;	// 浮遊状態[Y]
	FLOATINGZ		floatingZ;	// 浮遊状態[Z]
	int				nKeyX;		// 浮遊時間[X]
	int				nKeyY;		// 浮遊時間[Y]
	int				nKeyZ;		// 浮遊時間[Z]
	int				nNumKey;	// 浮遊間隔を管理
}Asteroid;

// プロトタイプ宣言
void InitAsteroid(void);
void UninitAsteroid(void);
void UpdateAsteroid(void);
void DrawAsteroid(void);
void SetAsteroid(ASTEROIDTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 move);
void SetSarellite(D3DXVECTOR3 posParent, D3DXVECTOR3 move, float fRadius, float fMove);
Asteroid* GetAsteroid(int nIdx);
Asteroid* GetAllAsteroid(void);
bool CollisionAsteroid(D3DXVECTOR3 pos, bool bBreak);

#endif