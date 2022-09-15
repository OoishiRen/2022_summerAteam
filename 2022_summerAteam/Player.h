#pragma once

/*************************
** 定数 **
*************************/

#define PAC_IMAGE_MAX	12	//パックマン画像格納用
#define DYING_IMAGE_MAX	11	//ミス時の画像格納用
#define PAC_WIDTH	16.0f	//パックマンの画像幅
#define PAC_HEIGHT	16.0f	//パックマンの画像高さ
#define PAC_SPEED	4.0f	//パックマンの速さ
#define PAC_LIFE	3		//パックマンの残機

/*********************************
**     構造体     **
**********************************/

struct PAC {
	bool flg;		//ミスしたら:false
	bool dir;		//向き
	int var;		//事前入力用
	int type;		//状態遷移
	int img;		//画像識別用
	float x, y, w, h;	//座標
	float speed;		//移動量
	int cnt;		//残機
};

extern struct PAC mPac;		//パックマン構造体

static bool pac_x0 = false, pac_x1 = false, pac_y0 = false, pac_y1 = false;

void Player_Initialize();	//初期化
void Player_Finalize();		//終了処理
void Player_Update();		//更新
void Player_Draw();			//描画

void Pac_Anim(float val);	//パックマンのアニメーション
void Pac_Move(int Type);			//パックマンの移動