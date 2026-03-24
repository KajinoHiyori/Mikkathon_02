//==================================================================================
// 
// ステージに関する処理[stage.cpp]
// Author : Soki Inamura
// 
//==================================================================================
#include "main.h"
#include "stage.h"

#include "file.h"

bool g_bLoadStage = false;	// ステージを読み込んだ

//========================================================================
// ステージの初期化処理
//========================================================================
void InitStage()
{
	
}

//========================================================================
// ステージの終了処理
//========================================================================
void UninitStage()
{

}

//========================================================================
// ステージの更新処理
//========================================================================
void UpdateStage()
{

}

//========================================================================
// ステージの描画処理
//========================================================================
void DrawStage()
{

}

//========================================================================
// ステージの設定処理
//========================================================================
bool SetLoadStage(const char* pStageFilename)
{
	if (g_bLoadStage == true)
	{// すでに読み取り終えている

		return true;	// 読み取りに成功したことを返す
	}

	// 変数宣言 ======================================

	// 読み取り
	char aReadText[256]  = {};	// 読み取った文字
	char aBlankText[256] = {};	// 空の読み取り

	// モデル
	int nNumModel = 0;		// 読み込むモデル数
	int nCntModel = 0;		// 読み込んだモデル数

	// 惑星情報
	int nNumPlanetType = 0;	// 読み込む惑星の種類数
	int nCntPlanetType = 0;	// 読み込んだ惑星の種類数

	// ===============================================

	// ▼ファイルを開く
	FILE* pFile = fopen(&pStageFilename[0], "r");

	if (pFile == NULL)
	{// ファイルを開けなかった

		MessageBox(GetActiveWindow(), "ステージファイルの読み取りに失敗しました", "ErrorMassage", MB_OK);

		return false;	// 読み取りに失敗したことを返す
	}

	// 読み取り開始地点まで読み取る
	if (ReadStartTopFile(pFile) == false)
	{
		MessageBox(GetActiveWindow(), "ステージファイルの読み取りに失敗しました", "ErrorMassage", MB_OK);

		return false;	// 読み取りに失敗したことを返す
	}

	while (FileExtractText(pFile, &aReadText[0]))
	{
		if (strcmp(&aReadText[0], "END_SCRIPT"))
		{
			break;	// while文を抜ける
		}
	// -----------------------------------
	// モデル情報
	// -----------------------------------
		else if (strcmp(&aReadText[0], "NUM_MODEL"))
		{
			fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
			fscanf(pFile, "%d", &nNumModel);				//読み込むモデル数を読み取る

			continue;
		}
		else if (strcmp(&aReadText[0], "MODEL_FILENAME"))
		{
			if (nCntModel < nNumModel)
			{// 読み取れる上限に達していない場合
				


				nCntModel++;	// 読み込んだモデルをインクリメント
			}

			continue;
		}
	// -----------------------------------
	// 惑星の情報
	// -----------------------------------
		else if (strcmp(&aReadText[0], "NUM_PLANETTYPE"))
		{
			fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
			fscanf(pFile, "%d", &nNumPlanetType);			//読み込むモデル数を読み取る

			continue;
		}
		else if (strcmp(&aReadText[0], "SET_PLANETTYPE"))
		{
			if (nCntPlanetType < nNumPlanetType)
			{// 読み取れる上限に達していない場合

				while (FileExtractText(pFile, &aReadText[0]))
				{
					if (strcmp(&aReadText[0], "END_PLANETTYPE"))
					{
						nCntPlanetType++;	// 設定した惑星の種類をインクリメント

						break;	// while文を抜ける
					}
					else if (strcmp(&aReadText[0], "MODELIDX"))
					{


						continue;
					}
					else if (strcmp(&aReadText[0], "RADIUS"))
					{


						continue;
					}
				}
			}

			continue;
		}
	// -----------------------------------
	// ステージの配置情報
	// -----------------------------------
		else if (strcmp(&aReadText[0], "SET_LAYOUT_STAGE"))
		{
			while (FileExtractText(pFile, &aReadText[0]))
			{
				if (strcmp(&aReadText[0], "END_LAYOUT_STAGE"))
				{
					break;	// while文を抜ける
				}
			// -----------------------------------
			// 惑星情報
			// -----------------------------------
				else if (strcmp(&aReadText[0], "SET_PLANET"))
				{
					while (FileExtractText(pFile, &aReadText[0]))
					{
						if (strcmp(&aReadText[0], "END_PLANET"))
						{
							break;	// while文を抜ける
						}
						else if (strcmp(&aReadText[0], "TYPE"))
						{


							continue;
						}
						else if (strcmp(&aReadText[0], "POS"))
						{


							continue;
						}
					}

					continue;
				}
			}

			continue;
		}
	}

	g_bLoadStage = true;	// 読み取りおえた状態に設定

	// ▲ファイルを閉じる
	fclose(pFile);

	return true;		// 読み取りに成功したことを返す
}