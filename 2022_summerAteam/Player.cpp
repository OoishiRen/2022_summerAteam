#include <math.h>

#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Game.h"
#include "Input.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "UI.h"

extern float HitCheckEnemy(PAC* p, AKABEI* e);

/*************************
** 変数 **
*************************/
static int pac_image[PAC_IMAGE_MAX];		//パックマン画像格納用
static int dying_image[DYING_IMAGE_MAX];	//ミス時の画像格納用

float count = 0;							//フレームカウント用
bool respawn = false;						//リスポーン用変数

/*************************
** 構造体 **
*************************/
struct PAC mPac;							//パックマン構造体

//初期化
void Player_Initialize() {
	//画像読み込み
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, pac_image);
	LoadDivGraph("player_images/dying.png", 11, 11, 1, 16, 16, dying_image);

	//構造体の初期化
	mPac.flg = true;
	mPac.type = 3;
	mPac.var = 3;
	mPac.img = 3;
	mPac.x = 240.0f;
	mPac.y = 392.0f;
	mPac.w = PAC_WIDTH;
	mPac.h = PAC_HEIGHT;
	mPac.speed = PAC_SPEED;
	mPac.cnt = PAC_LIFE;

	//カウント初期化
	count = 0;
}

//終了処理
void Player_Finalize() {

	for (int i = 0;i < PAC_IMAGE_MAX;i++) {
		DeleteGraph(pac_image[i]);
	}
	for (int i = 0; i < DYING_IMAGE_MAX; i++) {
		DeleteGraph(pac_image[i]);
	}
}

//更新
void Player_Update() {

	//上入力
	if (g_NowKey & PAD_INPUT_UP && mPac.type != 0 && !pac_y0) {
		mPac.var = 0;	//事前入力(上)
		/*mPac.type = 0;
		mPac.img = (3 * mPac.type);*/
	}
	//右入力
	if (g_NowKey & PAD_INPUT_RIGHT && mPac.type != 1 && !pac_x1) {
		mPac.var = 1;	//事前入力(右)
		/*mPac.type = 1;
		mPac.img = (3 * mPac.type);*/
	}
	//下入力
	if (g_NowKey & PAD_INPUT_DOWN && mPac.type != 2 && !pac_y1) {
		mPac.var = 2;	//事前入力(下)
		/*mPac.type = 2;
		mPac.img = (3 * mPac.type);*/
	}
	//左入力
	if (g_NowKey & PAD_INPUT_LEFT && mPac.type != 3 && !pac_x0) {
		mPac.var = 3;	//事前入力(左)
		/*mPac.type = 3;
		mPac.img = (3 * mPac.type);*/
	}

	//当たり判定(対アカベイ)
	if (HitCheckEnemy(&mPac, &Akabei)) {
		mPac.flg = false;
	}
	//当たり判定(対ピンキー)
	if (PowerUpFlg == false) {
		if (HitCheckEnemy(&mPac, &Pinkey)) {
			mPac.flg = false;
		}
	}
	//当たり判定(対アオスケ)
	if (PowerUpFlg == false) {
		if (HitCheckEnemy(&mPac, &Aosuke)) {
			mPac.flg = false;
		}
	}
	//当たり判定(対グズタ)
	if (PowerUpFlg == false) {
		if (HitCheckEnemy(&mPac, &Guzuta)) {
			mPac.flg = false;
		}
	}


	Pac_Anim(mPac.speed);	//パックマンのアニメーション
	Pac_Move(mPac.type);	//パックマンの動き

	//ミスした時のアニメーション終了後のリスポーン
	if (respawn) {
		respawn = false;
		mPac.flg = true;
		mPac.type = 3;
		mPac.var = 3;
		mPac.img = 3;
		mPac.speed = PAC_SPEED;
		mPac.x = 240.0f;
		mPac.y = 392.0f;
	}

}

//描画
void Player_Draw() {
	//パックマンの表示
	if (mPac.flg) {	//ゲーム中
		DrawRotaGraph(mPac.x, mPac.y, 1.0f, 0, pac_image[mPac.img], TRUE, FALSE);
	}
	else {			//ミス時
		DrawRotaGraph(mPac.x, mPac.y, 1.0f, 0, dying_image[mPac.img], TRUE, FALSE);
	}

	DrawFormatString(1000, 400, GetColor(255, 255, 255), "mPac.var:%d", mPac.var);		//デバッグ用
	DrawFormatString(1000, 420, GetColor(255, 255, 255), "mPac.type:%d", mPac.type);	//デバッグ用
}

/*************************************
** パックマンのアニメーション **
* 引　数 :パックマンの動くスピード
* 戻り値 :無し
**************************************/
void Pac_Anim(float val) {

	//1フレーム1カウント
	count += 1.0f;

	if (mPac.flg) {		//生き残っている場合
		if (count / val == 0.75f) {
			count = 0;
			if (mPac.img < (3 * (mPac.type + 1)) - 1) {
				mPac.img++;
			}
			else {
				mPac.img = (3 * mPac.type);
			}
		}
	}
	else {			//ミスした場合
		mPac.speed = 0.0f;	//パックマンの動きを停止

		if (count / (PAC_SPEED * 2) == 0.75f) {
			count = 0;
			if (mPac.img < 11) {
				mPac.img++;
			}
			else if(mPac.cnt > 0){
				respawn = true;
				mPac.cnt--;
				mPac.img = 0;
			}
		}
	}
}

/*************************************
** パックマンの移動 **
* 引　数 :パックマンの動く方向
* 戻り値 :無し
**************************************/
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

/*************************************
** パックマンとアカベイの当たり判定 **
* 引　数 :パックマンの値,アカベイの値
* 戻り値 :TRUE->ヒット,FALSE->無し
**************************************/
float HitCheckEnemy(PAC*p, AKABEI*e) {
	int sx1 = p->x - (int)(p->w * 0.5f);
	int sy1 = p->y - (int)(p->h * 0.5f);
	int sx2 = p->x + (int)(p->w * 0.5f);
	int sy2 = p->y + (int)(p->h * 0.5f);

	int dx1 = e->x - (int)(e->w * 0.5f);
	int dy1 = e->y - (int)(e->h * 0.5f);
	int dx2 = e->x + (int)(e->w * 0.5f);
	int dy2 = e->y + (int)(e->h * 0.5f);
	
	//矩形が重なっていれば当たり
	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
		//パワーアップ中はスルー（仮）
		if (!PowerUpFlg) {
			return TRUE;
		}
		else {
			Akabei.movenest = true;
		}
	}
	return FALSE;
}
