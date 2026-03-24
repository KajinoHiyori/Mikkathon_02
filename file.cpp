//==================================================================================
// 
// ファイルに関する処理[file.cpp]
// Author : Soki Inamura 
// 
//==================================================================================
#include "main.h"	// メインヘッダー

// 外部データ
#include "file.h"	// ファイルヘッダー

// マクロ定義 ==================================================

// ログ
#if 0
#define USE_LOGPASS	// ログパスを記録するか 
#endif
#if 0
#define USE_LOGTEXT	// ログテキストを記録するか 
#endif

#define MAX_NUMSAVELOG		(100000)						// 1ファイルに記録出来る回数

#define FILENAME_LOG_PASS	"data/FILE/Debug_Log/passlog"	// 通過ログのファイル名
#define FILENAME_LOG_TEXT	"data/FILE/Debug_Log/textlog"	// テキストログのファイル名

// グローバル宣言 ==============================================

// 通過ログ
char g_aPassLogFileName[256] = {};			// ログに記録するファイルの名前
int g_nFileNumberLogPass = 0;				// 何番目のファイルか
unsigned int g_unNumFileSaveLogPass = 0;	// 記録した回数
unsigned long g_ulTimeOld = 0;				// 前の時間

// テキストログ
int g_nFileNumberLogText = 0;				// 何番目のファイルか
unsigned int g_unNumFileSaveLogText = 0;	// 記録した回数

//========================================================================
// ファイルの更新処理
//========================================================================
void UpdateFile(void)
{
#ifdef _DEBUGPROC_H_	// DebugProcがある

#ifdef USE_LOGPASS	// 通過ログを使用する場合

	// 記録状況をデバッグ表示
	PrintDebugProc("left", "\nFILELOGPASS_NUM    %d", g_nFileNumberLogPass);	// 記録ファイル名(番号)
	PrintDebugProc("left", "\nFILELOGPASS_LOGCNT %d", g_unNumFileSaveLogPass);	// 記録回数
#endif

#ifdef USE_LOGTEXT	// テキストログを使用する場合

	// 記録状況をデバッグ表示
	PrintDebugProc("left", "\nFILELOGTEXT_NUM    %d", g_nFileNumberLogText);	// 記録ファイル名(番号)
	PrintDebugProc("left", "\nFILELOGTEXT_LOGCNT %d", g_unNumFileSaveLogText);	// 記録回数
#endif

#endif
}

//========================================================================
// 読み取りの開始位置までポインタを進める処理
//========================================================================
bool ReadStartTopFile(FILE* pFile)
{
	// 変数宣言 ===========================================

	char aReadText[128] = {};	// 読み取った文字

	// ====================================================

	do
	{// 読み取り開始位置まで読み取る

		// 文字の読み取り処理
		if (FileExtractText(pFile, &aReadText[0]) == false)
		{// 開始位置の読み取りに失敗した場合

			return false;	// 読み取りに失敗した事を返す
		}

	} while (strcmp(&aReadText[0], "SCRIPT"));	// ●SCRIPTまで

	return true;			// 読み取りに成功した事を返す
}

//========================================================================
// ファイルから文字だけの読み取り処理
//========================================================================
bool FileExtractText(FILE* pFile, char* pReadText)
{// <処理> 空欄、コメントを省いた文字を読み取る

	// 変数宣言 ===========================================

	char aReadText[256] = {};	// 読み取った文字
	char aBlank[256] = {};		// 空の読み取り

	int nCntPass = 0;			// 処理の通過(繰り返し)回数

	// ====================================================

	while (1)
	{
		// 文字の読み取り
		fscanf(pFile, "%[^ \n\t]", &aReadText[0]);	// 文字でなくなる所まで読み取る

		if (aReadText[0] == '#')
		{// 最初の文字が「#」の場合

			fscanf(pFile, "%[^\n]", &aBlank[0]);	// 改行まで読み取る
		}
		else
		{// 最初の文字が「#」ではない

			if (aReadText[0] != '\0')
			{// 読み取った文字の一文字目が「\0」ではない場合

				// 読み取った文字を入れる
				strcpy(&pReadText[0], &aReadText[0]);

				return true;		// 文字を読み取れた事を返す
			}
		}

		// 空欄の読み取り
		fscanf(pFile, "%[ \t\n]", &aBlank[0]);		// 空欄でなくなる所まで読み取る

		nCntPass++;	// 通過回数をカウント

		if (nCntPass >= 50)
		{// 正常に読み取れていなさそうな場合

			break;	// while文を抜ける
		}
	}

	return false;	// 「読み取りに問題がある」事を返す
}

//========================================================================
// 通過ログの書き込み処理
//========================================================================
void FileLogPass(const char* pPassPointName)
{
#ifdef USE_LOGPASS		// 記録するかしないか(0:する, 1:しない)
	
	// 変数宣言 ===========================================

	FILE* pFile;									// ファイルポインタ

	unsigned long ulCurrentTime = timeGetTime();	// 現在の時刻

	// ====================================================


	// 記録上限
	if (g_unNumFileSaveLogPass == 0)
	{// 記録出来る回数が0になった

		g_nFileNumberLogPass++;	// 書き込むファイル番号を増やす

		// ファイル番号を開くファイルにつなげる
		sprintf(&g_aPassLogFileName[0], "%s_%d.txt", FILENAME_LOG_PASS, g_nFileNumberLogPass);

		g_unNumFileSaveLogPass = MAX_NUMSAVELOG;	// 書き込める回数を設定
	}

	// ▼ファイルを開く
	pFile = fopen(g_aPassLogFileName, "a");	// 追記モード

	if (pFile == NULL)
	{// ファイルが開けなかった場合

		return;
	}

	// 情報を書き込む
	fprintf(pFile, "[%10s] %d\n", &pPassPointName[0], ulCurrentTime - g_ulTimeOld);

	g_unNumFileSaveLogPass--;		// 書き込める回数を減らす

	// ▲ファイルを閉じる
	fclose(pFile);

	// 前の時刻を現在の時刻に設定
	g_ulTimeOld = ulCurrentTime;

#endif
}

//========================================================================
// テキストを書き込む処理
//========================================================================
void FileLogText(const char* pDrawText)
{
#ifdef USE_LOGTEXT	// 記録するかしないか(0:する, 1:しない)
	
	// 変数宣言 ===========================================

	FILE* pFile;					// ファイルポインタ

	char aLogFileName[128] = {};	// ログファイル名

	// ====================================================


	// 記録上限
	if (g_unNumFileSaveLogText == 0)
	{// 記録出来る回数が0になった

		g_nFileNumberLogText++;	// 書き込むファイル番号を増やす

		g_unNumFileSaveLogText = MAX_NUMSAVELOG;	// 書き込める回数を設定
	}

	// ファイル番号を開くファイルにつなげる
	sprintf(&aLogFileName[0], "%s_%d.txt", FILENAME_LOG_TEXT, g_nFileNumberLogText);

	// ▼ファイルを開く
	pFile = fopen(&aLogFileName[0], "a");	// 追記モード

	if (pFile == NULL)
	{// ファイルを開け無かった場合

		return;
	}

	// 文字を書き込む
	fprintf(pFile, "%s", &pDrawText[0]);

	g_unNumFileSaveLogText--;	// 書き込める回数を減らす

	// ▲ファイルを閉じる
	fclose(pFile);

#endif
}