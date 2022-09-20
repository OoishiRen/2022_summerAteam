#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"



static int mTitleHandle; //画像ハンドル格納用変数
int img_x = 338, img_y = 720;

//初期化
void Menu_Initialize() {
	//mImageHandle = LoadGraph("images/Scene_Menu.png"); //画像のロード

	mTitleHandle = LoadGraph("Item_images/PAC-MAN_logo.png"); //画像のロード


	img_x = 338, img_y = 720;//タイトルの初期位置
}
//終了処理
void Menu_Finalize() {
	DeleteGraph(mTitleHandle); //画像の解放
}
//更新
void Menu_Update() {

	//デバッグ用
	if (CheckHitKey(KEY_INPUT_S) != 0) {//Sキーが押されていたら
		SceneMgr_ChangeScene(eScene_Game);//シーンをメニューに変更
	}


	if (img_y <= 138) {

		img_y = 138;

		if (CheckHitKey(KEY_INPUT_G) != 0) {//Gキーが押されていたら
			SceneMgr_ChangeScene(eScene_Game);//シーンをメニューに変更
		}
		if ((g_NowKey & PAD_INPUT_1) != 0 || (g_NowKey & PAD_INPUT_8) != 0) {//Aボタン、またはSTARTボタンが押されていたら
			SceneMgr_ChangeScene(eScene_Game);//シーンをメニューに変更
		}

		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0 || (g_NowKey & PAD_INPUT_7) != 0) {// ESCAPEキーが押されているかを調べる
			DxLib_End();
		}
	}
	else {
		img_y -= TITLE_SPD;
	}
}
//描画
void Menu_Draw() {
	DrawGraph(img_x, img_y, mTitleHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "Gキー、またはAボタンを押してゲーム画面へ");
	DrawFormatString(10, 680, 0xffffff, "Sキーを押してタイトルをスキップ(ゲーム画面へ)");
}