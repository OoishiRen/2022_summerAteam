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


/* ���v���C���[�i�C�W�P�j */
float PlayerX;		// ���v���C���[��x���W
float PlayerY;		// ���v���C���[��y���W
int PlayerImage;	// ���v���C���[�̉摜�i�[�p�ϐ�

float A, B, C;		// �O�����̒藝�p�̕ϐ�
float dx, dy;		// ���K���p�ϐ�


// ������
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // �����X�^�[�̉摜��ǂݍ���
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // �ڋʂ̉摜��ǂݍ���

	// �A�J�x�C�̏�����
	Akabei.x = 1000.0f;
	Akabei.y = 400.0f;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;

	// ���v���C���[�̏�����
	PlayerImage = MonsterImage[19];
	PlayerX = 900.0f;
	PlayerY = 400.0f;
}


// �I������
void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);	// �����X�^�[�̉摜�̉��
	}
	for (int i = 0; i < EYE_IMAGE_MAX; i++) {
		DeleteGraph(EyeImage[i]);		// �ڋʂ̉摜�̉��
	}
	DeleteGraph(PlayerImage);		// ���v���C���[�̉摜�̉��
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
	DrawFormatString(1000, 150, 255, "nd = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 210, 255, "mPac.x = %.1f", mPac.x);
	DrawFormatString(1000, 230, 255, "mPac.y = %.1f", mPac.y);


	// �A�j���[�V����
	if (Akabei.ImageCount == 0) {
		Akabei.ImageCount = 1;

	}
	else {
		Akabei.ImageCount = 0;
	}


	// ���v���C���[�̉��ړ�
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

	//AkabeiChasePlayer();		// �A�J�x�C�����v���C���[��ǂ������鏈��

}


// �`��
void Enemy_Draw() {

	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

	DrawRotaGraph(PlayerX, PlayerY, 1, 0, PlayerImage, TRUE);
}


// ���v���C���[��ǂ������鏈��
void AkabeiChasePlayer() {
	// �O�����̒藝���g��
	A = PlayerX - Akabei.x;

	B = PlayerY - Akabei.y;

	C = sqrtf(A * A + B * B);	// A �� B ���Q�悵�đ������l�̕����������߂�

	dx = A / C;		// C ��1�i���K���j�Ƃ���ɂ́AA �� C �Ŋ���
	dy = B / C;		// C ��1�i���K���j�Ƃ���ɂ́AB �� C �Ŋ���


	if (Akabei.x < PlayerX - 16) {	// �A�J�x�C���猩�ăv���C���[�͉E��
		Akabei.x += dx * Akabei.speed;
		Akabei.eyeImageCount = 1;
	}
	else if (Akabei.x > PlayerX + 16) {	// �A�J�x�C���猩�ăv���C���[�͍���
		Akabei.x += dx * Akabei.speed;
		Akabei.eyeImageCount = 3;
	}

	if (Akabei.y < PlayerY - 16) {	// �A�J�x�C���猩�ăv���C���[�͉���
		Akabei.y += dy * Akabei.speed;
		Akabei.eyeImageCount = 2;
	}
	else if (Akabei.y > PlayerY + 16) {		// �A�J�x�C���猩�ăv���C���[�͏㑤
		Akabei.y += dy * Akabei.speed;
		Akabei.eyeImageCount = 0;
	}
}

void AkabeiMove() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�
	Akabei.md = Akabei.ed;		// �G�̓���������ۑ�

	// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
	while (1) {
		switch (Akabei.ed) {
		case 0:	// ���ֈړ�
			Akabei.x--;
			Akabei.eyeImageCount = 3;
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

		// �ǁi��ʒ[���炢�ɐݒ肵�Ă�j�ɓ���������
		if (Akabei.x > (1280 - 16 * 3) || Akabei.x < 900 || Akabei.y < 16 || Akabei.y >(720 - 16 * 10)) {
			// ���̏ꏊ�ɖ߂�
			Akabei.x = Akabei.mx;
			Akabei.y = Akabei.my;

			// �i�ޕ��������߂�
			switch (Akabei.md) {
			case 0:
				// ���ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͏ォ���ɂȂ�
				if (Akabei.ed == 0) {
					if (Akabei.y > PlayerY) {	// �A�J�x�C�̈ʒu�����v���C���[��艺�Ȃ�
						Akabei.ed = 2;	// ��ɕ�����ς���
					}
					else {
						Akabei.ed = 3;	// ���ɕ�����ς���
					}
				}
				else if (Akabei.ed == 2) {	// ���݂̈ʒu�̏オ�ǂ������̂ŉ��ɕ�����ς���
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {	// ���݂̈ʒu�̉����ǂ������̂ŏ�ɕ�����ς���
					Akabei.ed = 2;
				}
				break;

			case 1:
				// �E�ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͏ォ���ɂȂ�
				if (Akabei.ed == 1) {
					if (Akabei.y > PlayerY) {	// �A�J�x�C�̈ʒu�����v���C���[��艺�Ȃ�
						Akabei.ed = 2;		// ��ɕ�����ς���
					}
					else {
						Akabei.ed = 3;		// ���ɕ�����ς���
					}
				}
				else if (Akabei.ed == 2) {		// ���݂̈ʒu�̏オ�ǂ������̂ŉ��ɕ�����ς���
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {		// ���݂̈ʒu�̉����ǂ������̂ŏ�ɕ�����ς���
					Akabei.ed = 2;
				}
				break;

			case 2:
				// ��ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͉E�����ɂȂ�
				if (Akabei.ed == 2) {
					if (Akabei.x > PlayerX) {	// �A�J�x�C�̈ʒu�����v���C���[���E�Ȃ�
						Akabei.ed = 0;		// ���ɕ�����ς���
					}
					else {
						Akabei.ed = 1;		// �E�ɕ�����ς���
					}
				}
				else if (Akabei.ed == 0) {		// ���݂̈ʒu�̍����ǂ������̂ŉE�ɕ�����ς���
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {		// ���݂̈ʒu�̉E���ǂ������̂ō��ɕ�����ς���
					Akabei.ed = 0;
				}
				break;

			case 3:
				// ���ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͉E�����ɂȂ�
				if (Akabei.ed == 3) {
					if (Akabei.x > PlayerX) {	// �A�J�x�C�̈ʒu�����v���C���[���E�Ȃ�
						Akabei.ed = 0;		// ���ɕ�����ς���
					}
					else {
						Akabei.ed = 1;		// �E�ɕ�����ς���
					}
				}
				else if (Akabei.ed == 0) {		// ���݂̈ʒu�̍����ǂ������̂ŉE�ɕ�����ς���
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {		// ���݂̈ʒu�̉E���ǂ������̂ō��ɕ�����ς���
					Akabei.ed = 0;
				}
				break;
			}
		}
		else {
			break;		// �ړ��悪�ǂ���Ȃ��ꍇ�͕�����ς��郋�[�v���甲����
		}
	}

}