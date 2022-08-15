#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"

struct AKABEI Akabei;
int MonsterImage[ENEMY_IMAGE_MAX];
int AkabeiImagecount;
int EyeImage[4];

/* 仮プレイヤー */
float PlayerX;
float PlayerY;
int PlayerImage;
float PlayerSpeed;

float A, B, C;
float dx, dy;

void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); //アカベイの画像
	LoadDivGraph("enemy_images / eyes.png", 4, 4, 1, 16, 16, EyeImage);
	Akabei.x = 1000.0f;
	Akabei.y = 400.0f;
	Akabei.ImageCount = 0;
	Akabei.speed = 0.0f;

	PlayerImage = MonsterImage[19];
	PlayerX = 900.0f;
	PlayerY = 400.0f;
	PlayerSpeed = 1.0f;
}

void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);
	}
	for (int i = 0; i < 4; i++) {
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


	// プレイヤーを追いかける処理
	A = PlayerX - Akabei.x;

	B = PlayerY - Akabei.y;

	C = sqrtf(A * A + B * B);

	dx = A / C;
	dy = B / C;

	if (Akabei.x < PlayerX - 16) {
		Akabei.x += dx * Akabei.speed;
	}
	else if (Akabei.x > PlayerX + 16) {
		Akabei.x -= dx * Akabei.speed;
	}
	Akabei.x += dx * Akabei.speed;
	//Akabei.y += dy * Akabei.speed;

	//if (Akabei.x < PlayerX - 16) {//アカベイから見て右側
	//	Akabei.x++;
	//}
	//else if (Akabei.x > PlayerX + 16) {
	//	Akabei.x--;
	//}
}

void Enemy_Draw() {

	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], FALSE);
	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[0], TRUE);

	DrawRotaGraph(PlayerX, PlayerY, 1, 0, PlayerImage, TRUE);
}

