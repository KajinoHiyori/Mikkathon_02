//=============================================================================
// 
// タイトルUI [title_ui.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "main.h"
#include "title_ui.h"
#include "input.h"
#include "debugproc.h"
#include "color.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TITLEUI_NUM			(2)										// 最大タイトルUI数
#define TITLELOGO_HEIGHT		(250.0f)								// タイトルロゴの既定の高さ
#define WAIT_COUNT				(90)									// 待機の間隔
#define BLINK_COUNT				(5)										// 点滅の間隔

//*****************************************************************************
// タイトルUI状態
//*****************************************************************************
typedef enum
{
	TITLEUISTATE_NONE = 0,			// 何もしてない状態
	TITLEUISTATE_WAIT,				// 待機状態(PRESS)
	TITLEUISTATE_BLINK,				// 点滅状態(PRESS)
	TITLEUISTATE_MAX
}TITLEUISTATE;

//*****************************************************************************
// タイトルUI構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	TITLEUISTATE state;				// 状態
	int nCounterState;				// 状態カウンター
	bool bDisp;						// 表示するかどうか
}TitleUI;

//*****************************************************************************
// タイトルUIデータ情報
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	float fWidth;					// 幅
	float fHeight;					// 高さ
}TitleUIInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTitleUI[MAX_TITLEUI_NUM] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleUI = NULL;					// 頂点バッファへのポインタ
bool g_bFadeNext = false;											// 次のシーンに遷移できるかどうか
TitleUI g_aTitleUI[MAX_TITLEUI_NUM];								// タイトルUIの情報
TitleUIInfo g_aTitleUIInfo[] =
{
	{D3DXVECTOR3(900.0f, 610.0f, 0.0f), 300.0f, 60.0f},
	{D3DXVECTOR3(380.0f, -250.0f, 0.0f), 400.0f, 200.0f},
};

//*****************************************************************************
// テクスチャファイル名
//*****************************************************************************
const char* c_apFilernamaTitleUI[MAX_TITLEUI_NUM] =
{
	"data\\TEXTURE\\titleUI\\pressanybutton.png",
	"data\\TEXTURE\\titleUI\\titlelogo.png",
};

//=============================================================================
// タイトルUIの初期化処理
//=============================================================================
void InitTitleUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_TITLEUI_NUM; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilernamaTitleUI[nCntTexture], &g_apTextureTitleUI[nCntTexture]);
	}

	// タイトルUIの情報の初期化
	for (int nCntTitleUI = 0; nCntTitleUI < MAX_TITLEUI_NUM; nCntTitleUI++)
	{
		g_aTitleUI[nCntTitleUI].pos = g_aTitleUIInfo[nCntTitleUI].pos;
		g_aTitleUI[nCntTitleUI].state = (nCntTitleUI == 1) ? TITLEUISTATE_NONE : TITLEUISTATE_WAIT;
		g_aTitleUI[nCntTitleUI].nCounterState = 0;
		g_aTitleUI[nCntTitleUI].bDisp = true;
	}

	g_bFadeNext = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLEUI_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleUI,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitleUI = 0; nCntTitleUI < MAX_TITLEUI_NUM; nCntTitleUI++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x - g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y - g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x + g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y - g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x - g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y + g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x + g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y + g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);

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

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleUI->Unlock();
}

//=============================================================================
// タイトルUIの終了処理
//=============================================================================
void UninitTitleUI(void)
{
	// テクスチャの破棄
	for (int nCntTitleUI = 0; nCntTitleUI < MAX_TITLEUI_NUM; nCntTitleUI++)
	{
		if (g_apTextureTitleUI[nCntTitleUI] != NULL)
		{
			g_apTextureTitleUI[nCntTitleUI]->Release();
			g_apTextureTitleUI[nCntTitleUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleUI != NULL)
	{
		g_pVtxBuffTitleUI->Release();
		g_pVtxBuffTitleUI = NULL;
	}
}

//=============================================================================
// タイトルUIの更新処理
//=============================================================================
void UpdateTitleUI(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitleUI = 0; nCntTitleUI < MAX_TITLEUI_NUM; nCntTitleUI++)
	{
		if (nCntTitleUI == 0)
		{// PRESSだったら
			switch (g_aTitleUI[nCntTitleUI].state)
			{
			case TITLEUISTATE_NONE:
				break;

			case TITLEUISTATE_WAIT:
				if (g_aTitleUI[nCntTitleUI].nCounterState % WAIT_COUNT == 0 && g_aTitleUI[nCntTitleUI].bDisp == true)
				{// 一定間隔で点滅
					g_aTitleUI[nCntTitleUI].bDisp = false;
				}
				else if (g_aTitleUI[nCntTitleUI].nCounterState % (WAIT_COUNT / 2) == 0 && g_aTitleUI[nCntTitleUI].bDisp == false)
				{// 一定間隔で点滅
					g_aTitleUI[nCntTitleUI].bDisp = true;
				}

				if (g_aTitleUI[1].pos.y == TITLELOGO_HEIGHT &&
					((GetKeyboardTrigger(DIK_RETURN) == true) || GetJoypadTrigger(JOYKEY_A) == true || GetJoypadTrigger(JOYKEY_START) == true))
				{// ロゴが規定の高さのときにキーが押されたら
					g_aTitleUI[nCntTitleUI].state = TITLEUISTATE_BLINK;
					g_aTitleUI[nCntTitleUI].nCounterState = WAIT_COUNT;
				}

				g_aTitleUI[nCntTitleUI].nCounterState++;

				break;

			case TITLEUISTATE_BLINK:
				if (g_aTitleUI[nCntTitleUI].nCounterState <= 0)
				{// カウンターがなくなったら遷移可能にする
					g_bFadeNext = true;
					g_aTitleUI[nCntTitleUI].nCounterState = 0;
				}
				else if (g_aTitleUI[nCntTitleUI].nCounterState % BLINK_COUNT == 0)
				{// 点滅
					g_aTitleUI[nCntTitleUI].bDisp = (g_aTitleUI[nCntTitleUI].bDisp == true) ? false : true;
				}

				g_aTitleUI[nCntTitleUI].nCounterState--;

				break;
			}
		}

		if (nCntTitleUI == 1)
		{// ロゴだったら
			if (g_aTitleUI[nCntTitleUI].pos.y >= TITLELOGO_HEIGHT)
			{// 既定の高さになった
				g_aTitleUI[nCntTitleUI].pos.y = TITLELOGO_HEIGHT;
			}
			else if ((GetKeyboardTrigger(DIK_RETURN) == true) || GetJoypadTrigger(JOYKEY_A) == true || GetJoypadTrigger(JOYKEY_START) == true)
			{// 移動スキップ
				g_aTitleUI[nCntTitleUI].pos.y = TITLELOGO_HEIGHT;
			}
			else
			{// まだついていないとき
				g_aTitleUI[nCntTitleUI].pos.y += 3.0f;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x - g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y - g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x + g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y - g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x - g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y + g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTitleUI[nCntTitleUI].pos.x + g_aTitleUIInfo[nCntTitleUI].fWidth, g_aTitleUI[nCntTitleUI].pos.y + g_aTitleUIInfo[nCntTitleUI].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleUI->Unlock();
}

//=============================================================================
// タイトルUIの描画処理
//=============================================================================
void DrawTitleUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntTitleUI = 0; nCntTitleUI < MAX_TITLEUI_NUM; nCntTitleUI++)
	{
		if (g_aTitleUI[nCntTitleUI].bDisp == true)
		{// 表示するとき
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffTitleUI, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitleUI[nCntTitleUI]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntTitleUI * 4,		// 描画する最初の頂点インデックス
				2);		// 描画するプリミティブ数
		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=============================================================================
// 次のシーンに遷移できるかどうか取得
//=============================================================================
bool GetTitleFinish(void)
{
	return g_bFadeNext;
}