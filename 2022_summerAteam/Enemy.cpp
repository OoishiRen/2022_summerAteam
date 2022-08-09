#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"

int AkabeiImage[ENEMY_IMAGE_MAX];

void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 32, 32, AkabeiImage); //エネミー画像
}

void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(AkabeiImage[i]);
	}
}

void Enemy_Update() {
	// プレイヤーを追いかける処理
}

void Enemy_Draw() {
	DrawRotaGraph(1000, 400, 1, 0, AkabeiImage[0], FALSE);
}