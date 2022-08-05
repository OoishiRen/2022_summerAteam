#include "DxLib.h"
#include "SceneMgr.h"
#include"Input.h"

//更新

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetMainWindowText("Pac-Man");		//タイトルを設定

	ChangeWindowMode(TRUE); //ウィンドウモード変更
	DxLib_Init(); //DXライブラリ初期化
	SetDrawScreen(DX_SCREEN_BACK); //裏画面設定
	SetGraphMode(1280, 720, 32);   // 画面モードの変更
	SceneMgr_Initialize(); 

	while (ProcessMessage() == 0 ) { //画面更新 &メッセージ処理& 画面消去

		double dNextTime = GetNowCount();

		ClearDrawScreen();
		
		InputKey();
		SceneMgr_Update(); //更新
		SceneMgr_Draw(); //描画
		if (dNextTime + 16.66 > GetNowCount()) {
			WaitTimer((int)dNextTime - GetNowCount());
		}
		ScreenFlip();
	}
	SceneMgr_Finalize();
	DxLib_End(); // DXライブラリ終了処理
	return 0;
}