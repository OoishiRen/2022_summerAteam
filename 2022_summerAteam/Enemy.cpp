#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"


struct AKABEI Akabei;
int MonsterImage[ENEMY_IMAGE_MAX];
int AkabeiImagecount;

/* 仮プレイヤー */
int PlayerX;
int PlayerY;
int PlayerImage;
float PlayerSpeed;

void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); //アカベイの画像

	Akabei.x = 1000;
	Akabei.y = 400;
	Akabei.ImageCount = 0;
	Akabei.speed = 0.0f;

	PlayerImage = MonsterImage[19];
	PlayerX = 900;
	PlayerY = 400;
	PlayerSpeed = 0.0f;
}

void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);
	}
	DeleteGraph(PlayerImage);
}

void Enemy_Update() {
	// アニメーション
	if (Akabei.ImageCount == 0) {
		Akabei.ImageCount = 1;

	}
	else {
		Akabei.ImageCount = 0;
	}

	// 仮プレイヤーの横移動
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		PlayerSpeed--;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		PlayerSpeed++;
	}

	// プレイヤーを追いかける処理
	if (Akabei.x < PlayerX) {//アカベイから見て右側
		if (Akabei.x > PlayerX) {
			++Akabei.speed;
		}
	}
	if (Akabei.x > PlayerX) {
		if (Akabei.x < PlayerX) {//アカベイから見て左側
			--Akabei.speed;
		}
	}

}

void Enemy_Draw() {

	DrawRotaGraph(Akabei.x + Akabei.speed, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], FALSE);

	DrawRotaGraph(PlayerX + PlayerSpeed, PlayerY, 1, 0, PlayerImage, TRUE);
}