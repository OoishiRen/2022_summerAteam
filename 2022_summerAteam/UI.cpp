#include "DxLib.h"
#include "SceneMgr.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"
#include "UI.h"
#include "Enemy.h"

extern float HitCheckEnemy(PAC* p, AKABEI* e);

static int score_image[15];
static int zanpakuto_image[10];
static int zanpakuto_image1[10];
static int zanpakuto_image2[10];
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
int ab;
int GrHandle;
//int high_score;

void UI_Initialize() {
	LoadDivGraph("player_images/num.png", 15, 15, 1, 16, 16, score_image);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image1);
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, zanpakuto_image2);
	GrHandle = LoadGraph("1UP.png");
	GrHandle = LoadGraph("GAME OVER.png");
	GrHandle = LoadGraph("HI-SCORE.png");
	GrHandle = LoadGraph("PLAYER.png");
	GrHandle = LoadGraph("READY!.png");
}

void  UI_Finalize() {

}

void UI_Updeta() {

}
//描画
void UI_Draw() {
	if (HitCheckEnemy(&mPac, &Akabei)) {
		DeleteGraph(zanpakuto_image[num6]);
		ab = 1;
	}
	if (HitCheckEnemy(&mPac, &Akabei) && ab == 1) {
		DeleteGraph(zanpakuto_image1[num7]);
		ab = 2;
	}
	if (HitCheckEnemy(&mPac, &Akabei) && ab == 2) {
		DeleteGraph(zanpakuto_image2[num8]);
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
			//// ハイスコアより大きければ
			//if (num10 > num5)
			//{
			//	// ハイスコアを新記録で更新
			//	num10 = esa; 
			//}
		}

		DrawRotaGraph(1000, 100, 2.0f, 0, score_image[num1], TRUE, FALSE);
		DrawRotaGraph(975, 100, 2.0f, 0, score_image[num2], TRUE, FALSE);
		DrawRotaGraph(950, 100, 2.0f, 0, score_image[num3], TRUE, FALSE);
		DrawRotaGraph(925, 100, 2.0f, 0, score_image[num4], TRUE, FALSE);
		DrawRotaGraph(900, 100, 2.0f, 0, score_image[num5], TRUE, FALSE);
		DrawRotaGraph(1000, 500, 1.0f, 0, zanpakuto_image[num6], TRUE, FALSE);
		DrawRotaGraph(950, 500, 1.0f, 0, zanpakuto_image1[num7], TRUE, FALSE);
		DrawRotaGraph(900, 500, 1.0f, 0, zanpakuto_image2[num8], TRUE, FALSE);
		DrawRotaGraph(900, 50, 2.0f, 0, score_image[num9], TRUE, FALSE);
		DrawRotaGraph(925, 50, 2.0f, 0, score_image[num10], TRUE, FALSE);
		DrawRotaGraph(950, 50, 2.0f, 0, score_image[num11], TRUE, FALSE);
		DrawRotaGraph(975, 50, 2.0f, 0, score_image[num12], TRUE, FALSE);
		DrawRotaGraph(1000, 50, 2.0f, 0, score_image[num13], TRUE, FALSE);
		DrawRotaGraph(320, 240, 1.0, 0.0, GrHandle, FALSE);
}




