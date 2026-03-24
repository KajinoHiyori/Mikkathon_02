//========================================================
//
// ケプラーMk3 [input.h]
// Author : Kajino Hiyori
// 
//========================================================
#ifndef _INPUT_H_
#define	_INPUT_H_
#include "main.h"

// マクロ定義
#define JOYSTICKVALUE_MAX	(32767)				// スティックの値の最大
#define JOYSTICKVALUE_MIN	(-32768)			// スティックの値の最小

// ジョイパッドのキーの種類
typedef enum
{
	JOYKEY_UP = 0,	// [0]十字キー(上)
	JOYKEY_DOWN,	// [1]十字キー(下)
	JOYKEY_LEFT,	// [2]十字キー(左)
	JOYKEY_RIGHT,	// [3]十字キー(右)
	JOYKEY_START,	// [4]スタートボタン
	JOYKEY_BACK,	// [5]バックボタン
	JOYKEY_STICKLEFT,	// [6]スティック押し込み(左)
	JOYKEY_STICKRIGHT,	// [7]スティック押し込み(右)
	JOYKEY_LB,			// [8]LBボタン
	JOYKEY_RB,			// [9]RBボタン
	JOYKEY_A = 12,		// [12]Aボタン
	JOYKEY_B,			// [13]Bボタン
	JOYKEY_X,			// [14]Xボタン
	JOYKEY_Y,			// [15]Yボタン
	JOYKEY_LSTICK_UP = 20,	// [20]左スティックを上に倒す
	JOYKEY_LSTICK_DOWN,		// [21]左スティックを下に倒す
	JOYKEY_LSTICK_LEFT,		// [22]左スティックを左に倒す
	JOYKEY_LSTICK_RIGHT,	// [23]左スティックを右に倒す
	JOYKEY_RSTICK_UP,		// [24]右スティックを上に倒す
	JOYKEY_RSTICK_DOWN,		// [25]右スティックを下に倒す
	JOYKEY_RSTICK_LEFT,		// [26]右スティックを左に倒す
	JOYKEY_RSTICK_RIGHT,	// [27]右スティックを右に倒す
	JOYKEY_MAX
}JOYKEY;

// プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);	// キーボード[Press]
bool GetKeyboardTrigger(int nKey);	// キーボード[Trigger]
bool GetKeyboardRelease(int nKey);	// キーボード[Release]
bool GetKeyboardRepeat(int nKey);	// キーボード[Repeat]
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);	// ジョイパッド[Press]
bool GetJoypadTrigger(JOYKEY key);	// ジョイパッド[Trigger]
bool GetJoypadRelease(JOYKEY key);	// ジョイパッド[Release]
bool GetJoypadRepeat(JOYKEY key);	// ジョイパッド[Repeat]
bool GetJoypadLeftStick(JOYKEY key);	// Lstick[Press]
bool GetJoypadRightStick(JOYKEY key);	// Rstick[Press]
bool GetJoypadLeftStickValue(int *pValueH, int *pValueV);	// Lstick[0.0f ~ 1.0fの範囲で入力値取得]
bool GetJoypadLeftStickTrigger(JOYKEY key);		// Lstick[Trigger]
bool GetJoypadRightStickTrigger(JOYKEY key);	// Rstick[Trigger]
bool GetJoypadLeftStickRelease(JOYKEY key);		// Lstick[Release]
void SetVibration(int left, int right, int time);
void StopVibration(void);
XINPUT_STATE GetJoypadState(void);

#endif
