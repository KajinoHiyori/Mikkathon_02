//==================================================================================
// 
// ステージに関する処理[stage.h]
// Author : Soki Inamura
// 
//==================================================================================

#ifndef _STAGE_H_
#define _STAGE_H_

// マクロ定義 ==================================================

#define MAX_TEX_STAGEMODEL	(64)	// ステージモデルのテクスチャの最大

// 構造体の定義 ================================================

// ステージモデルの情報
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_TEX_STAGEMODEL];	// モデルのテクスチャ情報
	LPD3DXMESH pMesh;								// モデルのメッシュ情報
	LPD3DXBUFFER pBuffMat;							// モデルのマテリアル情報

	DWORD dwNumMat;									// マテリアルの数

	bool bUse;										// 使用状態

}StageModelInfo;

// プロトタイプ宣言 ============================================

void InitStage(void);    			// ステージの初期化処理
void UninitStage(void);  			// ステージの終了処理
void UpdateStage(void);  			// ステージの更新処理
void DrawStage(void);				// ステージの描画処理

bool SetLoadStage					// ステージの設定処理
(const char* pStageFilename);

int SetLoadStageModelInfo			// ステージのモデルの読み込み処理
(const char* pModelFilename);

StageModelInfo *GetStageModelInfo	// ステージモデルの情報を返す処理
(int nIdx);

bool GetLoadStage					// ステージの読み込みが終わっているか返す処理
(void);	

void SetPosionStageObj				// オブジェクトの移動処理
(const char* pLabelName, D3DXVECTOR3* pPos, float fSpeed);

#endif