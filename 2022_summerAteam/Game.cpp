#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"

static int mImageHandle; //画像ハンドル格納用変数

//初期化
void Game_Initialize() {
	Enemy_Initialize();
	//mImageHandle = LoadGraph("images/Scene_GameMain.png"); //画像のロード
}

//終了処理
void Game_Finalize() {
	Enemy_Finalize();
	//DeleteGraph(mImageHandle); //画像の解放
}

//更新
void Game_Update() {
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Escキーが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
	}
	Enemy_Update();

}

//描画
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESCキーを押してメニュー画面へ");
	Enemy_Draw();
}