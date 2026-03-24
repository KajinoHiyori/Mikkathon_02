//========================================================
// 
// ケプラーMk3 [fade.h]
// Author : Kajino Hiyori
// 
//========================================================
#ifndef _FADE_H_
#define	_FADE_H_
#include "main.h"

// フェードの状態
typedef enum
{
	FADE_NONE = 0,	// 何もしていない状態
	FADE_IN,		// フェードイン
	FADE_OUT,		// フェードアウト
	FADE_MAX
}FADE;

// プロトタイプ宣言
void InitFade(MODE modeNext, D3DXCOLOR col);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext, D3DXCOLOR col);
FADE GetFade(void);

#endif
