#include "DxLib.h"
#include "SceneMgr.h"
#include"Input.h"

static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;	//平均を取るサンプル数
static const int FPS = 60;	//設定したFPS

bool Update() {
	if (mCount == 0) { //1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60フレーム目なら平均を計算する
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw() {
	DrawFormatString(0, 400, GetColor(255, 255, 255), "%.1f", mFps);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		Sleep(waitTime);	//待機
	}
}


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

		Update();	//更新

		ClearDrawScreen();
		
		InputKey();
		SceneMgr_Update(); //更新
		SceneMgr_Draw(); //描画

		Draw();		//描画

		if (dNextTime + 16.66 > GetNowCount()) {
			WaitTimer((int)dNextTime - GetNowCount());
		}
		ScreenFlip();
		Wait();
	}
	SceneMgr_Finalize();
	DxLib_End(); // DXライブラリ終了処理
	return 0;
}