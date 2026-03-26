//=============================================================================
// 
// スクリーン [screen.cpp]
// Author : 中澤 優奈
// 
//=============================================================================
#include "main.h"
#include "screen.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScreen = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureScreen = NULL;	// テクスチャへのポインタ
D3DXCOLOR g_colorScreen;	// ポリゴン（画面）の色
SCREEN_FADE g_ScreenFade = SCREEN_FADE_NONE;		// 画面の状態

//=============================================================================
// 画面の初期化処理
//=============================================================================
void InitScreen(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	g_colorScreen = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	g_ScreenFade = SCREEN_FADE_NONE;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\screen.png",
		&g_pTextureScreen);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScreen,
		NULL);

	VERTEX_2D* pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScreen->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = g_colorScreen;
	pVtx[1].col = g_colorScreen;
	pVtx[2].col = g_colorScreen;
	pVtx[3].col = g_colorScreen;

	// UV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffScreen->Unlock();
}

//=============================================================================
// 画面の終了処理
//=============================================================================
void UninitScreen(void)
{
	if (g_pTextureScreen != NULL)
	{// テクスチャの破棄
		g_pTextureScreen->Release();
		g_pTextureScreen = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScreen != NULL)
	{
		g_pVtxBuffScreen->Release();
		g_pVtxBuffScreen = NULL;
	}
}

//=============================================================================
// 画面の更新処理
//=============================================================================
void UpdateScreen(void)
{
	if (g_ScreenFade == SCREEN_FADE_IN)
	{
		g_colorScreen.a -= 0.005f;

		if (g_colorScreen.a <= 0.0f)
		{
			g_colorScreen.a = 0.0f;
			g_ScreenFade = SCREEN_FADE_OUT;
		}
	}
	else if (g_ScreenFade == SCREEN_FADE_OUT)
	{
		g_colorScreen.a += 0.005f;

		if (g_colorScreen.a >= 0.3f)
		{
			g_colorScreen.a = 0.3f;
			g_ScreenFade = SCREEN_FADE_IN;
		}
	}
	else if (g_ScreenFade == SCREEN_FADE_NONE)
	{
		g_colorScreen.a = 0.0f;
	}

	VERTEX_2D* pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScreen->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_colorScreen;
	pVtx[1].col = g_colorScreen;
	pVtx[2].col = g_colorScreen;
	pVtx[3].col = g_colorScreen;

	// 頂点バッファをアンロックする
	g_pVtxBuffScreen->Unlock();
}

//=============================================================================
// 画面の描画処理
//=============================================================================
void DrawScreen(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScreen, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScreen);	// ポリゴンの描画

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================================
// 画面の設定
//=======================================================
void SetScreen(SCREEN_FADE screen, D3DXCOLOR col)
{
	if (g_ScreenFade != screen)
	{// タイプが違うときだけ
		if (screen == SCREEN_FADE_OUT && g_ScreenFade != SCREEN_FADE_NONE)
		{// フェードアウトに出来るのは、何もしてない状態のときだけ
			return;
		}

		g_ScreenFade = screen;
		g_colorScreen = col;
		g_colorScreen.a = 0.0f;
	}
}