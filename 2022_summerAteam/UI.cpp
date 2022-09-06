#include "DxLib.h"
#include "SceneMgr.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"
#include "UI.h"

static int score_image[15];
int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;
int num5 = 0;
int esa;

void UI_Initialize() {
	LoadDivGraph("player_images/num.png", 15, 15, 1, 16, 16, score_image);
}

void  UI_Finalize() {

}

void UI_Updeta() {

}
//•`‰æ
void UI_Draw() {
	/*for (int i = 0; i < 9; i++)*/ 
		DrawRotaGraph(155, 20, 1.0f, 0, score_image[num1], TRUE, FALSE);
		DrawRotaGraph(140, 20, 1.0f, 0, score_image[num2], TRUE, FALSE);
		DrawRotaGraph(125, 20, 1.0f, 0, score_image[num3], TRUE, FALSE);
		DrawRotaGraph(110, 20, 1.0f, 0, score_image[num4], TRUE, FALSE);
		DrawRotaGraph(95, 20, 1.0f, 0, score_image[num5], TRUE, FALSE);
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
}



