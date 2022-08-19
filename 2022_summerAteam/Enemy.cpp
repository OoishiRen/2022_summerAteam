#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"

struct AKABEI Akabei;
int MonsterImage[ENEMY_IMAGE_MAX];
int EyeImage[EYE_IMAGE_MAX];
int ed;


/* 仮プレイヤー */
float PlayerX;
float PlayerY;
int PlayerImage;
float PlayerSpeed;

float A, B, C;
float dx, dy;

void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); //アカベイの画像
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);
	Akabei.x = 1000.0f;
	Akabei.y = 400.0f;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;

	PlayerImage = MonsterImage[19];
	PlayerX = 900.0f;
	PlayerY = 400.0f;
	PlayerSpeed = 1.0f;
}

void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);
	}
	for (int i = 0; i < EYE_IMAGE_MAX; i++) {
		DeleteGraph(EyeImage[i]);
	}
	DeleteGraph(PlayerImage);
}

void Enemy_Update() {

	DrawFormatString(10, 10, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(10, 30, 255, "A = %.1f", A);
	DrawFormatString(10, 50, 255, "B = %.1f", B);
	DrawFormatString(10, 70, 255, "C = %.1f", C);
	DrawFormatString(10, 90, 255, "dx = %.1f", dx);
	DrawFormatString(10, 110, 255, "dy = %.1f", dy);
	DrawFormatString(10, 130, 255, "md = %d", Akabei.md);
	DrawFormatString(10, 150, 255, "nd = %d", Akabei.ed);

	// アニメーション
	if (Akabei.ImageCount == 0) {
		Akabei.ImageCount = 1;

	}
	else {
		Akabei.ImageCount = 0;
	}

	// 仮プレイヤーの横移動
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		PlayerX--;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		PlayerX++;
	}

	if (CheckHitKey(KEY_INPUT_UP)) {
		PlayerY--;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN)) {
		PlayerY++;
	}

	//AkabeiChasePlayer();

	Akabei.mx = Akabei.x;
	Akabei.my = Akabei.y;
	Akabei.md = Akabei.ed;

	while(1) {
		switch (Akabei.ed) {
		case 0:	// 左
			Akabei.x--;
			break;
		case 1:	// 右
			Akabei.x++;
		case 2:	// 上
			Akabei.y--;
		case 3:	// 下
			Akabei.y++;
		}

		if (Akabei.x > (1280 - 16) || Akabei.x < 900 || Akabei.y < 16 || Akabei.y > (720 - 160)) {
			Akabei.x = Akabei.mx;
			Akabei.y = Akabei.my;
			// 進む方向を決める
			switch (Akabei.md) {
			case 0:
				if (Akabei.ed == 0) {
					// 左に進んでいたら
					if (Akabei.y > PlayerY) {
						Akabei.ed = 2;
					}
					else {
						Akabei.ed = 3;
					}
				}
				else if (Akabei.ed == 2) {
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {
					Akabei.ed = 2;
				}
				break;

			case 1:
				if (Akabei.ed == 1) {
					// 右に進んでいた
					if (Akabei.y > PlayerY) {
						Akabei.ed = 2;
					}
					else {
						Akabei.ed = 3;
					}
				}
				//else if (Akabei.ed == 2) {
				//	Akabei.ed = 3;
				//}
				//else if (Akabei.ed == 3) {
				//	Akabei.ed = 2;
				//}
				break;

			case 2:
				if (Akabei.ed == 2) {
					// 上に進んでいた
					if (Akabei.x > PlayerX) {	// プレイヤーから見て、アカベイが右側にいたら
						Akabei.ed = 0;
					}
					else {
						Akabei.ed = 1;
					}
				}
				//else if (Akabei.ed == 2) {
				//	Akabei.ed = 3;
				//}
				//else if (Akabei.ed == 3) {
				//	Akabei.ed = 2;
				//}
				break;
			case 3:
				if (Akabei.ed == 3) {
					// 上に進んでいた
					if (Akabei.x > PlayerX) {	// プレイヤーから見て、アカベイが右側にいたら
						Akabei.ed = 0;
					}
					else {
						Akabei.ed = 1;
					}
				}
				//else if (Akabei.ed == 2) {
				//	Akabei.ed = 3;
				//}
				//else if (Akabei.ed == 3) {
				//	Akabei.ed = 2;
				//}
				break;
			}
		}
		else {
			break;
		}
	}
}

void Enemy_Draw() {

	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

	DrawRotaGraph(PlayerX, PlayerY, 1, 0, PlayerImage, TRUE);
}


// プレイヤー処理
void AkabeiChasePlayer() {
	// 三平方の定理を使う
	A = PlayerX - Akabei.x;

	B = PlayerY - Akabei.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る


	if (Akabei.x < PlayerX - 16) {	// アカベイから見てプレイヤーは右側
		Akabei.x += dx * Akabei.speed;
		Akabei.eyeImageCount = 1;
	}
	else if (Akabei.x > PlayerX + 16) {	// アカベイから見てプレイヤーは左側
		Akabei.x += dx * Akabei.speed;
		Akabei.eyeImageCount = 3;
	}

	if (Akabei.y < PlayerY - 16) {
		Akabei.y += dy * Akabei.speed;
		Akabei.eyeImageCount = 2;
	}
	else if (Akabei.y > PlayerY + 16) {
		Akabei.y += dy * Akabei.speed;
		Akabei.eyeImageCount = 0;
	}
}