#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"



static int mTitleHandle; //画像ハンドル格納用変数
static int score_image[15];
int img_x = 338, img_y = 720;
int mozi_x = 590, mozi_y = 600;
int mozi1_x = 615, mozi1_y = 600;
int mozi2_x = 640, mozi2_y = 600;
int mozi3_x = 665, mozi3_y = 600;
int mozi4_x = 690, mozi4_y = 600;
int num14 = 1;
int num15 = 0;
int num16 = 0;
int num17 = 0;
int num18 = 0;



//初期化
void Menu_Initialize() {
	//mImageHandle = LoadGraph("images/Scene_Menu.png"); //画像のロード

	mTitleHandle = LoadGraph("Item_images/PAC-MAN_logo.png"); //画像のロード
	//mTitleHandle = LoadGraph("UI_images/num.png"); //画像のロード
	LoadDivGraph("UI_images/num.png", 15, 15, 1, 16, 16, score_image);



	img_x = 338, img_y = 720;//タイトルの初期位置
	mozi_x = 590, mozi_y = 600;
	 mozi1_x = 615, mozi1_y = 600;
	 mozi2_x = 640, mozi2_y = 600;
	 mozi3_x = 665, mozi3_y = 600;
	 mozi4_x = 690, mozi4_y = 600;

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

	if (mozi_y <= 50) {

		mozi_y = 50;
	}
	else {
		mozi_y -= TITLE_SPD;
	}
	if (mozi1_y <= 50) {

		mozi1_y = 50;
	}
	else {
		mozi1_y -= TITLE_SPD;
	}
	if (mozi2_y <= 50) {

		mozi2_y = 50;
	}
	else {
		mozi2_y -= TITLE_SPD;
	}
	if (mozi3_y <= 50) {

		mozi3_y = 50;
	}
	else {
		mozi3_y -= TITLE_SPD;
	}
	if (mozi4_y <= 50) {

		mozi4_y = 50;
	}

	else {
		mozi4_y -= TITLE_SPD;
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
	DrawRotaGraph(mozi_x, mozi_y, 2.0f, 0, score_image[num14], TRUE, FALSE);
	DrawRotaGraph(mozi1_x, mozi1_y, 2.0f, 0, score_image[num15], TRUE, FALSE);
	DrawRotaGraph(mozi2_x, mozi2_y, 2.0f, 0, score_image[num16], TRUE, FALSE);
	DrawRotaGraph(mozi3_x, mozi3_y, 2.0f, 0, score_image[num17], TRUE, FALSE);
	DrawRotaGraph(mozi4_x, mozi4_y, 2.0f, 0, score_image[num18], TRUE, FALSE);
}