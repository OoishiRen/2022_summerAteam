#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"



static int mImageHandle; //画像ハンドル格納用変数


//初期化
void Menu_Initialize() {

	//mImageHandle = LoadGraph("images/Scene_Menu.png"); //画像のロード
	mImageHandle = LoadGraph("images/Title.png"); //画像のロード
}
//終了処理
void Menu_Finalize() {
	DeleteGraph(mImageHandle); //画像の解放
}
//更新
void Menu_Update() {
	if (CheckHitKey(KEY_INPUT_G) != 0) {//Escキーが押されていたらorBボタンが押されていたら
		SceneMgr_ChangeScene(eScene_Game);//シーンをメニューに変更
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {// ESCAPEキーが押されているかを調べる
			DxLib_End();
	}

	if ((g_NowKey & PAD_INPUT_7) != 0) {
		DxLib_End();
	}

	if (CheckHitKey(KEY_INPUT_C) != 0) {//Escキーが押されていたらorBボタンが押されていたら
		SceneMgr_ChangeScene(eScene_Config);//シーンをメニューに変更
	}
}
//描画
void Menu_Draw() {
	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 650, 0xffffff, "Gキーを押してゲーム画面へ");
	DrawFormatString(10, 700, 0xffffff, "Cキーを押してコンフィグ画面へ");
}