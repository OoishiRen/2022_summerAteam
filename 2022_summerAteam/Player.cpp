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
static int pac_image[PAC_IMAGE_MAX];
static int dying_image[DYING_IMAGE_MAX];

float count = 0;
bool respawn = false;

/*************************
** 構造体 **
*************************/
struct PAC mPac;

//初期化
void Player_Initialize() {
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, pac_image);
	LoadDivGraph("player_images/dying.png", 11, 11, 1, 16, 16, dying_image);

	mPac.flg = true;
	mPac.type = 3;
	mPac.img = 3;
	mPac.x = 240.0f;
	mPac.y = 392.0f;
	mPac.w = PAC_WIDTH;
	mPac.h = PAC_HEIGHT;
	mPac.r = PAC_WIDTH / 2;
	mPac.speed = PAC_SPEED;
	mPac.cnt = PAC_LIFE;

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
	if (g_NowKey & PAD_INPUT_UP && mPac.type != 0 && !pac_y0) {
		mPac.var = 0;
		/*mPac.type = 0;
		mPac.img = (3 * mPac.type);*/
	}
	if (g_NowKey & PAD_INPUT_RIGHT && mPac.type != 1 && !pac_x1) {
		mPac.var = 1;
		/*mPac.type = 1;
		mPac.img = (3 * mPac.type);*/
	}
	if (g_NowKey & PAD_INPUT_DOWN && mPac.type != 2 && !pac_y1) {
		mPac.var = 2;
		/*mPac.type = 2;
		mPac.img = (3 * mPac.type);*/
	}
	if (g_NowKey & PAD_INPUT_LEFT && mPac.type != 3 && !pac_x0) {
		mPac.var = 3;
		/*mPac.type = 3;
		mPac.img = (3 * mPac.type);*/
	}
	if (HitCheckEnemy(&mPac, &Akabei)) {
		mPac.flg = false;
	}
	Pac_Anim(mPac.speed);
	Pac_Move(mPac.type);

	if (respawn) {
		respawn = false;
		mPac.flg = true;
		mPac.speed = PAC_SPEED;
		mPac.x = 240.0f;
		mPac.y = 392.0f;
	}

}

//描画
void Player_Draw() {
	if (mPac.flg) {
		DrawRotaGraph(mPac.x, mPac.y, 1.0f, 0, pac_image[mPac.img], TRUE, FALSE);
	}
	else {
		DrawRotaGraph(mPac.x, mPac.y, 1.0f, 0, dying_image[mPac.img], TRUE, FALSE);
	}

	DrawFormatString(1000, 400, GetColor(255, 255, 255), "mPac.var:%d", mPac.var);
	DrawFormatString(1000, 420, GetColor(255, 255, 255), "mPac.type:%d", mPac.type);
}

void Pac_Anim(float val) {
	count += 1.0f;
	if (mPac.flg) {
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
	else {
		mPac.speed = 0.0f;	//停止

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
* 戻り値 :無し
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
	}
	return FALSE;
}
