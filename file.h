//========================================================================
// 
// ファイルに関する処理[file.h]
// Author : Soki Inamura 
// 
//========================================================================

#ifndef _FILE_H_
#define _FILE_H_

// プロトタイプ宣言 ============================================

void UpdateFile(void);

bool FileExtractText(FILE* pFile, char* pReadText);		// ファイルから文字だけを読み取る処理

bool ReadStartTopFile(FILE* pFile);						// 読み取りの開始位置までポインタを進める処理

void FileLogPass										// 通過した場所とかかった時間を記録する処理
(const char* pPassPointName);

void FileLogText										// テキストを書き込む処理
(const char* pDrawText);

#endif
