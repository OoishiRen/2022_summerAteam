#include "DxLib.h"
#include "SceneMgr.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"
#include "UI.h"
#include "Enemy.h"

extern float HitCheckEnemy(PAC* p, AKABEI* e);
PAC pac;

static int score_image[15];
static int zanpakuto_image[12];
static int zanpakuto_image1[12];
static int zanpakuto_image2[12];

int hiscoreImg, oneupImg;

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
int fruit;
int a = 0;

void UI_Initialize() {
	LoadDivGraph("UI_images/num.png", 15, 15, 1, 16, 16, score_image);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image1);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image2);

	hiscoreImg = LoadGraph("UI_images/HI-SCORE.png");
	oneupImg = LoadGraph("UI_images/1UP.png");
}

void  UI_Finalize() {

}

void UI_Update() {
	if (EnemyScoreUI == true) {
		EnemyScoreUIEnabled();//フルーツUI
	}
	
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
	if (fruit == 1) {
		if (Round == 1) {
			num3 += 1;
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
		}
		if (Round == 2) {
			if (num3 <= 6) {
				num3 += 3;
			}
			else if (num3 >= 7) {
				num3 = num3 % 7;
				num4 += 1;
			}
			if (num4 > 9) {
				num4 = 0;
				num5 += 1;
			}
			if (num5 > 9) {
				num5 = 9;
			}
		}
		if (Round == 3 || Round == 4) {
			if (num3 <= 4) {
				num3 += 5;
			}
			else if (num3 >= 5) {
				num3 = num3 % 5;
				num4 += 1;
			}
			if (num4 > 9) {
				num4 = 0;
				num5 += 1;
			}
			if (num5 > 9) {
				num5 = 9;
			}
		}
		if (Round == 5 || Round == 6) {
			if (num3 <= 2) {
				num3 += 7;
			}
			else if (num3 >= 3) {
				num3 = num3 % 3;
				num4 += 1;
			}
			if (num4 > 9) {
				num4 = 0;
				num5 += 1;
			}
			if (num5 > 9) {
				num5 = 9;
			}
		}
		if (Round == 7 || Round == 8) {
			num4 += 1;
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
		}
		if (Round == 9 || Round == 10) {
			if (num4 <= 7) {
				num4 += 2;
			}
			else if (num4 >= 8) {
				num4 = num4 % 8;
				num5 += 1;
			}
			if (num5 > 9) {
				num5 = 9;
			}
		}
		if (Round == 11 || Round == 12) {
			if (num4 <= 6) {
				num4 += 3;
			}
			else if (num4 >= 7) {
				num4 = num4 % 7;
				num5 += 1;
			}
			if (num5 > 9) {
				num5 = 9;
			}
		}
		if (Round >= 13) {
			if (num4 <= 4) {
				num4 += 5;
			}
			else if (num4 >= 5) {
				num4 = num4 % 5;
				num5 += 1;
			}
			if (num5 > 9) {
				num5 = 9;
			}
		}
		fruit = 0;
	}

}
//描画
void UI_Draw() {
	
	//残機の表示
	for (int i = 0; i < mPac.cnt; i++) {
		DrawRotaGraph(900 + i * 50, 500, 3.0f, 0, zanpakuto_image2[num8], TRUE, FALSE);
	}

	//現在のスコア
	DrawRotaGraph(930, 150, 1.0f, 0, oneupImg, TRUE, FALSE);

	DrawRotaGraph(1020, 200, 2.0f, 0, score_image[num1], TRUE, FALSE);
	DrawRotaGraph(990, 200, 2.0f, 0, score_image[num2], TRUE, FALSE);
	DrawRotaGraph(960, 200, 2.0f, 0, score_image[num3], TRUE, FALSE);
	DrawRotaGraph(930, 200, 2.0f, 0, score_image[num4], TRUE, FALSE);
	DrawRotaGraph(900, 200, 2.0f, 0, score_image[num5], TRUE, FALSE);

	//ハイスコア
	DrawRotaGraph(930, 50, 1.0f, 0, hiscoreImg, TRUE, FALSE);

	DrawRotaGraph(900, 100, 2.0f, 0, score_image[num9], TRUE, FALSE);
	DrawRotaGraph(930, 100, 2.0f, 0, score_image[num10], TRUE, FALSE);
	DrawRotaGraph(960, 100, 2.0f, 0, score_image[num11], TRUE, FALSE);
	DrawRotaGraph(990, 100, 2.0f, 0, score_image[num12], TRUE, FALSE);
	DrawRotaGraph(1020, 100, 2.0f, 0, score_image[num13], TRUE, FALSE);

	DrawFormatString(1025, 600, GetColor(255, 255, 255), "%d", fruit);
}




