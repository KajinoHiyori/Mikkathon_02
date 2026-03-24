//=============================================================================
// 
// 燃料 [oil.cpp]
// Author : 中澤優奈
// 
//=============================================================================
#include "main.h"
#include "oil.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"
#include "color.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OIL				(2)										// 最大燃料数
#define MAX_WIDTH			(245.0f)								// メーターの最大の幅
#define MIN_PRESENT_X		(455.0f)								// 一番左のX座標

//*****************************************************************************
// 燃料構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	float fLength;					// 長さ
	bool bDisp;						// 表示するかどうか
}PresentMeter;

//*****************************************************************************
// 燃料データ情報
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	float fWidth;					// 幅
	float fHeight;					// 高さ
}PresentMeterInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePresentMeter[MAX_OIL] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPresentMeter = NULL;					// 頂点バッファへのポインタ
PresentMeter g_aPresentMeter[MAX_OIL];							// 燃料の情報
PresentMeterInfo g_aPresentMeterInfo[] =
{
	{D3DXVECTOR3(1130.0f, 590.0f, 0.0f), 150.0f, 150.0f},
	{D3DXVECTOR3(1130.0f, 600.0f, 0.0f), 70.0f, 70.0f},
};

//*****************************************************************************
// テクスチャファイル名
//*****************************************************************************
const char* c_apFilernamaPresentMeter[MAX_OIL] =
{
	"data\\TEXTURE\\presentframe.png",
	NULL,
};

#if 0
//=============================================================================
// 燃料の初期化処理
//=============================================================================
void InitPresentMeter(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_OIL; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilernamaPresentMeter[nCntTexture], &g_apTexturePresentMeter[nCntTexture]);
	}

	// 燃料の情報の初期化
	for (int nCntPresentMeter = 0; nCntPresentMeter < MAX_OIL; nCntPresentMeter++)
	{
		g_aPresentMeter[nCntPresentMeter].pos = g_aPresentMeterInfo[nCntPresentMeter].pos;
		g_aPresentMeter[nCntPresentMeter].rot = FIRST_ROT;
		g_aPresentMeter[nCntPresentMeter].fLength = sqrtf(g_aPresentMeterInfo[nCntPresentMeter].fWidth * g_aPresentMeterInfo[nCntPresentMeter].fWidth +
			g_aPresentMeterInfo[nCntPresentMeter].fHeight * g_aPresentMeterInfo[nCntPresentMeter].fHeight) * 0.5f;
		g_aPresentMeter[nCntPresentMeter].bDisp = true;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OIL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPresentMeter,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPresentMeter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPresentMeter = 0; nCntPresentMeter < MAX_OIL; nCntPresentMeter++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x - g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y - g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x + g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y - g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x - g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y + g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x + g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y + g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);

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
	g_pVtxBuffPresentMeter->Unlock();
}

//=============================================================================
// 燃料の終了処理
//=============================================================================
void UninitPresentMeter(void)
{
	// テクスチャの破棄
	for (int nCntPresentMeter = 0; nCntPresentMeter < MAX_OIL; nCntPresentMeter++)
	{
		if (g_apTexturePresentMeter[nCntPresentMeter] != NULL)
		{
			g_apTexturePresentMeter[nCntPresentMeter]->Release();
			g_apTexturePresentMeter[nCntPresentMeter] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPresentMeter != NULL)
	{
		g_pVtxBuffPresentMeter->Release();
		g_pVtxBuffPresentMeter = NULL;
	}
}

//=============================================================================
// 燃料の更新処理
//=============================================================================
void UpdatePresentMeter(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	Player* pPlayer = GetPlayer();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPresentMeter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPresentMeter = 0; nCntPresentMeter < MAX_OIL; nCntPresentMeter++)
	{
		if (nCntPresentMeter == 1)
		{// プレゼントだったら
			if (pPlayer->nMaxPresent == 0)
			{// 0になったら
				g_aPresentMeter[nCntPresentMeter].fLength = 0.0f;
			}
			else
			{// 0じゃないとき
				float fLength = sqrtf(g_aPresentMeterInfo[nCntPresentMeter].fWidth * g_aPresentMeterInfo[nCntPresentMeter].fWidth +
					g_aPresentMeterInfo[nCntPresentMeter].fHeight * g_aPresentMeterInfo[nCntPresentMeter].fHeight) * 0.5f;

				g_aPresentMeter[nCntPresentMeter].fLength = fLength * ((float)pPlayer->nMaxPresent / MAX_NUMPRESENT);

				if (g_aPresentMeter[nCntPresentMeter].fLength < 0.0f)
				{// もし超えたら戻す
					g_aPresentMeter[nCntPresentMeter].fLength = 0.0f;
				}
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x - g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y - g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x + g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y - g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x - g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y + g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPresentMeter[nCntPresentMeter].pos.x + g_aPresentMeter[nCntPresentMeter].fLength, g_aPresentMeter[nCntPresentMeter].pos.y + g_aPresentMeter[nCntPresentMeter].fLength, 0.0f);
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPresentMeter->Unlock();
}

//=============================================================================
// 燃料の描画処理
//=============================================================================
void DrawPresentMeter(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntPresentMeter = 0; nCntPresentMeter < MAX_OIL; nCntPresentMeter++)
	{
		if (g_aPresentMeter[nCntPresentMeter].bDisp == true)
		{// 表示するとき
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPresentMeter, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePresentMeter[nCntPresentMeter]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntPresentMeter * 4,		// 描画する最初の頂点インデックス
				2);		// 描画するプリミティブ数
		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
#endif