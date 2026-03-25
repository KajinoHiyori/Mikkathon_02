//=============================================================================
// 
// 背景画面 [bg.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "bg.h"
#include "color.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CENTER					(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// 中心座標
#define SCROLL_SPEED			(0.001f)								// スクロール速度

//*****************************************************************************
// 背景構造体を定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
}BG;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;			// 頂点バッファへのポインタ
BG g_BG;											// 背景の情報

//*****************************************************************************
// テクスチャファイル名
//*****************************************************************************
const char* c_apFilernamaBG[1] =
{
	"data\\TEXTURE\\space.png",
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
	for (int nCntTexture = 0; nCntTexture < 1; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilernamaBG[nCntTexture], &g_pTextureBG);
	}

	// 背景情報の初期化
	g_BG.pos = CENTER;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
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
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = COLOR_WHITE;
	pVtx[1].col = COLOR_WHITE;
	pVtx[2].col = COLOR_WHITE;
	pVtx[3].col = COLOR_WHITE;

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
	for (int nCntBG = 0; nCntBG < 1; nCntBG++)
	{
		if (g_pTextureBG != NULL)
		{
			g_pTextureBG->Release();
			g_pTextureBG = NULL;
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
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ

	// 描画順を調整
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBG);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,		// 描画する最初の頂点インデックス
		2);		// 描画するプリミティブ数

	// 描画順を戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}