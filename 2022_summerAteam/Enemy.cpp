#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"
#include "Game.h"
#include "Item.h"

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

	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 30, 255, "A = %.1f", A);
	DrawFormatString(1000, 50, 255, "B = %.1f", B);
	DrawFormatString(1000, 70, 255, "C = %.1f", C);
	DrawFormatString(1000, 90, 255, "dx = %.1f", dx);
	DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	DrawFormatString(1000, 150, 255, "nd = %d", Akabei.ed);

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
			Akabei.eyeImageCount = 3;
			break;
		case 1:	// 右
			Akabei.x++;
			Akabei.eyeImageCount = 1;
			break;
		case 2:	// 上
			Akabei.y--;
			Akabei.eyeImageCount = 0;
			break;
		case 3:	// 下
			Akabei.y++;
			Akabei.eyeImageCount = 2;
			break;
		}

		if (Akabei.x > (1280 - 16*3) || Akabei.x < 900 || Akabei.y < 16 || Akabei.y > (720 - 16*10)) {
			Akabei.x = Akabei.mx;
			Akabei.y = Akabei.my;
			// 進む方向を決める
			switch (Akabei.md) {
			case 0:
				if (Akabei.ed == 0) {
					// 左に進んでいたら
					if (Akabei.y > PlayerY) {	// アカベイの位置がプレイヤーより下なら
						Akabei.ed = 2;	// 上に移動
					}
					else {
						Akabei.ed = 3;	// 下に移動
					}
				}
				else if (Akabei.ed == 2) {	// 上に移動していたら
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {	// 下に移動していたら
					Akabei.ed = 2;
				}
				break;

			case 1:
				if (Akabei.ed == 1) {
					// 右に進んでいたら
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

			case 2:
				if (Akabei.ed == 2) {
					// 上に進んでいたら
					if (Akabei.x > PlayerX) {	// アカベイの位置がプレイヤーより右なら
						Akabei.ed = 0;
					}
					else {
						Akabei.ed = 1;
					}
				}
				else if (Akabei.ed == 0) {
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {
					Akabei.ed = 0;
				}
				break;

			case 3:
				if (Akabei.ed == 3) {
					// 下に進んでいたら
					if (Akabei.x > PlayerX) {	// アカベイの位置がプレイヤーより右なら
						Akabei.ed = 0;
					}
					else {
						Akabei.ed = 1;
					}
				}
				else if (Akabei.ed == 0) {
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {
					Akabei.ed = 0;
				}
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