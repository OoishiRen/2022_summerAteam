#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"

struct AKABEI Akabei;					// �A�J�x�C�̍\���̂̐錾
int MonsterImage[ENEMY_IMAGE_MAX];		// �����X�^�[�̉摜�i�[�p�ϐ�
int EyeImage[EYE_IMAGE_MAX];			// �ڋʂ̉摜�i�[�p�ϐ�



float A, B, C;		// �O�����̒藝�p�̕ϐ�
float dx, dy;		// ���K���p�ϐ�


// ������
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // �����X�^�[�̉摜��ǂݍ���
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // �ڋʂ̉摜��ǂݍ���

	// �A�J�x�C�̏�����
	Akabei.x = 440.0f;
	Akabei.y = 40.0f;
	Akabei.w = 16.0f;
	Akabei.h = 16.0f;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;
	Akabei.WallHit = false;
	Akabei.left = false;
	Akabei.right = false;
	Akabei.up = false;
	Akabei.bottom = false;

}


// �I������
void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);	// �����X�^�[�̉摜�̉��
	}
	for (int i = 0; i < EYE_IMAGE_MAX; i++) {
		DeleteGraph(EyeImage[i]);		// �ڋʂ̉摜�̉��
	}
}


//�X�V
void Enemy_Update() {

	// �f�o�b�O�p�̕ϐ��̕\��
	DrawFormatString(1000, 30, 255, "A = %.1f", A);
	DrawFormatString(1000, 50, 255, "B = %.1f", B);
	DrawFormatString(1000, 70, 255, "C = %.1f", C);
	DrawFormatString(1000, 90, 255, "dx = %.1f", dx);
	DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	DrawFormatString(1000, 150, 255, "ed = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 210, 255, "mPac.x = %.1f", mPac.x);
	DrawFormatString(1000, 230, 255, "mPac.y = %.1f", mPac.y);
	DrawFormatString(1000, 250, 255, "Akabei.WallHit = %d", Akabei.WallHit);
	DrawFormatString(1000, 270, 255, "Akabei.left = %d", Akabei.left);
	DrawFormatString(1000, 290, 255, "Akabei.right = %d", Akabei.right);
	DrawFormatString(1000, 310, 255, "Akabei.up = %d", Akabei.up);
	DrawFormatString(1000, 330, 255, "Akabei.bottom = %d", Akabei.bottom);



	// �A�j���[�V����
	if (!PowerUpFlg) {
		if (Akabei.ImageCount == 0) {
			Akabei.ImageCount = 1;

		}
		else {
			Akabei.ImageCount = 0;
		}
	}
	else {
		if (PowerUpTime > 150) {
			if (Akabei.ImageCount == 16) {
				Akabei.ImageCount = 17;

			}
			else {
				Akabei.ImageCount = 16;
			}
		}
		else {
			if (Akabei.ImageCount < 19) {
				if (PowerUpTime % 5 == 0) {
					Akabei.ImageCount++;
				}
			}
			else
			{
				Akabei.ImageCount = 16;
			}
		}
	}
	//Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	//Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�

	AkabeiChasePlayer();		// �A�J�x�C�����v���C���[��ǂ������鏈��
	//AkabeiMove();
}


// �`��
void Enemy_Draw() {

	if (!PowerUpFlg) {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);
	}
	else {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
	}
}


// ���v���C���[��ǂ������鏈��
void AkabeiChasePlayer() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�

	// �O�����̒藝���g��
	A = mPac.x - Akabei.x;

	B = mPac.y - Akabei.y;

	C = sqrtf(A * A + B * B);	// A �� B ���Q�悵�đ������l�̕����������߂�

	dx = A / C;		// C ��1�i���K���j�Ƃ���ɂ́AA �� C �Ŋ���
	dy = B / C;		// C ��1�i���K���j�Ƃ���ɂ́AB �� C �Ŋ���


	if (Akabei.x < mPac.x - 16) {	// �A�J�x�C���猩�ăv���C���[�͉E��
		if (Akabei.WallHit == false) {
			Akabei.x += dx * Akabei.speed;
			Akabei.eyeImageCount = 1;
		}
		else {
			Akabei.WallHit = false;
		}
	}
	else if (Akabei.x > mPac.x + 16) {	// �A�J�x�C���猩�ăv���C���[�͍���
		if (Akabei.WallHit == false) {
			Akabei.x += dx * Akabei.speed;
			Akabei.eyeImageCount = 3;
		}
		else {
			Akabei.WallHit = false;
		}
	}
	else if (Akabei.y < mPac.y - 16) {	// �A�J�x�C���猩�ăv���C���[�͉���
		if (Akabei.WallHit == false) {
			Akabei.y += dy * Akabei.speed;
			Akabei.eyeImageCount = 2;
		}
		else {
			Akabei.WallHit = false;
		}
	}
	else if (Akabei.y > mPac.y + 16) {		// �A�J�x�C���猩�ăv���C���[�͏㑤
		if (Akabei.WallHit == false) {
			Akabei.y += dy * Akabei.speed;
			Akabei.eyeImageCount = 0;
		}
		else {
			Akabei.WallHit = false;
		}
	}
}

void AkabeiMove() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�
	Akabei.md = Akabei.ed;		// �G�̓���������ۑ�

	// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
	if (Akabei.WallHit == false) {
		switch (Akabei.ed) {
		case 0:	// ���ֈړ�
			Akabei.x--;
			Akabei.eyeImageCount = 3;
			if (Akabei.left == true) {
				Akabei.ed = 3;
			}
			break;
		case 1:	// �E�ֈړ�
			Akabei.x++;
			Akabei.eyeImageCount = 1;
			break;
		case 2:	// ��ֈړ�
			Akabei.y--;
			Akabei.eyeImageCount = 0;
			break;
		case 3:	// ���ֈړ�
			Akabei.y++;
			Akabei.eyeImageCount = 2;
			break;
		}
	}
}