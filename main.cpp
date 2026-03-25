//======================================================================================
// 
// ケプラーMk3 [mian.cpp]
// Author : Kajino Hiyori
// 
//======================================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"
#include "color.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "light.h"

// 全体で初期化を行う処理
#include "player.h"
#include "camera.h"
#include "oil.h"
#include "stage.h"
#include "planet.h"
#include "asteroid.h"
#include "effect_3d.h"
#include "particle_3d.h"
#include "explosion.h"

// マクロ定義
#define CLASS_NAME	"WindowClass"	// ウィンドウクラスの名前
#define WINDOW_NAME	"ケプラーMk3"	// ウィンドウの名前(キャプションに表示)
#define FULL_SCREEN	(TRUE)			// フルスクリーン状態の管理[TRUE : 通常 / FALSE : フルスクリーン]

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

// グローバル変数
LPDIRECT3D9	 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
int g_nCountFPS = RESET_DATA;			// 最後にFPSを計測した時間
MODE g_mode = MODE_TITLE;	// 現在のモード
bool g_bFrame = false;	// フレームの表示切替
bool g_bField = true;	// フィールドの表示切替
bool g_bWall = true;	// 壁の表示切替

// デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//======================================================================================
// メイン関数
//======================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // 画面サイズの構造体

	// ↓ ウィンドウクラスの構造体
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 0にする(通常は使用しない)
		0,									// 0にする(通常は使用しない)
		hInstance,							// インスタントハンドル
		LoadIcon(NULL, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// クライアント領域の背景色を白にする
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION),	// ファイルのアイコン
	};

	HWND hWnd;	// ウィンドウハンドル(識別子)
	MSG msg;	// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時間

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに指定
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);	

	// ウィンドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウの左上X座標
		CW_USEDEFAULT,				// ウィンドウの左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウのID
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, FULL_SCREEN)))
	{ // 初期化処理に失敗した場合
		return -1;
	}

	// 分解能の設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;	// 初期化
	dwExecLastTime = timeGetTime();	// 現在時刻を取得して保存
	dwFrameCount = RESET_DATA;		// 初期化
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);			// クライアント領域を設定

	// メッセージループ
	// メッセージキューからメッセージを取得→メッセージを取得しなかった場合0を返す
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // Windowsの処理
			if (msg.message == WM_QUIT)
			{ // WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{ // DirectXの処理
			dwCurrentTime = timeGetTime(); // 現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// FPSを測定した時刻を保存
				dwFrameCount = RESET_DATA;		// フレームレートのクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻(現在時刻)を保存
				// 更新処理
				Update();
				// 描画処理
				Draw();
				dwFrameCount ++;
			}
		}
	}
	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}

//======================================================================================
// ウィンドウプロシージャ
//======================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY: // ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: // キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE: // ESCが押された場合
			nID = MessageBox(hWnd, "QUIT THE GAME?", WINDOW_NAME, MB_YESNO);
			if (nID == IDYES)
			{
				// ウィンドウを破棄する→WM_DESTROYメッセージを送る
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE: // ×ボタンを押された場合
		nID = MessageBox(hWnd, "QUIT THE GAME?", WINDOW_NAME,  MB_YESNO);
		if (nID == IDYES)
		{
			// ウィンドウを破棄する→WM_DESTROYメッセージを送る
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を返す
}

//======================================================================================
// 初期化処理
//======================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;	// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// DirectX3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファのとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	// アルファブレンドの設定1
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // アルファブレンドの設定2
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// アルファブレンドの設定3

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャの繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャの繰り返し設定
	
	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// テクスチャのアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// テクスチャのアルファブレンドの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// テクスチャのアルファブレンドの設定

	// デバッグ情報の初期化
	InitDebugProc();

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// サウンドの初期化処理
	//InitSound(hWnd);

	// カメラの初期化処理
	InitCamera();
	SetNumCamera(1);

	// プレイヤーの初期化処理
	InitPlayer();

	InitPlanet();


	// ライトの初期化処理
	InitLight();

	// 燃料の初期化処理
	InitOil();

	// 小惑星の初期化処理
	InitAsteroid();

	// エフェクトの初期化処理
	InitEffect3D();

	// パーティクルの初期化処理
	InitParticle3D();

	// 爆発の初期化処理
	InitExplosion();
	
	InitStage();

	// フェードの初期化
	InitFade(MODE_TITLE, COLOR_BLACK);

	// 乱数の種を設定
	srand((unsigned int)time(0));

	return S_OK;
}

//======================================================================================
// 終了処理
//======================================================================================
void Uninit(void)
{
	// カメラの終了処理
	UninitCamera();

	// 爆発の終了処理
	UninitExplosion();

	// パーティクルの終了処理
	UninitParticle3D();

	// エフェクトの終了処理
	UninitEffect3D();

	// 燃料の終了処理
	UninitOil();

	// 小惑星の終了処理
	UninitAsteroid();

	// プレイヤーの終了処理
	UninitPlayer();

	// ステージの終了
	UninitStage();

	// タイトル画面の終了処理
	UninitTitle();

	// ゲーム画面の終了処理
	UninitGame();

	// リザルト画面の終了処理
	UninitResult();

	// デバッグ情報の終了処理
	UninitDebugProc();

	// ライトの終了処理
	UninitLight();

	// BGMの停止
	//StopSound();
	
	// サウンドの終了処理
	//UninitSound();

	// キーボードの終了処理
	UninitKeyboard();

	// ジョイパッドの終了処理
	UninitJoypad();

	// フェードの終了処理
	UninitFade();

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		UINT uiCnt = g_pD3DDevice->Release();
		g_pD3DDevice = NULL;

		if (uiCnt != 0)
		{
			MessageBox(NULL, "未破棄のリソースがあります", "error", MB_OK);
		}
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//======================================================================================
// 更新処理
//======================================================================================
void Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();
	// ジョイパッドの更新処理
	UpdateJoypad();

	// カメラの更新処理
	UpdateCamera();

	// ライトの更新処理
	UpdateLight();

	PrintDebugProc("FPS : %d\n", g_nCountFPS);

	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UpdateTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		UpdateGame();

		break;

	case MODE_RESULT:	// リザルト画面
		UpdateResult();
		break;
	}
	// デバッグ情報の更新処理
	UpdateDebugProc();

	// フェードの更新処理
	UpdateFade();
}

//======================================================================================
// 描画処理
//======================================================================================
void Draw(void)
{
	// 画面のクリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(255, 255, 255, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{ // 描画が成功した場合

		// カメラの設定
		SetCamera(0);

		switch (g_mode)
		{
		case MODE_TITLE:	// タイトル画面
			DrawTitle();
			break;

		case MODE_GAME:		// ゲーム画面
			DrawGame();

			// 燃料の描画処理
			DrawOil();
			break;

		case MODE_RESULT:	// リザルト画面
			DrawResult();
			break;
		}
		// フェードの描画処理
		DrawFade();

		// デバッグ情報の描画処理
		DrawDebugProc();

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//======================================================================================
// モードの設定
//======================================================================================
void SetMode(MODE mode)
{
	// 現在のモードの終了処理
	switch (g_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UninitTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:	// リザルト画面
		UninitResult();
		break;
	}

	// 新しいモードの初期化処理
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面
		InitTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:	// リザルト画面
		InitResult();
		break;
	}
	g_mode = mode;	// 現在のモードを保存
}

//======================================================================================
// モードの取得
//======================================================================================
MODE GetMode(void)
{
	return g_mode;
}

//======================================================================================
// FPSの取得
//======================================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//======================================================================================
// 角度の正規化
//======================================================================================
float AngleNormalization(float fAngle)
{
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}
	return fAngle;
}