//=============================================================================
// 
// 背景画面 [bg.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_VERTEX				(4)										// 最大頂点数
#define MAX_POLYGON				(2)										// 最大ポリゴン数
#define DEFAULT_RHW				(1.0f)									// rhwの規定値
#define CENTER					(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// 中心座標
#define WHITE_VTX				(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 頂点カラーが白
#define SCROLL_SPEED			(0.001f)								// スクロール速度
#define ANIM_SPEED				(1)										// アニメーション切り替え速度
#define NUM_BG					(2)										// 背景の数

//*****************************************************************************
// 背景構造体を定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	int nCounterAnim;							// アニメーションカウンター
	int nPatternAnim;							// アニメーションのパターン
}BG;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBG[NUM_BG] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;			// 頂点バッファへのポインタ
BG g_BG;											// 背景の情報

//*****************************************************************************
// テクスチャファイル名
//*****************************************************************************
const char* c_apFilernamaBG[NUM_BG] =
{
	"data\\TEXTURE\\bg000.png",
	"data\\TEXTURE\\bg001.png",
};

//=============================================================================
// 背景の初期化処理
//=============================================================================
void InitBG(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < NUM_BG; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilernamaBG[nCntTexture], &g_apTextureBG[nCntTexture]);
	}

	// 背景情報の初期化
	g_BG.pos = CENTER;
	g_BG.nCounterAnim = 0;
	g_BG.nPatternAnim = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = DEFAULT_RHW;
	pVtx[1].rhw = DEFAULT_RHW;
	pVtx[2].rhw = DEFAULT_RHW;
	pVtx[3].rhw = DEFAULT_RHW;

	// 頂点カラーの設定
	pVtx[0].col = WHITE_VTX;
	pVtx[1].col = WHITE_VTX;
	pVtx[2].col = WHITE_VTX;
	pVtx[3].col = WHITE_VTX;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//=============================================================================
// 背景の終了処理
//=============================================================================
void UninitBG(void)
{
	// テクスチャの破棄
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_apTextureBG[nCntBG] != NULL)
		{
			g_apTextureBG[nCntBG]->Release();
			g_apTextureBG[nCntBG] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=============================================================================
// 背景の更新処理
//=============================================================================
void UpdateBG(void)
{
#if 0
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	g_BG.nCounterAnim++;		// カウンターを加算

	if (g_BG.nCounterAnim % ANIM_SPEED == 0)
	{// 一定時間経過した
		g_BG.nPatternAnim++;		// パターンNO.を更新

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex.x = g_BG.nPatternAnim * SCROLL_SPEED;
		pVtx[1].tex.x = g_BG.nPatternAnim * SCROLL_SPEED + 1.0f;
		pVtx[2].tex.x = g_BG.nPatternAnim * SCROLL_SPEED;
		pVtx[3].tex.x = g_BG.nPatternAnim * SCROLL_SPEED + 1.0f;
		pVtx[0].tex.y = g_BG.nPatternAnim * -SCROLL_SPEED;
		pVtx[1].tex.y = g_BG.nPatternAnim * -SCROLL_SPEED;
		pVtx[2].tex.y = g_BG.nPatternAnim * -SCROLL_SPEED + 1.0f;
		pVtx[3].tex.y = g_BG.nPatternAnim * -SCROLL_SPEED + 1.0f;

		// 頂点バッファをアンロックする
		g_pVtxBuffBG->Unlock();
	}
#endif
}

//=============================================================================
// 背景の描画処理
//=============================================================================
void DrawBG(void)
{
	// モードを取得
	MODE mode = GetMode();

	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (mode == MODE_GAME)
	{// ゲーム
		pDevice->SetTexture(0, g_apTextureBG[1]);
	}
	else
	{// それ以外
		pDevice->SetTexture(0, g_apTextureBG[0]);
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,		// 描画する最初の頂点インデックス
		MAX_POLYGON);		// 描画するプリミティブ数
}