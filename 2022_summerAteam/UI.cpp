#include "DxLib.h"
#include "SceneMgr.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"
#include "UI.h"
#include "Enemy.h"

extern float HitCheckEnemy(PAC* p, AKABEI* e);

static int score_image[15];
static int zanpakuto_image[12];
static int zanpakuto_image1[12];
static int zanpakuto_image2[12];

int HiScoreImg,oneUpImg,UiPlayerImg,ReadyImg;

int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;
int num5 = 0;
int num6 = 10;
int num7 = 10;
int num8 = 10;
int num9 = 1;
int num10 = 0;
int num11 = 0;
int num12 = 0;
int num13 = 0;
int esa;
int a;

int cnt;

void UI_Initialize() {

	//画像の読み込み(スコア)
	LoadDivGraph("UI_images/num.png", 15, 15, 1, 16, 16, score_image);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image1);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image2);
	//画像の読み込み(文字)
	HiScoreImg = LoadGraph("UI_images/HI-SCORE.png");
	oneUpImg = LoadGraph("UI_images/1UP.png");
	UiPlayerImg = LoadGraph("UI_images/PLAYER.png");
	ReadyImg = LoadGraph("UI_images/READY!.png");

	//カウント用初期化
	cnt = 0;
}

void  UI_Finalize() {
	for (int i = 0;i < 15;i++) {
		DeleteGraph(score_image[i]);
	}
	for (int j = 0; j < 12; j++) {
		DeleteGraph(zanpakuto_image[j]);
	}
	for (int l = 0; l < 12; l++) {
		DeleteGraph(zanpakuto_image1[l]);
	}
	for (int y = 0; y < 12; y++) {
		DeleteGraph(zanpakuto_image2[y]);
	}
	DeleteGraph(HiScoreImg);
	DeleteGraph(oneUpImg);
	DeleteGraph(UiPlayerImg);
	DeleteGraph(ReadyImg);
}

void UI_Update() {

	cnt++;//カウントアップ

	if (esa == 1) {
		num2 += 1;
		if (num2 > 9) {
			num2 = 0;
			num3 += 1;
		}
		if (num3 > 9) {
			num3 = 0;
			num4 += 1;
		}
		if (num4 > 9) {
			num4 = 0;
			num5 += 1;
		}
		if (num5 > 9) {
			num5 = 9;
		}
		esa = 0;
	}
	if (esa == 2) {

		if (num2 <= 4) {
			num2 += 5;
		}
		else if (num2 >= 5) {
			num2 = num2 % 5;
			num3 += 1;
		}

		if (num3 > 9) {
			num3 = 0;
			num4 += 1;
		}
		if (num4 > 9) {
			num4 = 0;
			num5 += 1;
		}
		if (num5 > 9) {
			num5 = 9;
		}
		esa = 0;

	}

	if (cnt > 30) {//0.5秒経過でリセット
		cnt = 0;
	}

}
//描画
void UI_Draw() {
		//ハイスコア
		DrawRotaGraph(960, 50, 1.0f, 0, HiScoreImg, TRUE, FALSE);

		DrawRotaGraph(900, 100, 2.0f, 0, score_image[num9], TRUE, FALSE);
		DrawRotaGraph(930, 100, 2.0f, 0, score_image[num10], TRUE, FALSE);
		DrawRotaGraph(960, 100, 2.0f, 0, score_image[num11], TRUE, FALSE);
		DrawRotaGraph(990, 100, 2.0f, 0, score_image[num12], TRUE, FALSE);
		DrawRotaGraph(1020, 100, 2.0f, 0, score_image[num13], TRUE, FALSE);

		//現在のスコア
		if (cnt <= 15) {
			DrawRotaGraph(960, 150, 1.0f, 0, oneUpImg, TRUE, FALSE);
		}

		DrawRotaGraph(1020, 200, 2.0f, 0, score_image[num1], TRUE, FALSE);
		DrawRotaGraph(990, 200, 2.0f, 0, score_image[num2], TRUE, FALSE);
		DrawRotaGraph(960, 200, 2.0f, 0, score_image[num3], TRUE, FALSE);
		DrawRotaGraph(930, 200, 2.0f, 0, score_image[num4], TRUE, FALSE);
		DrawRotaGraph(900, 200, 2.0f, 0, score_image[num5], TRUE, FALSE);

		//残機表示
		if (mPac.cnt != 0) {
			if (mPac.cnt > 2) {
				DrawRotaGraph(1000, 500, 3.0f, 0, zanpakuto_image[num6], TRUE, FALSE);
			}
			if (mPac.cnt > 1) {
				DrawRotaGraph(950, 500, 3.0f, 0, zanpakuto_image1[num7], TRUE, FALSE);
			}
			DrawRotaGraph(900, 500, 3.0f, 0, zanpakuto_image2[num8], TRUE, FALSE);
		}
}

void UI_Anim() {

}



