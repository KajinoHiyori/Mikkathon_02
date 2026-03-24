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
#define MAX_OILNUM			(2)										// 最大燃料数
#define MAX_HEIGHT			(160.0f)								// メーターの最大の高さ

//*****************************************************************************
// 燃料構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	float fLength;					// 長さ
	bool bDisp;						// 表示するかどうか
}Oil;

//*****************************************************************************
// 燃料データ情報
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	float fWidth;					// 幅
	float fHeight;					// 高さ
}OilInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureOil[MAX_OILNUM] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOil = NULL;					// 頂点バッファへのポインタ
Oil g_aOil[MAX_OILNUM];							// 燃料の情報
OilInfo g_aOilInfo[] =
{
	{D3DXVECTOR3(50.0f, 700.0f, 0.0f), 30.0f, 70.0f},
	{D3DXVECTOR3(50.0f, 610.0f, 0.0f), 50.0f, 100.0f},
};

//*****************************************************************************
// テクスチャファイル名
//*****************************************************************************
const char* c_apFilernamaOil[MAX_OILNUM] =
{
	NULL,
	"data\\TEXTURE\\oil000.png",
};

//=============================================================================
// 燃料の初期化処理
//=============================================================================
void InitOil(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_OILNUM; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilernamaOil[nCntTexture], &g_apTextureOil[nCntTexture]);
	}

	// 燃料の情報の初期化
	for (int nCntOil = 0; nCntOil < MAX_OILNUM; nCntOil++)
	{
		g_aOil[nCntOil].pos = g_aOilInfo[nCntOil].pos;
		g_aOil[nCntOil].fLength = 0.0f;
		g_aOil[nCntOil].bDisp = true;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OILNUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOil,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOil->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOil = 0; nCntOil < MAX_OILNUM; nCntOil++)
	{
		// 頂点座標の設定
		if (nCntOil == 0)
		{// メーター
			pVtx[0].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x - g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y - g_aOil[nCntOil].fLength, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x + g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y - g_aOil[nCntOil].fLength, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x - g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x + g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y, 0.0f);
		}
		else if (nCntOil == 1)
		{// フレーム
			pVtx[0].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x - g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y - g_aOilInfo[nCntOil].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x + g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y - g_aOilInfo[nCntOil].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x - g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y + g_aOilInfo[nCntOil].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x + g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y + g_aOilInfo[nCntOil].fHeight, 0.0f);
		}

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
	g_pVtxBuffOil->Unlock();
}

//=============================================================================
// 燃料の終了処理
//=============================================================================
void UninitOil(void)
{
	// テクスチャの破棄
	for (int nCntOil = 0; nCntOil < MAX_OILNUM; nCntOil++)
	{
		if (g_apTextureOil[nCntOil] != NULL)
		{
			g_apTextureOil[nCntOil]->Release();
			g_apTextureOil[nCntOil] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffOil != NULL)
	{
		g_pVtxBuffOil->Release();
		g_pVtxBuffOil = NULL;
	}
}

//=============================================================================
// 燃料の更新処理
//=============================================================================
void UpdateOil(void)
{
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ
	Player* pPlayer = GetPlayer();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOil->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOil = 0; nCntOil < MAX_OILNUM; nCntOil++)
	{
		if (nCntOil == 0)
		{// メーターだったら
			if (pPlayer->fOil < 0.0f)
			{// 0になったら
				g_aOil[nCntOil].fLength = 0.0f;
			}
			else
			{// 0じゃないとき
				g_aOil[nCntOil].fLength = ((float)pPlayer->fOil / MAX_OIL) * MAX_HEIGHT;

				if (g_aOil[nCntOil].fLength < 0.0f)
				{// もし超えたら戻す
					g_aOil[nCntOil].fLength = 0.0f;
				}
				else if (g_aOil[nCntOil].fLength > MAX_HEIGHT)
				{// もし超えたら戻す
					g_aOil[nCntOil].fLength = MAX_HEIGHT;
				}
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x - g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y - g_aOil[nCntOil].fLength, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x + g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y - g_aOil[nCntOil].fLength, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x - g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aOil[nCntOil].pos.x + g_aOilInfo[nCntOil].fWidth, g_aOil[nCntOil].pos.y, 0.0f);

			// 頂点カラーの設定
			if (g_aOil[nCntOil].fLength <= MAX_HEIGHT * 0.2f)
			{// 危険
				pVtx[0].col = COLOR_RED;
				pVtx[1].col = COLOR_RED;
				pVtx[2].col = COLOR_RED;
				pVtx[3].col = COLOR_RED;
			}
			else if (g_aOil[nCntOil].fLength <= MAX_HEIGHT * 0.5f)
			{// 注意
				pVtx[0].col = COLOR_YELLOW;
				pVtx[1].col = COLOR_YELLOW;
				pVtx[2].col = COLOR_YELLOW;
				pVtx[3].col = COLOR_YELLOW;
			}
			else
			{// 良好
				pVtx[0].col = COLOR_GREEN;
				pVtx[1].col = COLOR_GREEN;
				pVtx[2].col = COLOR_GREEN;
				pVtx[3].col = COLOR_GREEN;
			}
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffOil->Unlock();
}

//=============================================================================
// 燃料の描画処理
//=============================================================================
void DrawOil(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntOil = 0; nCntOil < MAX_OILNUM; nCntOil++)
	{
		if (g_aOil[nCntOil].bDisp == true)
		{// 表示するとき
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffOil, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureOil[nCntOil]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntOil * 4,		// 描画する最初の頂点インデックス
				2);		// 描画するプリミティブ数
		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=============================================================================
// 燃料の描画設定
//=============================================================================
void SetOilDisp(bool bDisp)
{
	g_aOil[0].bDisp = bDisp;
	g_aOil[1].bDisp = bDisp;
}