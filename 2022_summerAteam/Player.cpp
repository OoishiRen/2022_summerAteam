#include "Player.h"
#include "SceneMgr.h"
#include "Input.h"
#include "DxLib.h"

/*************************
** 変数 **
*************************/
static int pac_image[PAC_IMAGE_MAX];

float count = 0;

/*************************
** 構造体 **
*************************/
struct PAC mPac;

//初期化
void Player_Initialize() {
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, pac_image);

	mPac.flg = true;
	mPac.type = 0;
	mPac.img = 3;
	mPac.x = 240.0f;
	mPac.y = 392.0f;
	mPac.w = 16.0f;
	mPac.h = 16.0f;
	mPac.speed = 4.0f;
	mPac.cnt = 0;

	count = 0;
}

//終了処理
void Player_Finalize() {
	for (int i = 0;i < PAC_IMAGE_MAX;i++) {
		DeleteGraph(pac_image[i]);
	}
}

//更新
void Player_Update() {

	if (g_NowKey & PAD_INPUT_UP && mPac.type != 0) {
		mPac.type = 0;
		mPac.img = (3 * mPac.type);
	}
	else if (g_NowKey & PAD_INPUT_RIGHT && mPac.type != 1) {
		mPac.type = 1;
		mPac.img = (3 * mPac.type);
	}
	else if (g_NowKey & PAD_INPUT_DOWN && mPac.type != 2) {
		mPac.type = 2;
		mPac.img = (3 * mPac.type);
	}
	else if (g_NowKey & PAD_INPUT_LEFT && mPac.type != 3) {
		mPac.type = 3;
		mPac.img = (3 * mPac.type);
	}
	Pac_Anim(mPac.speed);
	Pac_Move(mPac.type);

}

//描画
void Player_Draw() {
	DrawRotaGraph(mPac.x,mPac.y,1.0f,0,pac_image[mPac.img],TRUE,FALSE);
}

void Pac_Anim(float val) {
	count += 1.0f;
	if (count / val ==  0.75f) {
		count = 0;
		if (mPac.img <	(3*(mPac.type + 1)) - 1 ) {
			mPac.img++;
		}
		else {
			mPac.img = (3 * mPac.type);
		}
	}
}

void Pac_Move(int Type) {
	switch (Type)
	{
	case 0:
		mPac.y -= mPac.speed;
		break;
	case 1:
		mPac.x += mPac.speed;
		break;
	case 2:
		mPac.y += mPac.speed;
		break;
	case 3:
		mPac.x -= mPac.speed;
		break;
	default:
		break;
	}
}