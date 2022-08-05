#include "DxLib.h"
#include "Config.h"
#include "Game.h"
#include "Menu.h"
#include "SceneMgr.h"


static eScene mScene = eScene_Menu; //シーン管理変数
static eScene mNextScene = eScene_None; //次のシーン管理変数
static void SceneMgr_InitializeModule(eScene scene);//指定モジュールを初期化する
static void SceneMgr_FinalizeModule(eScene scene);//指定モジュールの終了処理を行う


//初期化
void SceneMgr_Initialize() {
	SceneMgr_InitializeModule(mScene);
}


//終了処理
void SceneMgr_Finalize() {
	SceneMgr_FinalizeModule(mScene);
}

//更新
void SceneMgr_Update() {
	if (mNextScene != eScene_None) { //次のシーンがセットされていたら
		SceneMgr_FinalizeModule(mScene);//現在のシーンの終了処理を実行
		mScene = mNextScene; //次のシーンを現在のシーンセット
		mNextScene = eScene_None; //次のシーン情報をクリア
		SceneMgr_InitializeModule(mScene); //現在のシーンを初期化
	}
	switch (mScene) { //シーンによって処理を分岐
	case eScene_Menu: //現在の画面がメニュー(タイトル)なら
		Menu_Update(); //メニュー画面の更新処理をする
		break;//以下略
	case eScene_Game:
		Game_Update();
		break;
	case eScene_Config:
		Config_Update();
		break;
	}
}
//描画
void SceneMgr_Draw() {
	switch (mScene) { //シーンによって処理を分岐
	case eScene_Menu: //現在の画面がメニュー画面なら
		Menu_Draw(); //メニュー画面の描画処理をする
		break;//以下略
	case eScene_Game:
		Game_Draw();
		break;
	case eScene_Config:
		Config_Draw();
		break;
	}
}


// 引数 nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene NextScene) {
	mNextScene = NextScene; //次のシーンをセットする
}

// 引数sceneモジュールを初期化する
static void SceneMgr_InitializeModule(eScene scene) {
	switch
		(scene) { //シーンによって処理分岐
	case eScene_Menu: //指定画面がメニュー画面なら
		Menu_Initialize(); //メニュー画面の初期化処理をする
		break;//以下略
	case eScene_Game:
		Game_Initialize();
		break;
	case eScene_Config:
		Config_Initialize();
		break;
	}
}
// 引数sceneモジュールの終了処理を行う
static void SceneMgr_FinalizeModule(eScene scene) {
	switch
		(scene) { //シーンによって処理を分岐
	case eScene_Menu: //指定画面がメニュー画面なら
		Menu_Finalize(); //メニュー画面の終了処理処理をする
		break;//以下略
	case eScene_Game:
		Game_Finalize();
		break;
	case eScene_Config:
		Config_Finalize();
		break;
	}
}