//========================================================
// 
// ケプラーMk3 [distance.h]
// Author : Kajino Hiyori
// 
//========================================================
#include "distance.h"
#include "player.h"
#include "planet.h"
#include "color.h"

// テクスチャの種類
typedef enum
{
	DISTANCETEX_DISTANCE = 0,	// ケプラーMk3まで
	DISTANCETEX_LIGHTYEAR,		// 光年
	DISTANCETEX_NUMBER,			// 数字
	DISTANCETEX_MAX
}DISTANCETEX;

// ポリゴンの種類
typedef enum
{
	DISTANCETYPE_X0000 = 0,	// X0000
	DISTANCETYPE_0X000,		// 0X000
	DISTANCETYPE_00X00,		// 00X00
	DISTANCETYPE_000X0,		// 000X0
	DISTANCETYPE_0000X,		// 0000X
	DISTANCETYPE_DISTANCE,	// ケプラーMk3まで
	DISTANCETYPE_LIGHTYEAR,	// 光年
	DISTANCETYPE_MAX
}DISTANCETYPE;

// 距離の構造体
typedef struct
{
	DISTANCETEX tex;	// テクスチャの種類
	DISTANCETYPE type;	// ポリゴンの種類
	D3DXVECTOR3 pos;	// 位置
	float fWidth;	// 幅
	float fHeight;	// 高さ
	float fTexX;	// テクスチャ読み込み範囲[X方向小]
}Distance;

// マクロ定義
#define MAX_DISTANCE_TEX	(DISTANCETEX_MAX)	// テクスチャの最大数
#define NUM_PLACE			(5)	// 数字の桁数
#define MAX_DISP			(NUM_PLACE + 2)	// 表示ポリゴン数
#define NUM_WIDTH			(25.0f)	// 数字の幅
#define NUM_HEIGHT			(NUM_WIDTH * 2)	// 数字の高さ
#define DISTANCE_WIDTH		(250.0f)	// 距離の幅
#define DISTANCE_HEIGHT		(50.0f)		// 距離の高さ
#define LIGHTYEAR_WIDTH		(70.0f * 0.5f)		// 光年の幅
#define LIGHTYEAR_HEIGHT	(50.0f * 0.5f)		// 光年の高さ
#define POS__X0000			(D3DXVECTOR3(980.0f, 150.0f, 0.0f))								// 数字の表示位置[X0000]
#define POS__0X000			(D3DXVECTOR3(POS__X0000.x + NUM_WIDTH * 2, POS__X0000.y, 0.0f))	// 数字の表示位置[0X000]
#define POS__00X00			(D3DXVECTOR3(POS__0X000.x + NUM_WIDTH * 2, POS__X0000.y, 0.0f))	// 数字の表示位置[00X00]
#define POS__000X0			(D3DXVECTOR3(POS__00X00.x + NUM_WIDTH * 2, POS__X0000.y, 0.0f))	// 数字の表示位置[000X0]
#define POS__0000X			(D3DXVECTOR3(POS__000X0.x + NUM_WIDTH * 2, POS__X0000.y, 0.0f))	// 数字の表示位置[0000X]
#define POS__DISTANCE		(D3DXVECTOR3(1040.0f, 50.0f, 0.0f))	// 距離の位置
#define POS__LIGHTYEAR		(D3DXVECTOR3(1240.0f, 165.0f, 0.0f))	// 光年の位置

// テクスチャの読み込み
const char* c_apFilenameDistance[MAX_DISTANCE_TEX] =
{
	"data\\TEXTURE\\lightyear000.png",	// ケプラーMk3まで
	"data\\TEXTURE\\lightyear001.png",	// 光年
	"data\\TEXTURE\\number.png",		// 数字
};

// グローバル変数
Distance	g_aDistance[MAX_DISP];	// 距離構造体の設定
LPDIRECT3DTEXTURE9 g_apTextureDistance[MAX_DISTANCE_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDistance = NULL; // 頂点バッファへのポインタ

//=======================================================
// 距離の初期化処理
//=======================================================
void InitDistance(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < MAX_DISTANCE_TEX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameDistance[nCntTexture], &g_apTextureDistance[nCntTexture]);
	}

	// 燃料の情報の初期化
	for (int nCntDistance = 0; nCntDistance < MAX_DISP; nCntDistance++)
	{
		switch (nCntDistance)
		{
		case DISTANCETYPE_X0000:	// X0000
			g_aDistance[nCntDistance].tex = DISTANCETEX_NUMBER;
			g_aDistance[nCntDistance].type = DISTANCETYPE_X0000;
			g_aDistance[nCntDistance].pos = POS__X0000;
			g_aDistance[nCntDistance].fWidth = NUM_WIDTH;
			g_aDistance[nCntDistance].fHeight = NUM_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;

		case DISTANCETYPE_0X000:	// 0X000
			g_aDistance[nCntDistance].tex = DISTANCETEX_NUMBER;
			g_aDistance[nCntDistance].type = DISTANCETYPE_0X000;
			g_aDistance[nCntDistance].pos = POS__0X000;
			g_aDistance[nCntDistance].fWidth = NUM_WIDTH;
			g_aDistance[nCntDistance].fHeight = NUM_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;

		case DISTANCETYPE_00X00:	// 00X00
			g_aDistance[nCntDistance].tex = DISTANCETEX_NUMBER;
			g_aDistance[nCntDistance].type = DISTANCETYPE_00X00;
			g_aDistance[nCntDistance].pos = POS__00X00;
			g_aDistance[nCntDistance].fWidth = NUM_WIDTH;
			g_aDistance[nCntDistance].fHeight = NUM_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;

		case DISTANCETYPE_000X0:	// 000X0
			g_aDistance[nCntDistance].tex = DISTANCETEX_NUMBER;
			g_aDistance[nCntDistance].type = DISTANCETYPE_000X0;
			g_aDistance[nCntDistance].pos = POS__000X0;
			g_aDistance[nCntDistance].fWidth = NUM_WIDTH;
			g_aDistance[nCntDistance].fHeight = NUM_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;

		case DISTANCETYPE_0000X:	// 0000X
			g_aDistance[nCntDistance].tex = DISTANCETEX_NUMBER;
			g_aDistance[nCntDistance].type = DISTANCETYPE_0000X;
			g_aDistance[nCntDistance].pos = POS__0000X;
			g_aDistance[nCntDistance].fWidth = NUM_WIDTH;
			g_aDistance[nCntDistance].fHeight = NUM_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;

		case DISTANCETYPE_DISTANCE:	// ケプラーMk3まで
			g_aDistance[nCntDistance].tex = DISTANCETEX_DISTANCE;
			g_aDistance[nCntDistance].type = DISTANCETYPE_DISTANCE;
			g_aDistance[nCntDistance].pos = POS__DISTANCE;
			g_aDistance[nCntDistance].fWidth = DISTANCE_WIDTH;
			g_aDistance[nCntDistance].fHeight = DISTANCE_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;

		case DISTANCETYPE_LIGHTYEAR:	// 光年
			g_aDistance[nCntDistance].tex = DISTANCETEX_LIGHTYEAR;
			g_aDistance[nCntDistance].type = DISTANCETYPE_LIGHTYEAR;
			g_aDistance[nCntDistance].pos = POS__LIGHTYEAR;
			g_aDistance[nCntDistance].fWidth = LIGHTYEAR_WIDTH;
			g_aDistance[nCntDistance].fHeight = LIGHTYEAR_HEIGHT;
			g_aDistance[nCntDistance].fTexX = 0.0f;
			break;
		}
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DISP, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffDistance, NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDistance->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDistance = 0; nCntDistance < MAX_DISP; nCntDistance++, pVtx += 4)
	{
		// 頂点座標の設定
		
		pVtx[0].pos = D3DXVECTOR3(g_aDistance[nCntDistance].pos.x - g_aDistance[nCntDistance].fWidth, g_aDistance[nCntDistance].pos.y - g_aDistance[nCntDistance].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aDistance[nCntDistance].pos.x + g_aDistance[nCntDistance].fWidth, g_aDistance[nCntDistance].pos.y - g_aDistance[nCntDistance].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aDistance[nCntDistance].pos.x - g_aDistance[nCntDistance].fWidth, g_aDistance[nCntDistance].pos.y + g_aDistance[nCntDistance].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aDistance[nCntDistance].pos.x + g_aDistance[nCntDistance].fWidth, g_aDistance[nCntDistance].pos.y + g_aDistance[nCntDistance].fHeight, 0.0f);

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

		switch (g_aDistance[nCntDistance].tex)
		{
		case DISTANCETEX_NUMBER:	// 数字
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_aDistance[nCntDistance].fTexX,			0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aDistance[nCntDistance].fTexX + 0.1f,	0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aDistance[nCntDistance].fTexX,			1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aDistance[nCntDistance].fTexX + 0.1f,	1.0f);
			break;

		default:
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffDistance->Unlock();
}

//=======================================================
// 距離の終了処理
//=======================================================
void UninitDistance(void)
{
	// テクスチャの破棄
	for (int nCntDistance = 0; nCntDistance < MAX_DISTANCE_TEX; nCntDistance++)
	{
		if (g_apTextureDistance[nCntDistance] != NULL)
		{
			g_apTextureDistance[nCntDistance]->Release();
			g_apTextureDistance[nCntDistance] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffDistance != NULL)
	{
		g_pVtxBuffDistance->Release();
		g_pVtxBuffDistance = NULL;
	}
}

//=======================================================
// 距離の更新処理
//=======================================================
void UpdateDistance(void)
{

}

//=======================================================
// 距離の描画処理
//=======================================================
void DrawDistance(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntDistance = 0; nCntDistance < MAX_DISP; nCntDistance++)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffDistance, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureDistance[g_aDistance[nCntDistance].tex]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDistance * 4,	2);		// 描画するプリミティブ数
		
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}