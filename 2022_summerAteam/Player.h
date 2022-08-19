#pragma once

/*************************
** 定数 **
*************************/

#define PAC_IMAGE_MAX	12	//パックマン画像格納用

/*********************************
**     構造体     **
**********************************/

struct PAC {
	bool flg;		//ミスしたら:false
	bool dir;		//向き
	int type;		//状態遷移
	int img;		//画像識別用
	float x, y, w, h;	//座標
	float speed;		//移動量
	int cnt;		//タイム
};

extern struct PAC mPac;		//パックマン構造体

void Player_Initialize();	//初期化
void Player_Finalize();		//終了処理
void Player_Update();		//更新
void Player_Draw();			//描画

void Pac_Anim(float val);	//パックマンのアニメーション
void Pac_Move(int Type);			//パックマンの移動