//=============================================================================
// 
// プレイヤーヘッダー [player.h]
// Author : 中澤優奈
// 
//=============================================================================
#ifndef _PLAYER_H_		// このマクロ定義がされていなかったら
#define _PLAYER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE				(16)									// 最大テクスチャ数
#define FIRST_POS				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))			// 初期座標
#define CENTER					(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// 中心座標

//*****************************************************************************
// プレイヤーの状態
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		// 出現状態
	PLAYERSTATE_WAIT,			// 待機状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*****************************************************************************
// プレイヤーモデルの定義
//*****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;								// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;							// マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 apTexture[MAX_TEXTURE];		// テクスチャへのポインタ
	DWORD dwNumMat;									// マテリアルの数
}Player_Model;

//*****************************************************************************
// プレイヤー構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	float fAngleZ;				// 目的の向き
	float fSpeedZ;				// Z軸のスピード
	PLAYERSTATE state;			// 状態
	int nCounterState;			// 状態カウンター
	float fOil;					// 燃料
	bool bUse;					// 使用しているか
}Player;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pos);
Player* GetPlayer(void);

void CorrectAngle(float* fAngle, float fAngleCmp);

#endif