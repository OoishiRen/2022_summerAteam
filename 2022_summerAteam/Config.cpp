#include "Config.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"

static int mImageHandle; //画像ハンドル格納用変数h
//初期化
void Config_Initialize() {
	//mImageHandle = LoadGraph("images/Help.png");//画像のロード
}
//終了処理
void Config_Finalize() {
	//DeleteGraph(mImageHandle);//画像の解放
}
//更新
void Config_Update() {
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Escキーが押されていたらorBボタンが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
	}
}
//描画
void Config_Draw() {
	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESCキーを押してメニュー画面へ");
}