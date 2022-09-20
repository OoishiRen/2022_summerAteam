#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"

struct AKABEI Akabei;					// �A�J�x�C�̍\���̂̐錾
struct AKABEI Pinkey;
struct AKABEI Aosuke;
struct AKABEI Guzuta;
int MonsterImage[ENEMY_IMAGE_MAX];		// �����X�^�[�̉摜�i�[�p�ϐ�
int EyeImage[EYE_IMAGE_MAX];			// �ڋʂ̉摜�i�[�p�ϐ�

int px, py, px2, py2;

float A, B, C;		// �O�����̒藝�p�̕ϐ�
float dx, dy;		// ���K���p�ϐ�

int eCnt;

int ScatterModeTime;//�꒣�胂�[�h�̎��ԁi�t���[���j
int ChaseModeTime;//�ǐՃ��[�h�̎��ԁi�t���[���j
bool EnemyMode; // True = �꒣�胂�[�h�Afalse = �ǐՃ��[�h
int IjikeCnt;

// ������
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // �����X�^�[�̉摜��ǂݍ���
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // �ڋʂ̉摜��ǂݍ���

	// �A�J�x�C�̏�����
	Akabei.x = 240.0f;
	Akabei.y = 200.0f;
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

	// �s���L�[�̏�����
	Pinkey.x = 240.0f;
	Pinkey.y = 240.0f;
	Pinkey.w = ENEMY_SIZE;
	Pinkey.h = ENEMY_SIZE;
	Pinkey.ed = 2;
	Pinkey.ImageCount = 2;
	Pinkey.eyeImageCount = 0;
	Pinkey.speed = 1.5f;
	Pinkey.WallHit = false;
	Pinkey.left = false;
	Pinkey.right = false;
	Pinkey.up = false;
	Pinkey.bottom = false;

	// �A�I�X�P�̏�����
	Aosuke.x = 200.0f;
	Aosuke.y = 260.0f;
	Aosuke.w = ENEMY_SIZE;
	Aosuke.h = ENEMY_SIZE;
	Aosuke.ed = 2;
	Aosuke.ImageCount = 4;
	Aosuke.eyeImageCount = 0;
	Aosuke.speed = 1.5f;
	Aosuke.WallHit = false;
	Aosuke.left = false;
	Aosuke.right = false;
	Aosuke.up = false;
	Aosuke.bottom = false;

	// �O�Y�^�̏�����
	Guzuta.x = 280.0f;
	Guzuta.y = 260.0f;
	Guzuta.w = ENEMY_SIZE;
	Guzuta.h = ENEMY_SIZE;
	Guzuta.ed = 2;
	Guzuta.ImageCount = 6;
	Guzuta.eyeImageCount = 0;
	Guzuta.speed = 1.5f;
	Guzuta.WallHit = false;
	Guzuta.left = false;
	Guzuta.right = false;
	Guzuta.up = false;
	Guzuta.bottom = false;

	eCnt = 0;
	ScatterModeTime = 480;
	ChaseModeTime = 1230;
	EnemyMode = true;
	IjikeCnt = 20;
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

	//DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	//DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	//DrawFormatString(1000, 150, 255, "ed = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 210, 255, "Akabei.x = %.1f", Akabei.x / 16);
	DrawFormatString(1000, 230, 255, "Akabei.y = %.1f", Akabei.y / 16);
	DrawFormatString(1000, 250, 255, "Akabei.WallHit = %d", Akabei.WallHit);
	DrawFormatString(1000, 270, 255, "Akabei.left = %d", Akabei.left);
	DrawFormatString(1000, 290, 255, "Akabei.right = %d", Akabei.right);
	DrawFormatString(1000, 310, 255, "Akabei.up = %d", Akabei.up);
	DrawFormatString(1000, 330, 255, "Akabei.bottom = %d", Akabei.bottom);
	DrawFormatString(1000, 350, 255, "Akabei.mapY = %d", Akabei.mapY);
	DrawFormatString(1000, 370, 255, "%d", IjikeCnt);


	DrawFormatString(500, 30, GetColor(255, 255, 255),
		EnemyMode ? "Scatter %d" : "Chase %d", EnemyMode ? ScatterModeTime : ChaseModeTime);


	// �A�j���[�V����
	if (!PowerUpFlg) {

		if (eCnt < ENEMY_CNT_SPEED) {
			eCnt++;
		}
		else if (eCnt == ENEMY_CNT_SPEED) {
			eCnt = 0;
		}

		if (eCnt < ENEMY_CNT_SPEED / 2) {
			Akabei.ImageCount = 1;
			Pinkey.ImageCount = 3;
			Aosuke.ImageCount = 5;
			Guzuta.ImageCount = 7;
		}
		else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
			Akabei.ImageCount = 0;
			Pinkey.ImageCount = 2;
			Aosuke.ImageCount = 4;
			Guzuta.ImageCount = 6;
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


			if (eCnt < ENEMY_CNT_SPEED) {
				eCnt++;
			}
			else if (eCnt == ENEMY_CNT_SPEED) {
				eCnt = 0;
			}
			if (eCnt < ENEMY_CNT_SPEED / 2) {
				Pinkey.ImageCount = 16;
			}
			else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
				Pinkey.ImageCount = 17;
			}

		}
		else {
			if (Akabei.ImageCount < 19) {
				if (PowerUpTime % 5 == 0) {
					Akabei.ImageCount++;
					Pinkey.ImageCount++;
				}
			}
			else
			{
				Akabei.ImageCount = 16;
				Pinkey.ImageCount = 16;
			}
		}


	}
	//Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	//Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�

	AkabeiChasePlayer();		// �A�J�x�C�����v���C���[��ǂ������鏈��

	ModeChange();

	//AkabeiMove2();
	AosukeMove();
	GuzutaMove();
}


// �`��
void Enemy_Draw() {
	if (!PowerUpFlg) {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);
		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, EyeImage[Pinkey.eyeImageCount], TRUE);


		DrawRotaGraph(Aosuke.x, Aosuke.y, 1, 0, MonsterImage[Aosuke.ImageCount], TRUE);
		DrawRotaGraph(Aosuke.x, Aosuke.y, 1, 0, EyeImage[Aosuke.eyeImageCount], TRUE);

		DrawRotaGraph(Guzuta.x, Guzuta.y, 1, 0, MonsterImage[Guzuta.ImageCount], TRUE);
		DrawRotaGraph(Guzuta.x, Guzuta.y, 1, 0, EyeImage[Guzuta.eyeImageCount], TRUE);

	}
	else {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);

		DrawRotaGraph(Aosuke.x, Aosuke.y, 1, 0, MonsterImage[Aosuke.ImageCount], TRUE);

		DrawRotaGraph(Guzuta.x, Guzuta.y, 1, 0, MonsterImage[Guzuta.ImageCount], TRUE);

	}
}


// ���v���C���[��ǂ������鏈��
void AkabeiChasePlayer() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�
	Akabei.md = Akabei.ed;		// �G�̓���������ۑ�

	Akabei.mapX = (int)Akabei.x / 16;
	Akabei.mapY = (int)Akabei.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// �O�����̒藝���g��
	A = mPac.x - Akabei.x;

	B = mPac.y - Akabei.y;

	C = sqrtf(A * A + B * B);	// A �� B ���Q�悵�đ������l�̕����������߂�

	dx = A / C;		// C ��1�i���K���j�Ƃ���ɂ́AA �� C �Ŋ���
	dy = B / C;		// C ��1�i���K���j�Ƃ���ɂ́AB �� C �Ŋ���


	// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
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


	DrawLine(mPac.x, mPac.y, Akabei.x, Akabei.y, GetColor(255, 0, 0));

	// �A�J�x�C���i��ł�������ɂ���ď�����؂�ւ���
	//switch (Akabei.md) {
	//case 0:	// �A�J�x�C�����ɐi��ł�����
	//	// �\���H�ɗ�����
	//	if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//		// �ǂɓ������Ă��Ȃ�������
	//		if (Akabei.WallHit == false) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX - 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
	//				if (dy < 0.0f) {
	//					Akabei.ed = 2;				// ��ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//				else {
	//					Akabei.ed = 3;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ō��ɕǂ��������ꍇ
	//	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX - 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
	//				if (dy < 0.0f) {
	//					Akabei.ed = 2;				// ��ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//				else {
	//					Akabei.ed = 3;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ŏ�ɕǍ������ꍇ
	//	else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX - 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//				if (dy > 0.0f) {
	//					Akabei.ed = 3;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ŉ��ɕǍ������ꍇ
	//	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX - 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//				if (dy < 0.0f) {
	//					Akabei.ed = 2;				// ��ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// ���Ə�ɕǂ��������ꍇ
	//	else if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//			if (dy > 0.0f) {
	//				Akabei.ed = 3;				// ���ɕ�����ς���
	//				Akabei.WallHit = false;
	//			}
	//		}
	//	}
	//	// ���Ɖ��ɕǂ��������ꍇ
	//	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//			if (dy < 0.0f) {
	//				Akabei.ed = 2;				// ��ɕ�����ς���
	//				Akabei.WallHit = false;
	//			}
	//		}
	//	}
	//case 1:
	//	// �\���H�ɗ�����
	//	if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//		// �ǂɓ������Ă��Ȃ�������
	//		if (Akabei.WallHit == false) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX + 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
	//				if (dy < 0.0f) {
	//					Akabei.ed = 2;				// ��ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//				else {
	//					Akabei.ed = 3;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ŉE�ɕǂ���������
	//	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX + 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
	//				if (dy < 0.0f) {
	//					Akabei.ed = 2;				// ��ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//				else {
	//					Akabei.ed = 3;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ŏ�ɕǂ���������
	//	else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX - 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//				if (dy > 0.0f) {
	//					Akabei.ed = 3;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ŉ��ɕǍ������ꍇ
	//	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapX - 0.5f == Akabei.x / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//				if (dy < 0.0f) {
	//					Akabei.ed = 2;				// ��ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// �E�Ə�ɕǂ��������ꍇ
	//	else if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//			if (dy > 0.0f) {
	//				Akabei.ed = 3;				// ���ɕ�����ς���
	//				Akabei.WallHit = false;
	//			}
	//		}
	//	}
	//	// �E�Ɖ��ɕǂ��������ꍇ
	//	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
	//		// �ǂɓ������Ă�����
	//		if (Akabei.WallHit == true) {
	//			// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
	//			if (dy < 0.0f) {
	//				Akabei.ed = 2;				// ��ɕ�����ς���
	//				Akabei.WallHit = false;
	//			}
	//		}
	//	}
	//case 2:
	//	// �\���H�ɗ�����
	//	if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//		// �ǂɓ������Ă��Ȃ�������
	//		if (Akabei.WallHit == false) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapY - 0.5f == Akabei.y / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C��荶�Ȃ�
	//				if (dx < 0.0f) {
	//					Akabei.ed = 0;				// ���ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//				else {
	//					Akabei.ed = 1;				// �E�ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}
	//	// T���H�ō��ɕǂ���������
	//	if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//		// �ǂɓ������Ă��Ȃ�������
	//		if (Akabei.WallHit == false) {
	//			// �A�J�x�C���ǂɈ��������炸�ɋȂ����悤��
	//			if ((float)Akabei.mapY - 0.5f == Akabei.y / 16.0f) {
	//				// �p�b�N�}���̈ʒu���A�J�x�C���E�Ȃ�
	//				if (dx > 0.0f) {
	//					Akabei.ed = 1;				// �E�ɕ�����ς���
	//					Akabei.WallHit = false;
	//				}
	//			}
	//		}
	//	}

	//}

	 // ���Ə�ɕǂ��������ꍇ
	if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// �E��������Ă�����A���ɕ�����ς���
			if (Akabei.md == 0) {
				Akabei.ed = 3;				// ���ɕ�����ς���
				Akabei.WallHit = false;
			}
			// ����������Ă�����A�E�ɕ�����ς���
			else if (Akabei.md == 2) {
				Akabei.ed = 1;				// �E�ɕ�����ς���
				Akabei.WallHit = false;
			}
		}
	}
	// ���Ɖ��ɕǂ���������
	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// �E��������Ă�����A��ɕ�����ς���
			if (Akabei.md == 0) {
				Akabei.ed = 2;				// ��ɕ�����ς���
				Akabei.WallHit = false;
			}
			// �ォ������Ă�����A�E�ɕ�����ς���
			else if (Akabei.md == 3) {
				Akabei.ed = 1;				// �E�ɕ�����ς���
				Akabei.WallHit = false;
			}
		}
	}
	// �E�Ə�ɕǂ�����ꍇ
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// ����������Ă�����A���ɕ�����ς���
			if (Akabei.md == 1) {
				Akabei.ed = 3;				// ���ɕ�����ς���
				Akabei.WallHit = false;
			}
			// ����������Ă�����A���ɕ�����ς���
			else if (Akabei.md == 2) {	// ��ɐi��ł�����
				Akabei.ed = 0;				// ���ɕ�����ς���
				Akabei.WallHit = false;
			}
		}
	}
	// �E�Ɖ��ɕǂ�����ꍇ
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// ����������Ă�����A��ɕ�����ς���
			if (Akabei.md == 1) {	// �E�ɐi��ł�����
				Akabei.ed = 2;				// ��ɕ�����ς���
				Akabei.WallHit = false;
			}
			// �ォ������Ă�����A���ɕ�����ς���
			else if (Akabei.md == 3) {	// ���ɐi��ł�����
				Akabei.ed = 0;				// ���ɕ�����ς���
				Akabei.WallHit = false;
			}
		}
	}
	// �ゾ���ɕǂ��������ꍇ
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			if (Akabei.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C���E�Ȃ�
				if (dx > 0.0f) {
					Akabei.ed = 1;				// �E�ɕ�����ς���
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;				// ���ɕ�����ς���
					Akabei.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Akabei.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
						Akabei.ed = 3;				// ���ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
						Akabei.ed = 3;				// ���ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// �ǂɓ��������ꍇ
		if (Akabei.WallHit == true) {
			// �ォ������Ă�����
			if (Akabei.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C���E�Ȃ�
				if (dx > 0.0f) {
					Akabei.ed = 1;				// �E�ɕ�����ς���
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;				// ���ɕ�����ς���
					Akabei.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Akabei.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
						Akabei.ed = 2;				// ��ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
						Akabei.ed = 2;				// ��ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	//�@�E�����ɕǂ��������ꍇ
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			if (Akabei.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
				if (dy > 0.0f) {
					Akabei.ed = 3;				// ���ɕ�����ς���
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 2;				// ��ɕ�����ς���
					Akabei.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Akabei.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
						Akabei.ed = 0;				// ���ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
						Akabei.ed = 0;				// ���ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			if (Akabei.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���A�J�x�C��艺�Ȃ�
				if (dy > 0.0f) {
					Akabei.ed = 3;				// ���ɕ�����ς���
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 2;				// ��ɕ�����ς���
					Akabei.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Akabei.md == 2) {	// ��Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���A�J�x�C���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
						Akabei.ed = 1;				// �E�ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 3) {	// ���Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���A�J�x�C���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
						Akabei.ed = 1;				// �E�ɕ�����ς���
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// �\���H�̏ꍇ
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// �ǂɓ������Ă��Ȃ�������
		if (Akabei.WallHit == false)
		{		// �A�J�x�C���i��ł�������ɂ���ď��������߂�
			switch (Akabei.md) {
			case 0:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
					// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
					if (dy < 0.0f) {
						Akabei.ed = 2;				// ��ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;				// ���ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
				}
			case 1:	// �E�ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
					// �p�b�N�}���̈ʒu���A�J�x�C����Ȃ�
					if (dy < 0.0f) {
						Akabei.ed = 2;				// ��ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;				// ���ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
				}
			case 2:	// ��ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
					// �p�b�N�}���̈ʒu���A�J�x�C��荶�Ȃ�
					if (dx < 0.0f) {
						Akabei.ed = 0;				// ���ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;				// �E�ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
				}
			case 3:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
					// �p�b�N�}���̈ʒu���A�J�x�C��荶�Ȃ�
					if (dx < 0.0f) {
						Akabei.ed = 0;				// ���ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;				// �E�ɕ�����ς���
						Akabei.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void AkabeiMove() {

}

// ���E�㉺�̓����蔻���ǂ������������������f���Đi�ޕ��������߂鏈��
// �r���̒ʘH�ɂ͓��炸�ɕǂɓ�����i��
void AkabeiMove2() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�
	Akabei.md = Akabei.ed;		// �G�̓���������ۑ�



	// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
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

	if (Akabei.WallHit == true) {
		// �A�J�x�C�����ɐi��ł���ꍇ
		if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
			if (Akabei.ed == 0) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 2) {
				Akabei.ed = 1;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
			// �v���C���[�̈ʒu���A�J�x�C��艺�Ȃ�
			if (mPac.y > Akabei.y) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
			if (Akabei.ed == 0) {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 3) {
				Akabei.ed = 1;
				Akabei.WallHit = false;
			}
		}

		// �A�J�x�C���E�ɐi��ł���ꍇ
		else if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
			if (Akabei.ed == 1) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 2) {
				Akabei.ed = 0;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
			if (mPac.y > Akabei.y) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
			if (Akabei.ed == 1) {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 3) {
				Akabei.ed = 0;
				Akabei.WallHit = false;
			}

		}
		else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
			// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
			if (mPac.x >= Akabei.x) {
				if (Akabei.ed == 2) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
				}
			}
			else {
				if (Akabei.ed == 2) {
					Akabei.ed = 0;
					Akabei.WallHit = false;
				}
			}
		}
		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
			// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
			if (mPac.x >= Akabei.x) {
				if (Akabei.ed == 3) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
				}
			}
			else {
				if (Akabei.ed == 3) {
					Akabei.ed = 0;
					Akabei.WallHit = false;
				}
			}
		}
	}
	//else {
	//	if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//		if (Akabei.ed == 0) {
	//			if (mPac.y > Akabei.y) {
	//				Akabei.ed = 3;
	//				Akabei.WallHit = false;
	//			}
	//		}
	//		else if (Akabei.ed == 2) {
	//			if (mPac.x > Akabei.x) {
	//				Akabei.ed = 1;
	//				Akabei.WallHit = false;
	//			}
	//			else {
	//				Akabei.ed = 0;
	//				Akabei.WallHit = false;
	//			}
	//		}
	//	}
	//}
}


void ModeChange() {
	if (EnemyMode == true) {//�꒣�胂�[�h
		ChaseModeTime = 1230;//�ǐՃ��[�h�̎��Ԃ�������
		//�꒣�胂�[�h�̎��Ԃ�����Ƃ�&&�p�b�N�}�����p���[�A�b�v��ザ��Ȃ���
		if (ScatterModeTime > 0 != PowerUpFlg) {
			ScatterMode();//�꒣�胂�[�h�̏����֐�
			ScatterModeTime--;//�꒣�胂�[�h�̎��Ԃ����炷
		}
		//�p�b�N�}�����p���[�A�b�v���[�h�̎�
		else if (PowerUpFlg == true) {
			//�C�W�P��Ԃɂ���
			IjikeMode();
		}//�꒣�胂�[�h�̎��Ԃ������Ȃ�����
		else if (ScatterModeTime == 0) {
			EnemyMode = false;//�ǐՃ��[�h�ɂ���
		}
	}
	else if (EnemyMode == false) {//�ǐՃ��[�h
		ScatterModeTime = 480;//�꒣�胂�[�h�̎��Ԃ�������
		//�ǐՃ��[�h�̎��Ԃ�����Ƃ�&&�p�b�N�}�����p���[�A�b�v��ザ��Ȃ���
		if (ChaseModeTime > 0 != PowerUpFlg) {
			ChaseMode();//�ǐՃ��[�h�̏����֐�
			ChaseModeTime--;//�ǐՃ��[�h�̎��Ԃ����炷
		}
		//�p�b�N�}�����p���[�A�b�v���[�h�̎�
		else if (PowerUpFlg == true) {
			//�C�W�P��Ԃɂ���
			IjikeMode();
		}//�ǐՃ��[�h�̎��Ԃ������Ȃ�����
		else if (ChaseModeTime == 0) {
			EnemyMode = true;//�꒣�胂�[�h�ɂ���
		}
	}
}

void ScatterMode() {

	//int ax, ay, bx, by, cx, cy;
	//int difx, dify, difc;
	//int goal;
	//ax = 2 * MAP_SIZE;
	//ay = 2 * MAP_SIZE;
	//bx = Pinkey.x;
	//by = Pinkey.y;
	//Pinkey.mx = Pinkey.x;		// �s���L�[��x���W��ۑ�
	//Pinkey.my = Pinkey.y;		// �s���L�[��y���W��ۑ�
	//Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�
	//for (int i = 0; i < MAP_HEIGHT; i++) {
	//	for (int j = 0; j < MAP_WIDTH; j++) {
	//		if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
	//			j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
	//			DrawLine(2 * MAP_SIZE + 4, 2 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
	//		}
	//	}
	//}
	// //�s���L�[���ǂ�����Ȃ���ړ����鏈��
	//switch (Pinkey.ed) {
	//case 0:	// ���ֈړ�
	//	Pinkey.x -= Pinkey.speed;
	//	Pinkey.eyeImageCount = 3;
	//	break;
	//case 1:	// �E�ֈړ�
	//	Pinkey.x += Pinkey.speed;
	//	Pinkey.eyeImageCount = 1;
	//	break;
	//case 2:	// ��ֈړ�
	//	Pinkey.y -= Pinkey.speed;
	//	Pinkey.eyeImageCount = 0;
	//	break;
	//case 3:	// ���ֈړ�
	//	Pinkey.y += Pinkey.speed;
	//	Pinkey.eyeImageCount = 2;
	//	break;
	//}
	//if (Pinkey.WallHit == true) {//�ǂɓ���������
	//	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
	//		if (Pinkey.ed == 0) {// �s���L�[�����ɐi��ł���ꍇ
	//			Pinkey.ed = 3;//���ֈړ�
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 2) {//��ֈړ����Ă���ꍇ
	//			Pinkey.ed = 1;//�E�ֈړ�
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
	//		// �꒣��̈ʒu���s���L�[��艺�Ȃ�
	//		if (ay > Pinkey.y) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
	//		if (Pinkey.ed == 0) {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 3) {
	//			Pinkey.ed = 1;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	// �s���L�[�E�ɐi��ł���ꍇ
	//	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
	//		if (Pinkey.ed == 1) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 2) {
	//			Pinkey.ed = 0;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
	//		//�꒣��̈ʒu���s���L�[����Ȃ�
	//		if (ay > Pinkey.y) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
	//		if (Pinkey.ed == 1) {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 3) {
	//			Pinkey.ed = 0;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
	//		// �꒣��̈ʒu���s���L�[���E�Ȃ�
	//		if (ax >= Pinkey.x) {
	//			if (Pinkey.ed == 2) {
	//				Pinkey.ed = 1;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//		else {
	//			if (Pinkey.ed == 2) {
	//				Pinkey.ed = 0;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
	//		// �꒣��̈ʒu���s���L�[���E�Ȃ�
	//		if (ax >= Pinkey.x) {
	//			if (Pinkey.ed == 3) {
	//				Pinkey.ed = 1;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//		else {
	//			if (Pinkey.ed == 3) {
	//				Pinkey.ed = 0;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//	}
	//}

	Pinkey.mx = Pinkey.x;		// �s���L�[��x���W��ۑ�
	Pinkey.my = Pinkey.y;		// �s���L�[��y���W��ۑ�
	Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�

	Pinkey.mapX = (int)Pinkey.x / 16;
	Pinkey.mapY = (int)Pinkey.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// �O�����̒藝���g��
	A = 2 * MAP_SIZE - Pinkey.x;

	B = 2 * MAP_SIZE - Pinkey.y;

	C = sqrtf(A * A + B * B);	// A �� B ���Q�悵�đ������l�̕����������߂�

	dx = A / C;		// C ��1�i���K���j�Ƃ���ɂ́AA �� C �Ŋ���
	dy = B / C;		// C ��1�i���K���j�Ƃ���ɂ́AB �� C �Ŋ���

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(2 * MAP_SIZE + 4, 2 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));


			}
		}
	}
	// �s���L�[���ǂ�����Ȃ���ړ����鏈��
	switch (Pinkey.ed) {
	case 0:	// ���ֈړ�
		Pinkey.x--;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// �E�ֈړ�
		Pinkey.x++;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// ��ֈړ�
		Pinkey.y--;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// ���ֈړ�
		Pinkey.y++;
		Pinkey.eyeImageCount = 2;
		break;
	}
	// ���Ə�ɕǂ��������ꍇ
	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// �E��������Ă�����A���ɕ�����ς���
			if (Pinkey.md == 0) {
				Pinkey.ed = 3;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// ����������Ă�����A�E�ɕ�����ς���
			else if (Pinkey.md == 2) {
				Pinkey.ed = 1;				// �E�ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// ���Ɖ��ɕǂ���������
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// �E��������Ă�����A��ɕ�����ς���
			if (Pinkey.md == 0) {
				Pinkey.ed = 2;				// ��ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// �ォ������Ă�����A�E�ɕ�����ς���
			else if (Pinkey.md == 3) {
				Pinkey.ed = 1;				// �E�ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �E�Ə�ɕǂ�����ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// ����������Ă�����A���ɕ�����ς���
			if (Pinkey.md == 1) {
				Pinkey.ed = 3;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// ����������Ă�����A���ɕ�����ς���
			else if (Pinkey.md == 2) {	// ��ɐi��ł�����
				Pinkey.ed = 0;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �E�Ɖ��ɕǂ�����ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// ����������Ă�����A��ɕ�����ς���
			if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				Pinkey.ed = 2;				// ��ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// �ォ������Ă�����A���ɕ�����ς���
			else if (Pinkey.md == 3) {	// ���ɐi��ł�����
				Pinkey.ed = 0;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �ゾ���ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// �E�ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ��������ꍇ
		if (Pinkey.WallHit == true) {
			// �ォ������Ă�����
			if (Pinkey.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// �E�ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	//�@�E�����ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// ��ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// ��ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 2) {	// ��Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// ���Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �\���H�̏ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ������Ă��Ȃ�������
		if (Pinkey.WallHit == false)
		{		// �s���L�[���i��ł�������ɂ���ď��������߂�
			switch (Pinkey.md) {
			case 0:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 1:	// �E�ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 2:	// ��ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 3:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void ChaseMode() {
	//int ax, ay, bx, by, cx, cy;
	//ax = 2 * MAP_SIZE;
	//ay = 2 * MAP_SIZE;
	//bx = Pinkey.x;
	//by = Pinkey.y;
	//Pinkey.mx = Pinkey.x;		// �s���L�[��x���W��ۑ�
	//Pinkey.my = Pinkey.y;		// �s���L�[��y���W��ۑ�
	//Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�
	//for (int i = 0; i < MAP_HEIGHT; i++) {
	//	for (int j = 0; j < MAP_WIDTH; j++) {
	//		if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
	//			j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
	//			DrawLine(mPac.x, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
	//		}
	//	}
	//}
	//// �s���L�[���ǂ�����Ȃ���ړ����鏈��
	//switch (Pinkey.ed) {
	//case 0:	// ���ֈړ�
	//	Pinkey.x -= Pinkey.speed;
	//	Pinkey.eyeImageCount = 3;
	//	break;
	//case 1:	// �E�ֈړ�
	//	Pinkey.x += Pinkey.speed;
	//	Pinkey.eyeImageCount = 1;
	//	break;
	//case 2:	// ��ֈړ�
	//	Pinkey.y -= Pinkey.speed;
	//	Pinkey.eyeImageCount = 0;
	//	break;
	//case 3:	// ���ֈړ�
	//	Pinkey.y += Pinkey.speed;
	//	Pinkey.eyeImageCount = 2;
	//	break;
	//}
	//if (Pinkey.WallHit == true) {
	//	// �s���L�[�����ɐi��ł���ꍇ
	//	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
	//		if (Pinkey.ed == 0) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 2) {
	//			Pinkey.ed = 1;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
	//		// �v���C���[�̈ʒu���s���L�[��艺�Ȃ�
	//		if (mPac.y > Pinkey.y) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
	//		if (Pinkey.ed == 0) {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 3) {
	//			Pinkey.ed = 1;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	// �s���L�[���E�ɐi��ł���ꍇ
	//	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
	//		if (Pinkey.ed == 1) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 2) {
	//			Pinkey.ed = 0;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
	//		if (mPac.y > Pinkey.y) {
	//			Pinkey.ed = 3;
	//			Pinkey.WallHit = false;
	//		}
	//		else {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
	//		if (Pinkey.ed == 1) {
	//			Pinkey.ed = 2;
	//			Pinkey.WallHit = false;
	//		}
	//		else if (Pinkey.ed == 3) {
	//			Pinkey.ed = 0;
	//			Pinkey.WallHit = false;
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
	//		// �v���C���[�̈ʒu���s���L�[���E�Ȃ�
	//		if (mPac.x >= Pinkey.x) {
	//			if (Pinkey.ed == 2) {
	//				Pinkey.ed = 1;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//		else {
	//			if (Pinkey.ed == 2) {
	//				Pinkey.ed = 0;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//	}
	//	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
	//		// �v���C���[�̈ʒu���s���L�[���E�Ȃ�
	//		if (mPac.x >= Pinkey.x) {
	//			if (Pinkey.ed == 3) {
	//				Pinkey.ed = 1;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//		else {
	//			if (Pinkey.ed == 3) {
	//				Pinkey.ed = 0;
	//				Pinkey.WallHit = false;
	//			}
	//		}
	//	}
	//}

	Pinkey.mx = Pinkey.x;		// �s���L�[��x���W��ۑ�
	Pinkey.my = Pinkey.y;		// �s���L�[��y���W��ۑ�
	Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�

	Pinkey.mapX = (int)Pinkey.x / 16;
	Pinkey.mapY = (int)Pinkey.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// �O�����̒藝���g��
	A = mPac.x - Pinkey.x;

	B = mPac.y - Pinkey.y;

	C = sqrtf(A * A + B * B);	// A �� B ���Q�悵�đ������l�̕����������߂�

	dx = A / C;		// C ��1�i���K���j�Ƃ���ɂ́AA �� C �Ŋ���
	dy = B / C;		// C ��1�i���K���j�Ƃ���ɂ́AB �� C �Ŋ���

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(mPac.x, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));


			}
		}
	}
	// �s���L�[���ǂ�����Ȃ���ړ����鏈��
	switch (Pinkey.ed) {
	case 0:	// ���ֈړ�
		Pinkey.x--;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// �E�ֈړ�
		Pinkey.x++;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// ��ֈړ�
		Pinkey.y--;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// ���ֈړ�
		Pinkey.y++;
		Pinkey.eyeImageCount = 2;
		break;
	}
	// ���Ə�ɕǂ��������ꍇ
	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// �E��������Ă�����A���ɕ�����ς���
			if (Pinkey.md == 0) {
				Pinkey.ed = 3;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// ����������Ă�����A�E�ɕ�����ς���
			else if (Pinkey.md == 2) {
				Pinkey.ed = 1;				// �E�ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// ���Ɖ��ɕǂ���������
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// �E��������Ă�����A��ɕ�����ς���
			if (Pinkey.md == 0) {
				Pinkey.ed = 2;				// ��ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// �ォ������Ă�����A�E�ɕ�����ς���
			else if (Pinkey.md == 3) {
				Pinkey.ed = 1;				// �E�ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �E�Ə�ɕǂ�����ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// ����������Ă�����A���ɕ�����ς���
			if (Pinkey.md == 1) {
				Pinkey.ed = 3;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// ����������Ă�����A���ɕ�����ς���
			else if (Pinkey.md == 2) {	// ��ɐi��ł�����
				Pinkey.ed = 0;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �E�Ɖ��ɕǂ�����ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// ����������Ă�����A��ɕ�����ς���
			if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				Pinkey.ed = 2;				// ��ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// �ォ������Ă�����A���ɕ�����ς���
			else if (Pinkey.md == 3) {	// ���ɐi��ł�����
				Pinkey.ed = 0;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �ゾ���ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// �E�ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ��������ꍇ
		if (Pinkey.WallHit == true) {
			// �ォ������Ă�����
			if (Pinkey.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// �E�ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	//�@�E�����ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// ��ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// ��ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 2) {	// ��Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// ���Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �\���H�̏ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ������Ă��Ȃ�������
		if (Pinkey.WallHit == false)
		{		// �s���L�[���i��ł�������ɂ���ď��������߂�
			switch (Pinkey.md) {
			case 0:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 1:	// �E�ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 2:	// ��ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 3:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void IjikeMode() {


	

	Pinkey.mx = Pinkey.x;		// �s���L�[��x���W��ۑ�
	Pinkey.my = Pinkey.y;		// �s���L�[��y���W��ۑ�
	Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�

	Pinkey.mapX = (int)Pinkey.x / 16;
	Pinkey.mapY = (int)Pinkey.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// �O�����̒藝���g��
	A = mPac.x - Pinkey.x;

	B = mPac.y - Pinkey.y;

	C = sqrtf(A * A + B * B);	// A �� B ���Q�悵�đ������l�̕����������߂�

	dx = A / C;		// C ��1�i���K���j�Ƃ���ɂ́AA �� C �Ŋ���
	dy = B / C;		// C ��1�i���K���j�Ƃ���ɂ́AB �� C �Ŋ���
	// �s���L�[���ǂ�����Ȃ���ړ����鏈��
	if (IjikeCnt != 0) {
		IjikeCnt--;
		switch (Pinkey.ed) {
		case 0:	// ���ֈړ�
			Pinkey.x++;
			Pinkey.eyeImageCount = 3;
			break;
		case 1:	// �E�ֈړ�
			Pinkey.x--;
			Pinkey.eyeImageCount = 1;
			break;
		case 2:	// ��ֈړ�
			Pinkey.y++;
			Pinkey.eyeImageCount = 0;
			break;
		case 3:	// ���ֈړ�
			Pinkey.y--;
			Pinkey.eyeImageCount = 2;
			break;
		}
	}
	else {
		switch (Pinkey.ed) {
		case 0:	// ���ֈړ�
			Pinkey.x--;
			Pinkey.eyeImageCount = 3;
			break;
		case 1:	// �E�ֈړ�
			Pinkey.x++;
			Pinkey.eyeImageCount = 1;
			break;
		case 2:	// ��ֈړ�
			Pinkey.y--;
			Pinkey.eyeImageCount = 0;
			break;
		case 3:	// ���ֈړ�
			Pinkey.y++;
			Pinkey.eyeImageCount = 2;
			break;
		}
	}
	// ���Ə�ɕǂ��������ꍇ
	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// �E��������Ă�����A���ɕ�����ς���
			if (Pinkey.md == 0) {
				Pinkey.ed = 3;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// ����������Ă�����A�E�ɕ�����ς���
			else if (Pinkey.md == 2) {
				Pinkey.ed = 1;				// �E�ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// ���Ɖ��ɕǂ���������
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// �E��������Ă�����A��ɕ�����ς���
			if (Pinkey.md == 0) {
				Pinkey.ed = 2;				// ��ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// �ォ������Ă�����A�E�ɕ�����ς���
			else if (Pinkey.md == 3) {
				Pinkey.ed = 1;				// �E�ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �E�Ə�ɕǂ�����ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// ����������Ă�����A���ɕ�����ς���
			if (Pinkey.md == 1) {
				Pinkey.ed = 3;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// ����������Ă�����A���ɕ�����ς���
			else if (Pinkey.md == 2) {	// ��ɐi��ł�����
				Pinkey.ed = 0;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �E�Ɖ��ɕǂ�����ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			// ����������Ă�����A��ɕ�����ς���
			if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				Pinkey.ed = 2;				// ��ɕ�����ς���
				Pinkey.WallHit = false;
			}
			// �ォ������Ă�����A���ɕ�����ς���
			else if (Pinkey.md == 3) {	// ���ɐi��ł�����
				Pinkey.ed = 0;				// ���ɕ�����ς���
				Pinkey.WallHit = false;
			}
		}
	}
	// �ゾ���ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// �E�ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// �ǂɓ��������ꍇ
		if (Pinkey.WallHit == true) {
			// �ォ������Ă�����
			if (Pinkey.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// �E�ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
				if (dy < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	//�@�E�����ɕǂ��������ꍇ
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 1) {	// �E�ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// ��ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 2) {	// ��ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
				if (dx < 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ���������
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 0) {	// ���ɐi��ł�����
				// �p�b�N�}���̈ʒu���s���L�[��艺�Ȃ�
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// ���ɕ�����ς���
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// ��ɕ�����ς���
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// �ǂɓ������Ă��Ȃ�������
			if (Pinkey.md == 2) {	// ��Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// ���Ɉړ����Ă�����
				// �p�b�N�}���̈ʒu���s���L�[���E�Ȃ�
				if (dx > 0.0f) {
					// �ǂɈ��������炸�ɋȂ����悤��
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// �\���H�̏ꍇ
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// �ǂɓ������Ă��Ȃ�������
		if (Pinkey.WallHit == false)
		{		// �s���L�[���i��ł�������ɂ���ď��������߂�
			switch (Pinkey.md) {
			case 0:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 1:	// �E�ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[����Ȃ�
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// ��ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 2:	// ��ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			case 3:	// ���ɐi��ł�����
				// �ǂɈ��������炸�ɋȂ����悤��
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
					// �p�b�N�}���̈ʒu���s���L�[��荶�Ȃ�
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// ���ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// �E�ɕ�����ς���
						Pinkey.WallHit = false;
						break;
					}
				}
			}
		}
	}
}
void GuzutaMove() {
	Guzuta.mx = Guzuta.x;		// �O�Y�^��x���W��ۑ�
	Guzuta.my = Guzuta.y;		// �O�Y�^��y���W��ۑ�
	Guzuta.md = Guzuta.ed;		// �O�Y�^�̓���������ۑ�



	// �O�Y�^���ǂ�����Ȃ���ړ����鏈��
	switch (Guzuta.ed) {
	case 0:	// ���ֈړ�
		Guzuta.x--;
		Guzuta.eyeImageCount = 3;
		break;
	case 1:	// �E�ֈړ�
		Guzuta.x++;
		Guzuta.eyeImageCount = 1;
		break;
	case 2:	// ��ֈړ�
		Guzuta.y--;
		Guzuta.eyeImageCount = 0;
		break;
	case 3:	// ���ֈړ�
		Guzuta.y++;
		Guzuta.eyeImageCount = 2;
		break;
	}

	if (DotCnt < 90) {
		if (Guzuta.WallHit == true) {
			if (Guzuta.ed == 2) {
				Guzuta.ed = 3;
				Guzuta.WallHit = false;
			}
			else if (Guzuta.ed == 3) {
				Guzuta.ed = 2;
				Guzuta.WallHit = false;
			}
		}
	}
	else {
		// MapDtaa[15][15]��ڎw���āAMapData[12][15]��ڎw��
		if (Guzuta.x / 16 != 15.0f) {
			Guzuta.ed = 0;
		}
		else {
			Guzuta.ed = 2;
		}
	}
}

void AosukeMove() {
	Aosuke.mx = Aosuke.x;		// �A�I�X�P��x���W��ۑ�
	Aosuke.my = Aosuke.y;		// �A�I�X�P��y���W��ۑ�
	Aosuke.md = Aosuke.ed;		// �A�I�X�P�̓���������ۑ�



	// �A�I�X�P���ǂ�����Ȃ���ړ����鏈��
	switch (Aosuke.ed) {
	case 0:	// ���ֈړ�
		Aosuke.x--;
		Aosuke.eyeImageCount = 3;
		break;
	case 1:	// �E�ֈړ�
		Aosuke.x++;
		Aosuke.eyeImageCount = 1;
		break;
	case 2:	// ��ֈړ�
		Aosuke.y--;
		Aosuke.eyeImageCount = 0;
		break;
	case 3:	// ���ֈړ�
		Aosuke.y++;
		Aosuke.eyeImageCount = 2;
		break;
	}

	if (DotCnt < 30) {
		if (Aosuke.WallHit == true) {
			if (Aosuke.ed == 2) {
				Aosuke.ed = 3;
				Aosuke.WallHit = false;
			}
			else if (Aosuke.ed == 3) {
				Aosuke.ed = 2;
				Aosuke.WallHit = false;
			}
		}
	}
	else {
		// MapDtaa[15][15]��ڎw���āAMapData[12][15]��ڎw��
		if (Aosuke.x / 16 != 15.0f) {
			Aosuke.ed = 1;
		}
		else {
			Aosuke.ed = 2;
		}
	}
}