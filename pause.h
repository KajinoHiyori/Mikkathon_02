//========================================================
// 
// 3Dアクションゲーム_MASTER KEY[pause.h]
// 
//========================================================
#ifndef _PAUSE_H_
#define	_PAUSE_H_

// ポーズメニュー
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	// ゲームに戻る
	PAUSE_MENU_RETRY,			// ゲームをやり直す
	PAUSE_MENU_QUIT,			// タイトル画面に戻る
	PAUSE_MENU_BG,				// ポーズメニューの背景
	PAUSE_MENU_ALPHA,			// ポーズ背景を暗くする
	PAUSE_MENU_MAX
}PAUSE_MENU;

// プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(int pauseMenu);

#endif
