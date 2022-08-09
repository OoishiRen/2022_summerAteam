#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"


struct AKABEI Akabei;
int MonsterImage[ENEMY_IMAGE_MAX];
int AkabeiImagecount;

/* ���v���C���[ */
int PlayerX;
int PlayerY;
int PlayerImage;
float PlayerSpeed;

void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); //�A�J�x�C�̉摜

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
	// �A�j���[�V����
	if (Akabei.ImageCount == 0) {
		Akabei.ImageCount = 1;

	}
	else {
		Akabei.ImageCount = 0;
	}

	// ���v���C���[�̉��ړ�
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		PlayerSpeed--;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		PlayerSpeed++;
	}

	// �v���C���[��ǂ������鏈��
	if (Akabei.x < PlayerX) {//�A�J�x�C���猩�ĉE��
		if (Akabei.x > PlayerX) {
			++Akabei.speed;
		}
	}
	if (Akabei.x > PlayerX) {
		if (Akabei.x < PlayerX) {//�A�J�x�C���猩�č���
			--Akabei.speed;
		}
	}

}

void Enemy_Draw() {

	DrawRotaGraph(Akabei.x + Akabei.speed, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], FALSE);

	DrawRotaGraph(PlayerX + PlayerSpeed, PlayerY, 1, 0, PlayerImage, TRUE);
}