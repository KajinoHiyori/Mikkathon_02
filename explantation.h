//=============================================================================
//
//	説明処理 [explantation.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _EXPLANTATION_H_
#define	_EXPLANTATION_H_

// 説明の種類
typedef enum
{
	EXPLANTATIONTYPE_MOVE = 0,	// 操作方法
	EXPLANTATIONTYPE_DEST,		// 目的地
	EXPLANTATIONTYPE_OIL,		// オイル
	EXPLANTATIONTYPE_PLANET,	// 惑星
	EXPLANTATIONTYPE_SARELLITE,	// 衛星
	EXPLANTATIONTYPE_GOODLUCK,	// Good Luck
	EXPLANTATIONTYPE_MAX
}EXPLANTATIONTYPE;

// プロトタイプ宣言
void InitExplantation(void);
void UninitExplantation(void);
void UpdateExplantation(void);
void DrawExplantation(void);
void SetExplantation(EXPLANTATIONTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

void SetExplantationAppear(int nIdx);
void SetExplantationDisp(int nIdx);
void SetExplantationDisappear(int nIdx);
void SetExplantationNonDisp(int nIdx);
#endif