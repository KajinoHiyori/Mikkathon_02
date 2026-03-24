//==================================================================================
// 
// ステージに関する処理[stage.h]
// Author : Soki Inamura
// 
//==================================================================================

#ifndef _STAGE_H_
#define _STAGE_H_

// プロトタイプ宣言 ============================================

void InitStage();    	// 惑星の初期化処理
void UninitStage();  	// 惑星の終了処理
void UpdateStage();  	// 惑星の更新処理
void DrawStage();		// 惑星の描画処理

bool SetLoadStage		// 惑星の設定処理
(const char* pStageFilename);

#endif