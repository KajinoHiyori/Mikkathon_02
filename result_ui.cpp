//=============================================================================
// 
// リザルトUI [result_ui.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "main.h"
#include "result_ui.h"
#include "input.h"
#include "debugproc.h"
#include "color.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RESULTUI_NUM		(2)										// 最大リザルトUI数

//*****************************************************************************
// リザルトUI構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	bool bDisp;						// 表示するかどうか
}ResultUI;

//*****************************************************************************
// リザルトUIデータ情報
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	float fWidth;					// 幅
	float fHeight;					// 高さ
}ResultUIInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResultUI[MAX_RESULTUI_NUM] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultUI = NULL;					// 頂点バッファへのポインタ
ResultUI g_ResultUI;												// リザルトUIの情報
ResultUIInfo g_aResultUIInfo[] =
{
	{D3DXVECTOR3(640.0f, 100.0f, 0.0f), 300.0f, 60.0f},
};

//*****************************************************************************
// テクスチャファイル名
//*****************************************************************************
const char* c_apFilernamaResultUI[MAX_RESULTUI_NUM] =
{
	"data\\TEXTURE\\resultUI\\result000.png",
	"data\\TEXTURE\\resultUI\\result001.png",
};

//=============================================================================
// リザルトUIの初期化処理
//=============================================================================
void InitResultUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_RESULTUI_NUM; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilernamaResultUI[nCntTexture], &g_apTextureResultUI[nCntTexture]);
	}

	// リザルトUIの情報の初期化
	g_ResultUI.pos = g_aResultUIInfo[0].pos;
	g_ResultUI.bDisp = true;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultUI,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_ResultUI.pos.x - g_aResultUIInfo[0].fWidth, g_ResultUI.pos.y - g_aResultUIInfo[0].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_ResultUI.pos.x + g_aResultUIInfo[0].fWidth, g_ResultUI.pos.y - g_aResultUIInfo[0].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_ResultUI.pos.x - g_aResultUIInfo[0].fWidth, g_ResultUI.pos.y + g_aResultUIInfo[0].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_ResultUI.pos.x + g_aResultUIInfo[0].fWidth, g_ResultUI.pos.y + g_aResultUIInfo[0].fHeight, 0.0f);

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
	g_pVtxBuffResultUI->Unlock();
}

//=============================================================================
// リザルトUIの終了処理
//=============================================================================
void UninitResultUI(void)
{
	// テクスチャの破棄
	for (int nCntResultUI = 0; nCntResultUI < MAX_RESULTUI_NUM; nCntResultUI++)
	{
		if (g_apTextureResultUI[nCntResultUI] != NULL)
		{
			g_apTextureResultUI[nCntResultUI]->Release();
			g_apTextureResultUI[nCntResultUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResultUI != NULL)
	{
		g_pVtxBuffResultUI->Release();
		g_pVtxBuffResultUI = NULL;
	}
}

//=============================================================================
// リザルトUIの更新処理
//=============================================================================
void UpdateResultUI(void)
{
}

//=============================================================================
// リザルトUIの描画処理
//=============================================================================
void DrawResultUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	if (g_ResultUI.bDisp == true)
	{// 表示するとき
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResultUI, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		if (GetGAmeState() == GAMESTATE_CLEAR)
		{// ゲームクリア
			pDevice->SetTexture(0, g_apTextureResultUI[0]);
		}
		else if (GetGAmeState() == GAMESTATE_OVER)
		{// ゲームクリア
			pDevice->SetTexture(0, g_apTextureResultUI[1]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,		// 描画する最初の頂点インデックス
			2);		// 描画するプリミティブ数
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}