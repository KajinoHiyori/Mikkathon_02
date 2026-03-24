//=============================================================================
// 
// モデルヘッダー [model.h]
// Author : 中澤優奈
// 
//=============================================================================
#ifndef _MODEL_H_		// このマクロ定義がされていなかったら
#define _MODEL_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//*****************************************************************************
// モデル構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
}Model;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void CollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fDepth);
Model* GetModel(void);

#endif