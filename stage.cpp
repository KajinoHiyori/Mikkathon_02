//==================================================================================
// 
// ステージに関する処理[stage.cpp]
// Author : Soki Inamura
// 
//==================================================================================
#include "main.h"
#include "stage.h"

#include "planet.h"

#include "file.h"

// マクロ定義 ==================================================

#define MAX_NUM_STAGEMODEL		(128)	// ステージのモデル数の最大

// グローバル宣言 ==============================================

StageModelInfo g_aStageModelInfo[MAX_NUM_STAGEMODEL];		// ステージモデルの情報

bool g_bLoadStage = false;	// ステージを読み込んだか

//========================================================================
// ステージの初期化処理
//========================================================================
void InitStage(void)
{
	if (g_bLoadStage == false)
	{// まだステージを読み込んでない

		// ステージモデルの情報を初期化
		for (int nCntStageModel = 0; nCntStageModel < MAX_NUM_STAGEMODEL; nCntStageModel++)
		{
			memset(&g_aStageModelInfo[nCntStageModel].apTexture[0], NULL, sizeof g_aStageModelInfo[nCntStageModel].apTexture);	// マテリアルテクスチャを初期化
			g_aStageModelInfo[nCntStageModel].pMesh = NULL;																		// メッシュの中身を空にする
			g_aStageModelInfo[nCntStageModel].pBuffMat = NULL;																	// マテリアルの中身を空にする
			g_aStageModelInfo[nCntStageModel].dwNumMat = 0;																		// マテリアル数を初期化

			g_aStageModelInfo[nCntStageModel].bUse = false;																		// 使用していない状態に設定
		}

		// ステージの読み込み
		SetLoadStage("data/FILE/StageSet.txt");
	}
}

//========================================================================
// ステージの終了処理
//========================================================================
void UninitStage(void)
{
	for (int nCntStageModel = 0; nCntStageModel < MAX_NUM_STAGEMODEL; nCntStageModel++)
	{
		// テクスチャの破棄
		for (int nCntTexture = 0; nCntTexture < MAX_TEX_STAGEMODEL; nCntTexture++)
		{// テクスチャの数分繰り返す

			if (g_aStageModelInfo[nCntStageModel].apTexture[nCntTexture] != NULL)
			{// 中身がある

				g_aStageModelInfo[nCntStageModel].apTexture[nCntTexture]->Release();
				g_aStageModelInfo[nCntStageModel].apTexture[nCntTexture] = NULL;
			}
		}

		// メッシュの破棄
		if (g_aStageModelInfo[nCntStageModel].pMesh != NULL)
		{// 中身がある

			g_aStageModelInfo[nCntStageModel].pMesh->Release();
			g_aStageModelInfo[nCntStageModel].pMesh = NULL;
		}

		// マテリアルの破棄
		if (g_aStageModelInfo[nCntStageModel].pBuffMat != NULL)
		{// 中身がある

			g_aStageModelInfo[nCntStageModel].pBuffMat->Release();
			g_aStageModelInfo[nCntStageModel].pBuffMat = NULL;
		}
	}
}

//========================================================================
// ステージの更新処理
//========================================================================
void UpdateStage(void)
{

}

//========================================================================
// ステージの描画処理
//========================================================================
void DrawStage(void)
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

	// 設定情報
	PlanetModelInfo tmpPlanetModelInfo;
	Planet tmpPlanet;

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
		if (strcmp(&aReadText[0], "END_SCRIPT") == false)
		{
			break;	// while文を抜ける
		}
	// -----------------------------------
	// モデル情報
	// -----------------------------------
		else if (strcmp(&aReadText[0], "NUM_MODEL") == false)
		{
			fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
			fscanf(pFile, "%d", &nNumModel);				//読み込むモデル数を読み取る

			continue;
		}
		else if (strcmp(&aReadText[0], "MODEL_FILENAME") == false)
		{
			if (nCntModel < nNumModel)
			{// 読み取れる上限に達していない場合
				
				fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
				fscanf(pFile, "%s", &aReadText[0]);				//読み込むモデルファイルを読み取る

				// ステージモデルの読み込み
				SetLoadStageModelInfo(&aReadText[0]);

				nCntModel++;	// 読み込んだモデルをインクリメント
			}

			continue;
		}
	// -----------------------------------
	// 惑星の情報
	// -----------------------------------
		else if (strcmp(&aReadText[0], "NUM_PLANETTYPE") == false)
		{
			fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
			fscanf(pFile, "%d", &nNumPlanetType);			//読み込むモデル数を読み取る

			continue;
		}
		else if (strcmp(&aReadText[0], "SET_PLANETTYPE") == false)
		{
			if (nCntPlanetType < nNumPlanetType)
			{// 読み取れる上限に達していない場合

				// 設定情報の初期化
				tmpPlanetModelInfo.nIdxStageModel = -1;
				tmpPlanetModelInfo.fHitRadius = 0.0f;
				tmpPlanetModelInfo.fGravity = 0.0f;
				tmpPlanetModelInfo.fRadius = 0.0f;
				tmpPlanetModelInfo.addRot = D3DXVECTOR3(0.0f,0.0f,0.0f);

				while (FileExtractText(pFile, &aReadText[0]))
				{
					if (strcmp(&aReadText[0], "END_PLANETTYPE") == false)
					{
						// 惑星の情報の設定
						SetLoadPlanetInfo(tmpPlanetModelInfo.nIdxStageModel, tmpPlanetModelInfo.fHitRadius, tmpPlanetModelInfo.fGravity, tmpPlanetModelInfo.fRadius, tmpPlanetModelInfo.addRot);

						nCntPlanetType++;	// 設定した惑星の種類をインクリメント

						break;	// while文を抜ける
					}
					else if (strcmp(&aReadText[0], "MODELIDX") == false)
					{
						fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);					// 余分な情報を読み取る
						fscanf(pFile, "%d", &tmpPlanetModelInfo.nIdxStageModel);	// モデルインデックスを読み取る

						continue;
					}
					else if (strcmp(&aReadText[0], "HITRADIUS") == false)
					{
						fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);			// 余分な情報を読み取る
						fscanf(pFile, "%f", &tmpPlanetModelInfo.fHitRadius);	// モデルインデックスを読み取る

						continue;
					}
					else if (strcmp(&aReadText[0], "RADIUS") == false)
					{
						fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);			// 余分な情報を読み取る
						fscanf(pFile, "%f", &tmpPlanetModelInfo.fRadius);	// モデルインデックスを読み取る

						continue;
					}
					else if (strcmp(&aReadText[0], "GRAVITY") == false)
					{
						fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);			// 余分な情報を読み取る
						fscanf(pFile, "%f", &tmpPlanetModelInfo.fGravity);	// モデルインデックスを読み取る

						continue;
					}
					else if (strcmp(&aReadText[0], "ROTATION") == false)
					{
						fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);			// 余分な情報を読み取る
						fscanf(pFile, "%f %f %f", &tmpPlanetModelInfo.addRot.x,
												  &tmpPlanetModelInfo.addRot.y,
												  &tmpPlanetModelInfo.addRot.z);	// モデルインデックスを読み取る

						continue;
					}
				}
			}

			continue;
		}
	// -----------------------------------
	// ステージの配置情報
	// -----------------------------------
		else if (strcmp(&aReadText[0], "SET_LAYOUT_STAGE") == false)
		{
			while (FileExtractText(pFile, &aReadText[0]))
			{
				if (strcmp(&aReadText[0], "END_LAYOUT_STAGE") == false)
				{
					break;	// while文を抜ける
				}
			// -----------------------------------
			// 惑星情報
			// -----------------------------------
				else if (strcmp(&aReadText[0], "SET_PLANET") == false)
				{
					// 設定情報の初期化
					tmpPlanet.type = PLANETTYPE_MAX;
					tmpPlanet.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					tmpPlanet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					while (FileExtractText(pFile, &aReadText[0]))
					{
						if (strcmp(&aReadText[0], "END_PLANET") == false)
						{
							// 惑星の設定
							SetPlanet(tmpPlanet.type, tmpPlanet.pos, tmpPlanet.rot);

							break;	// while文を抜ける
						}
						else if (strcmp(&aReadText[0], "TYPE") == false)
						{
							fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
							fscanf(pFile, "%d", &tmpPlanet.type);			// 種類を読み取る

							continue;
						}
						else if (strcmp(&aReadText[0], "POS") == false)
						{
							fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
							fscanf(pFile, "%f %f %f", &tmpPlanet.pos.x, 
													  &tmpPlanet.pos.y,
													  &tmpPlanet.pos.z);			// 位置を読み取る

							continue;
						}
						else if (strcmp(&aReadText[0], "ROT") == false)
						{
							fscanf(pFile, "%[ =\t\n]", &aBlankText[0]);		// 余分な情報を読み取る
							fscanf(pFile, "%f %f %f", &tmpPlanet.rot.x, 
													  &tmpPlanet.rot.y,
													  &tmpPlanet.rot.z);			// 角度を読み取る

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

//========================================================================
// ステージのモデルの読み込み処理
//========================================================================
int SetLoadStageModelInfo(const char* pModelFilename)
{
	// 変数宣言 ======================================

	// デバイスの獲得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;									// 出力結果

	char aErrorText[256] = {};					// エラーテキスト

	// ===============================================

	// 惑星モデルの読み込み
	for (int nCntModel = 0; nCntModel < MAX_NUM_STAGEMODEL; nCntModel++)
	{
		if (g_aStageModelInfo[nCntModel].bUse == true)
		{// 使用している場合

			continue;
		}

		// Xファイルの読み込み
		hr = D3DXLoadMeshFromX(pModelFilename,
							   D3DXMESH_SYSTEMMEM,
							   pDevice,
							   NULL,
							   &g_aStageModelInfo[nCntModel].pBuffMat,
							   NULL,
							   &g_aStageModelInfo[nCntModel].dwNumMat,
							   &g_aStageModelInfo[nCntModel].pMesh);

		if (FAILED(hr))
		{// Xファイルの読み込みに失敗した場合

			sprintf(&aErrorText[0], "惑星の.xファイルの読み込みに失敗しました\nFILENAME %s\nHRESULT  0x%x", pModelFilename, hr);

			// 警告ウィンドウの作成
			MessageBox(NULL, &aErrorText[0], "ErrerMessage", MB_OK);
		}
		else
		{// Xファイルの読み込みに成功した場合

			// マテリアルデータへのポインタを所得
			D3DXMATERIAL* pMat = (D3DXMATERIAL*)g_aStageModelInfo[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aStageModelInfo[nCntModel].dwNumMat; nCntMat++)
			{// マテリアルの数分繰り返す

				if (pMat[nCntMat].pTextureFilename != NULL)
				{// テクスチャファイルがある

					// テクスチャの読み込み
					hr = D3DXCreateTextureFromFile(pDevice,												// Direct3Dデバイスへのポインタ
												   pMat[nCntMat].pTextureFilename,						// 読み込むテクスチャ
												   &g_aStageModelInfo[nCntModel].apTexture[nCntMat]);	// テクスチャへのポインタ

					if (FAILED(hr))
					{// 画像ファイルの読み込みに失敗した場合

						sprintf(&aErrorText[0], "惑星の画像ファイルの読み込みに失敗しました\nFILENAME %s\nHRESULT  0x%x", pMat[nCntMat].pTextureFilename, hr);

						// 警告ウィンドウの作成
						MessageBox(NULL, &aErrorText[0], "ErrerMessage", MB_OK);
					}
				}
			}

			g_aStageModelInfo[nCntModel].bUse = true;	// 使用している状態に設定

			return nCntModel;	// 設定した場所のインデックスを返す
		}
	}

	return -1;
}

//========================================================================
// ステージの読み込みが終わっているか返す処理
//========================================================================
bool GetLoadStage(void)
{
	return g_bLoadStage;
}

//========================================================================
// ステージモデルの情報返す処理
//========================================================================
StageModelInfo* GetStageModelInfo(int nIdx)
{
	return &g_aStageModelInfo[nIdx];
}