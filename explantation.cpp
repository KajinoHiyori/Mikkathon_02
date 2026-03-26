//======================================================================================
// 
// チュートリアルの2DUI処理[explantation.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "color.h"
#include "fade.h"
#include "explantation.h"
#include "title.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"
// 説明演出の管理
typedef enum
{
	EXPLANTATIONSTATE_NONDISPLAY = 0,	// 非表示
	EXPLANTATIONSTATE_APPEAR,			// 出現
	EXPLANTATIONSTATE_DISPLAY,		// 表示
	EXPLANTATIONSTATE_DISAPPEAR,		// 収縮
}EXPLANTATIONSTATE;

// 説明の構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	EXPLANTATIONTYPE	type;		// 種類
	EXPLANTATIONSTATE state;		// 状態
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}Explantation;

// マクロ定義
#define MAX_EXPLANTATION	(EXPLANTATIONTYPE_MAX)	// テクスチャの最大数
#define WIDTH				(300.0f * 0.5f)	// 横幅
#define HEIGHT				(200.0f * 0.5f)	// 縦幅
#define UI_KEY				(30)		// UIのキー数
#define UI_ROT				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 表示方向
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル
#define POS_MOVE			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 操作方法
#define POS_DEST			(D3DXVECTOR3(-500.0f, 0.0f, 1000.0f))	// 目的地
#define POS_OIL				(D3DXVECTOR3(0.0f, 0.0f, 1200.0f))	// オイル
#define POS_PLANET			(D3DXVECTOR3(0.0f, 0.0f, 300.0f))	// 惑星
#define POS_SARELLITE		(D3DXVECTOR3(0.0f, 0.0f, 400.0f))	// 衛星
#define APPEAR_SIZE			(250.0f)	// 出現の当たり判定を管理するサイズ

// テクスチャの読み込み
const char* c_apFilenameExplantation[MAX_EXPLANTATION] =
{
	"data\\TEXTURE\\explantation\\explantation000.png",	// EXPLANTATIONTYPE_PLAYPAD
	"data\\TEXTURE\\explantation\\explantation001.png",	// EXPLANTATIONTYPE_PLAYKEY	
	"data\\TEXTURE\\explantation\\explantation002.png",	// EXPLANTATIONTYPE_MAGICPAD
	"data\\TEXTURE\\explantation\\explantation003.png",	// EXPLANTATIONTYPE_MAGICKEY
	"data\\TEXTURE\\explantation\\explantation004.png",	// EXPLANTATIONTYPE_TIMELIMIT
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureExplantation[MAX_EXPLANTATION] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplantation = NULL; // 頂点バッファへのポインタ
Explantation g_aExplantation[MAX_EXPLANTATION];	// 構造体

int g_nSelectTypeExplantation = EXPLANTATIONTYPE_MOVE;	// 表示されているUIの種類

//======================================================================================
// Explantationの初期化処理
//======================================================================================
void InitExplantation(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameExplantation[nCntUI], &g_apTextureExplantation[nCntUI]);
	}

	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		g_aExplantation[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
		g_aExplantation[nCntUI].rot = UI_ROT;						// 向き
		g_aExplantation[nCntUI].type = EXPLANTATIONTYPE_MOVE;		// 種類
		g_aExplantation[nCntUI].state = EXPLANTATIONSTATE_NONDISPLAY;	// 出現モード
		g_aExplantation[nCntUI].fWidth = WIDTH;						// 幅
		g_aExplantation[nCntUI].fWidthDest = WIDTH;						// 幅の目的値
		g_aExplantation[nCntUI].fHeight = HEIGHT;						// 高さ
		g_aExplantation[nCntUI].fHeightDest = HEIGHT;						// 高さの目的値
		g_aExplantation[nCntUI].nNumKey = UI_KEY;						// 浮遊感をカウントするキー数
		g_aExplantation[nCntUI].nKey = 0;							// 現在のキー数
		g_aExplantation[nCntUI].bDisp = false;						// 表示状態

		SetExplantationNonDisp(nCntUI);
	}

	g_nSelectTypeExplantation = EXPLANTATIONTYPE_MOVE;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLANTATION, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffExplantation, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplantation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aExplantation[nCntUI].fWidth, g_aExplantation[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplantation[nCntUI].fWidth, g_aExplantation[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aExplantation[nCntUI].fWidth, -g_aExplantation[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplantation[nCntUI].fWidth, -g_aExplantation[nCntUI].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_UIBUBBLE;
		pVtx[1].col = COLOR_UIBUBBLE;
		pVtx[2].col = COLOR_UIBUBBLE;
		pVtx[3].col = COLOR_UIBUBBLE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// 頂点バッファをアンロック
	g_pVtxBuffExplantation->Unlock();

	// 説明用のUIを設置
	SetExplantation(EXPLANTATIONTYPE_MOVE,      D3DXVECTOR3(0.0f,0.0f,100.0f), UI_ROT);
	SetExplantation(EXPLANTATIONTYPE_DEST,      D3DXVECTOR3(0.0f,0.0f,100.0f), UI_ROT);
	SetExplantation(EXPLANTATIONTYPE_OIL,       D3DXVECTOR3(0.0f,0.0f,100.0f), UI_ROT);
	SetExplantation(EXPLANTATIONTYPE_PLANET,    D3DXVECTOR3(0.0f,0.0f,100.0f), UI_ROT);
	SetExplantation(EXPLANTATIONTYPE_SARELLITE, D3DXVECTOR3(0.0f,0.0f,100.0f), UI_ROT);
}

//======================================================================================
// Explantationの終了処理
//======================================================================================
void UninitExplantation(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		if (g_apTextureExplantation[nCntUI] != NULL)
		{
			g_apTextureExplantation[nCntUI]->Release();
			g_apTextureExplantation[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffExplantation != NULL)
	{
		g_pVtxBuffExplantation->Release();
		g_pVtxBuffExplantation = NULL;
	}

}

//======================================================================================
// Explantationの更新処理
//======================================================================================
void UpdateExplantation(void)
{
	Player* pPlayer = GetPlayer();	// プレイヤーの情報を取得
	float fDiffKey, fRateKey = 0.0f;
	bool bDisp[MAX_EXPLANTATION] = { false, false, false, false, false};

	if (pPlayer->bUse == false)
	{
		return;
	}
#if 0
	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		// プレイヤーが設置位置に近づいた場合展開する
		if (pPlayer->pos.x >= g_aExplantation[nCntUI].pos.x - APPEAR_SIZE &&	// 一定範囲より右にある
			pPlayer->pos.x <= g_aExplantation[nCntUI].pos.x + APPEAR_SIZE &&	// 一定範囲より左にある
			pPlayer->pos.z >= g_aExplantation[nCntUI].pos.z - APPEAR_SIZE &&	// 一定範囲より奥にある
			pPlayer->pos.z <= g_aExplantation[nCntUI].pos.z + APPEAR_SIZE)
		{
			bDisp[nCntUI] = true;
			if (g_aExplantation[nCntUI].state == EXPLANTATIONSTATE_NONDISPLAY)
			{
				SetExplantationAppear(nCntUI);
			}
		}
		else if (g_aExplantation[nCntUI].state == EXPLANTATIONSTATE_APPEAR || // 出現状態
			g_aExplantation[nCntUI].state == EXPLANTATIONSTATE_DISPLAY)	// 表示状態

		{
			SetExplantationDisappear(nCntUI);
		}
	}
#else
	// 表示されているUIの状態の設定処理
	if (g_nSelectTypeExplantation < MAX_EXPLANTATION)
	{// まだ表示していないのがある場合

		// 状態による設定処理
		switch (g_aExplantation[g_nSelectTypeExplantation].state)
		{
		case EXPLANTATIONSTATE_NONDISPLAY:	// 非表示

			// 出現状態の設定
			SetExplantationAppear(g_nSelectTypeExplantation);

			break;

		case EXPLANTATIONSTATE_DISPLAY:		// 表示

			if (GetKeyboardTrigger(DIK_RETURN))
			{// ENTERが押された

				// 収縮状態の設定
				SetExplantationDisappear(g_nSelectTypeExplantation);
			}

			break;
		}
	}
	

#endif

	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		switch (g_aExplantation[nCntUI].state)
		{
		case EXPLANTATIONSTATE_NONDISPLAY:	// 非表示
			g_aExplantation[nCntUI].bDisp = false;
			break;

		case EXPLANTATIONSTATE_APPEAR:	// 出現
			// 背景の高度変更
			fRateKey = (float)g_aExplantation[nCntUI].nKey / (float)g_aExplantation[nCntUI].nNumKey;
			fDiffKey = g_aExplantation[nCntUI].fHeightDest - g_aExplantation[nCntUI].fHeight;
			g_aExplantation[nCntUI].fHeight = g_aExplantation[nCntUI].fHeight + fDiffKey * fRateKey;

			// 中心位置からの位置を求める
			g_aExplantation[nCntUI].nKey++;

			if (g_aExplantation[nCntUI].nKey > g_aExplantation[nCntUI].nNumKey)
			{
				SetExplantationDisp(nCntUI);
			}
			break;

		case EXPLANTATIONSTATE_DISPLAY:	// 表示
			g_aExplantation[nCntUI].bDisp = true;
			break;

		case EXPLANTATIONSTATE_DISAPPEAR:	// 収縮
			// 背景の高度変更
			fRateKey = (float)g_aExplantation[nCntUI].nKey / (float)g_aExplantation[nCntUI].nNumKey;
			fDiffKey = g_aExplantation[nCntUI].fHeightDest - g_aExplantation[nCntUI].fHeight;
			g_aExplantation[nCntUI].fHeight = g_aExplantation[nCntUI].fHeight + fDiffKey * fRateKey;

			// 中心位置からの位置を求める
			g_aExplantation[nCntUI].nKey++;

			if (g_aExplantation[nCntUI].nKey > g_aExplantation[nCntUI].nNumKey)
			{
				SetExplantationNonDisp(nCntUI);

				g_nSelectTypeExplantation++;	// UIを次の種類に設定する
			}
			break;
		}
	}

	// モードの切り替え
	if (g_nSelectTypeExplantation == MAX_EXPLANTATION)
	{// 全て確認された場合

		// ゲームモードの設定
		SetGameState(GAMESTATE_NONE, 30);
	}

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplantation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aExplantation[nCntUI].fWidth,	 g_aExplantation[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_aExplantation[nCntUI].fWidth,	 g_aExplantation[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aExplantation[nCntUI].fWidth,	-g_aExplantation[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_aExplantation[nCntUI].fWidth,	-g_aExplantation[nCntUI].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_UIBUBBLE;
		pVtx[1].col = COLOR_UIBUBBLE;
		pVtx[2].col = COLOR_UIBUBBLE;
		pVtx[3].col = COLOR_UIBUBBLE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// 頂点バッファをアンロック
	g_pVtxBuffExplantation->Unlock();
}

//======================================================================================
// Explantationの描画処理
//======================================================================================
void DrawExplantation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX UIMatrix, mtxRot, mtxView;	// UIのマトリックス情報を取得

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&UIMatrix);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングをオフにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		if (g_aExplantation[nCntUI].bDisp == false)
		{
			continue;
		}

		// ワールドマトリックスの設定

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX	mtxParent;					// 親のマトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aExplantation[nCntUI].mtxWorld);

		// ビューマトリックスを取得する
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aExplantation[nCntUI].mtxWorld, NULL, &mtxView);	//逆行列を求める

		g_aExplantation[nCntUI].mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		g_aExplantation[nCntUI].mtxWorld._42 = 0.0f;
		g_aExplantation[nCntUI].mtxWorld._43 = 0.0f;

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aExplantation[nCntUI].pos.x, g_aExplantation[nCntUI].pos.y, g_aExplantation[nCntUI].pos.z);
		D3DXMatrixMultiply(&g_aExplantation[nCntUI].mtxWorld, &g_aExplantation[nCntUI].mtxWorld, &mtxTransModel);
		
		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplantation[nCntUI].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffExplantation, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureExplantation[g_aExplantation[nCntUI].type]);

		// UIの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);

	}
	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングを元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法を設定(条件に関わらず描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
}

//======================================================================================
// Explantationの設置
//======================================================================================
void SetExplantation(EXPLANTATIONTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntUI = 0; nCntUI < MAX_EXPLANTATION; nCntUI++)
	{
		if (g_aExplantation[nCntUI].bDisp == false)
		{
			g_aExplantation[nCntUI].type = type;
			g_aExplantation[nCntUI].pos = pos;
			g_aExplantation[nCntUI].rot = rot;
			g_aExplantation[nCntUI].bDisp = true;
			break;
		}
	}
}

//======================================================================================
// Explantationを出現状態にする
//======================================================================================
void SetExplantationAppear(int nIdx)
{
	g_aExplantation[nIdx].state		= EXPLANTATIONSTATE_APPEAR;
	g_aExplantation[nIdx].fWidth		= WIDTH;		// 幅
	g_aExplantation[nIdx].fWidthDest	= WIDTH;	// 幅の目的値
	g_aExplantation[nIdx].fHeightDest = HEIGHT;	// 高さの目的値
	g_aExplantation[nIdx].nKey		= 0;		// 現在のキー数
	g_aExplantation[nIdx].bDisp		= true;		// 表示状態
}

//======================================================================================
// Explantationを表示状態にする
//======================================================================================
void SetExplantationDisp(int nIdx)
{
	g_aExplantation[nIdx].state		= EXPLANTATIONSTATE_DISPLAY;
	g_aExplantation[nIdx].fWidth		= WIDTH;	// 幅
	g_aExplantation[nIdx].fWidthDest	= WIDTH;	// 幅の目的値
	g_aExplantation[nIdx].fHeight		= HEIGHT;	// 高さの目的値
	g_aExplantation[nIdx].fHeightDest = HEIGHT;	// 高さの目的値
	g_aExplantation[nIdx].nKey		= 0;		// 現在のキー数
	g_aExplantation[nIdx].bDisp		= true;		// 表示状態
}

//======================================================================================
// Explantationを収縮状態にする
//======================================================================================
void SetExplantationDisappear(int nIdx)
{
	g_aExplantation[nIdx].state		= EXPLANTATIONSTATE_DISAPPEAR;
	g_aExplantation[nIdx].fWidth		= WIDTH;	// 幅
	g_aExplantation[nIdx].fWidthDest	= WIDTH;	// 幅の目的値
	g_aExplantation[nIdx].fHeightDest = 0.0f;		// 高さの目的値
	g_aExplantation[nIdx].nKey		= 0;		// 現在のキー数
	g_aExplantation[nIdx].bDisp		= true;		// 表示状態
}

//======================================================================================
// Explantationを非表示状態にする
//======================================================================================
void SetExplantationNonDisp(int nIdx)
{
	g_aExplantation[nIdx].state = EXPLANTATIONSTATE_NONDISPLAY;
	g_aExplantation[nIdx].fWidth = WIDTH;			// 幅
	g_aExplantation[nIdx].fWidthDest = WIDTH;		// 幅の目的値
	g_aExplantation[nIdx].fHeight = 0.0f;			// 高さ
	g_aExplantation[nIdx].fHeightDest = HEIGHT;	// 高さの目的値
	g_aExplantation[nIdx].nKey = 0;				// 現在のキー数
	g_aExplantation[nIdx].bDisp = false;			// 表示状態
}
