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

int pup, pbottom, pleft, pright;
int px, py, px2, py2;


float A, B, C;		// �O�����̒藝�p�̕ϐ�
float dx, dy;		// ���K���p�ϐ�

int eCnt;

int ScatterModeTime;//�꒣�胂�[�h�̎��ԁi�t���[���j
int ChaseModeTime;//�ǐՃ��[�h�̎��ԁi�t���[���j
bool EnemyMode; // True = �꒣�胂�[�h�Afalse = �ǐՃ��[�h

// ������
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // �����X�^�[�̉摜��ǂݍ���
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // �ڋʂ̉摜��ǂݍ���

	// �A�J�x�C�̏�����
	Akabei.x = 240.0f;
	Akabei.y = 200.0f;
	Akabei.w = ENEMY_SIZE;
	Akabei.h = ENEMY_SIZE;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;
	Akabei.WallHit = false;
	Akabei.juuji = false;
	Akabei.juujiUp = false;
	Akabei.juujiLeft = false;
	Akabei.juujiDown = false;
	Akabei.juujiRight = false;
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

	Pinkey.mx = Pinkey.x;
	Pinkey.my = Pinkey.y;

	// �f�o�b�O�p�̕ϐ��̕\��
	DrawFormatString(1000, 30, 255, "A = %.1f", A);
	DrawFormatString(1000, 50, 255, "B = %.1f", B);
	DrawFormatString(1000, 70, 255, "C = %.1f", C);
	DrawFormatString(1000, 90, 255, "dx = %.1f", dx);
	DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	DrawFormatString(1000, 150, 255, "ed = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Aosuke.x = %.1f", Aosuke.x / 16);
	DrawFormatString(1000, 190, 255, "Guzuta.x = %.1f", Guzuta.x / 16);
	DrawFormatString(1000, 210, 255, "mPac.x = %.1f", mPac.x / 16);
	DrawFormatString(1000, 230, 255, "mPac.y = %.1f", mPac.y / 16);
	DrawFormatString(1000, 250, 255, "Akabei.WallHit = %d", Akabei.WallHit);
	DrawFormatString(1000, 270, 255, "Akabei.left = %d", Akabei.left);
	DrawFormatString(1000, 290, 255, "Akabei.right = %d", Akabei.right);
	DrawFormatString(1000, 310, 255, "Akabei.up = %d", Akabei.up);
	DrawFormatString(1000, 330, 255, "Akabei.bottom = %d", Akabei.bottom);
	DrawFormatString(1000, 350, 255, "DotCnt = %d", DotCnt);
	//DrawFormatString(1000, 370, 255, "Akabei.juuji = %d", Akabei.juuji);


	DrawFormatString(500, 30, GetColor(255, 255, 255),
		EnemyMode ? "Scatter %d" : "Chase %d", EnemyMode ? ScatterModeTime : ChaseModeTime);

	ModeChange();

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

	//AkabeiChasePlayer();		// �A�J�x�C�����v���C���[��ǂ������鏈��
	AkabeiMove2();
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

// ���E�㉺�̓����蔻���ǂ������������������f���Đi�ޕ��������߂鏈��
// �r���̒ʘH�ɂ͓��炸�ɕǂɓ�����i��
void AkabeiMove() {
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
		else {
			// �E�ɕǂ������āA���Ə㉺�ɕǂ��������ɂǂ̕����Ɉړ����邩
			if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
				// �i��ł��������ɂ���āA������ς���
				switch (Akabei.ed) {
				case 1:	// �E
					if (mPac.y > Akabei.y) {
						Akabei.ed = 3;
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 2;
						Akabei.WallHit = false;
						break;
					}
				case 2:
					if (mPac.x < Akabei.x) {
						Akabei.ed = 0;
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;
						Akabei.WallHit = false;
						break;
					}
				case 3:
					if (mPac.x < Akabei.x) {
						Akabei.ed = 0;
						Akabei.WallHit = false;
						break;
					}
				}
			}
			// ���ɕǂ������āA�E�Ə㉺�ɕǂ��������ɂǂ̕����Ɉړ����邩
			else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
				switch (Akabei.ed) {
				case 1:
					if (mPac.y < Akabei.y) {
						Akabei.ed = 2;
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;
						Akabei.WallHit = false;
					}
				case 2:
					if (mPac.x > Akabei.x) {
						Akabei.ed = 1;
						Akabei.WallHit = false;
						break;
					}
				case 3:
					if (mPac.x > Akabei.x) {
						Akabei.ed = 1;
						Akabei.WallHit = false;
						break;
					}
				}
			}
			// ��ɕǂ������āA���ƍ��E�ɕǂ��������ɂǂ̕����Ɉړ����邩
			else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
				switch (Akabei.ed) {
				case 0:
					if (mPac.y < Akabei.y) {
						Akabei.ed = 2;
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;
						Akabei.WallHit = false;
						break;
					}
				case 1:
					if (Akabei.y > Akabei.y) {
						Akabei.ed = 3;
						Akabei.WallHit = false;
						break;
					}
				case 2:
					if (mPac.x < Akabei.x) {
						Akabei.ed = 0;
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;
						Akabei.WallHit = false;
						break;
					}
				}
			}
			// ���ɕǂ������āA��ƍ��E�ɕǂ��������ɂǂ̕����Ɉړ����邩
			else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
				switch (Akabei.ed) {
				case 0:
					if (mPac.y < Akabei.y) {
						Akabei.ed = 2;
						Akabei.WallHit = false;
						break;
					}
				case 1:
					if (mPac.y < Akabei.y) {
						Akabei.ed = 2;
						Akabei.WallHit = false;
						break;
					}
				case 3:
					if (mPac.x < Akabei.x) {
						Akabei.ed = 0;
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;
						Akabei.WallHit = false;
						break;
					}
				}

			}
		}
	}
	// ���E�㉺�ǂ��ɂ��ǂ��������̈ړ�����
	else {
		if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
			switch (Akabei.ed) {
			case 0:
				if (mPac.y < Akabei.y) {
					Akabei.ed = 2;
					Akabei.WallHit = false;
					break;
				}
				else {
					Akabei.ed = 3;
					Akabei.WallHit = false;
					break;
				}
			case 1:
				if (mPac.y < Akabei.y) {
					Akabei.ed = 2;
					Akabei.WallHit = false;
					break;
				}
				else {
					Akabei.ed = 3;
					Akabei.WallHit = false;
					break;
				}
			case 2:
				if (mPac.x > Akabei.x) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
					break;
				}
				else {
					Akabei.ed = 0;
					Akabei.WallHit = false;
					break;
				}
			case 3:
				if (mPac.x > Akabei.x) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
					break;
				}
				else {
					Akabei.ed = 0;
					Akabei.WallHit = false;
					break;
				}
			}
			//if (Akabei.ed == 0 || Akabei.ed == 1) {
			//	if (mPac.y < Akabei.y) {
			//		Akabei.ed = 2;
			//	}
			//	else {
			//		Akabei.ed = 3;
			//	}
			//}
			//else if (Akabei.ed == 2 || Akabei.ed == 3) {
			//	if (mPac.x > Akabei.x) {
			//		Akabei.ed = 1;
			//	}
			//	else {
			//		Akabei.ed = 0;
			//	}
			//}
		}


	}
}

void AkabeiMove2() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�
	Akabei.md = Akabei.ed;		// �G�̓���������ۑ�

	Akabei.mapX = (int)Akabei.x / 16;
	Akabei.mapY = (int)Akabei.y / 16;

	DrawLine(mPac.x, mPac.y, Akabei.x, Akabei.y, GetColor(255, 0, 0));


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

	// ���Ə�ɕǂ��������ꍇ
	if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// �E��������Ă�����A���ɕ�����ς���
			if (Akabei.md == 0) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			// ����������Ă�����A�E�ɕ�����ς���
			else if (Akabei.md == 2) {
				Akabei.ed = 1;
				Akabei.WallHit = false;
			}
		}
	}
	// ���Ɖ��ɕǂ���������
	if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// �E��������Ă�����A��ɕ�����ς���
			if (Akabei.md == 0) {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
			// �ォ������Ă�����A�E�ɕ�����ς���
			else if (Akabei.md == 3) {
				Akabei.ed = 1;
				Akabei.WallHit = false;
			}
		}
	}
	// �E�Ə�ɕǂ�����ꍇ
	if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// ����������Ă�����A���ɕ�����ς���
			if (Akabei.md == 1) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			// ����������Ă�����A���ɕ�����ς���
			else if (Akabei.md == 2) {
				Akabei.ed = 0;
				Akabei.WallHit = false;
			}
		}
	}
	// �E�Ɖ��ɕǂ�����ꍇ
	if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
		// �ǂɂ���������
		if (Akabei.WallHit == true) {
			// ����������Ă�����A��ɕ�����ς���
			if (Akabei.md == 1) {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
			// �ォ������Ă�����A���ɕ�����ς���
			else if (Akabei.md == 3) {
				Akabei.ed = 0;
				Akabei.WallHit = false;
			}
		}

	}
	// �ゾ���ɕǂ��������ꍇ
	if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
			// ����������Ă�����
			if (Akabei.md == 2) {
				// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
				if (mPac.x >= Akabei.x) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;
					Akabei.WallHit = false;
				}
			}
		}
	}
	// �������ɕǂ��������ꍇ
	if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// �ǂɓ��������ꍇ
		if (Akabei.WallHit == true) {
			// �ォ������Ă�����
			if (Akabei.md == 3) {
				// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
				if (mPac.x >= Akabei.x) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;
					Akabei.WallHit = false;
				}
			}
		}
	}
	//�@�E�����ɕǂ��������ꍇ
	if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
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
	}
	// �������ɕǂ��������ꍇ
	if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// �ǂɓ���������
		if (Akabei.WallHit == true) {
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
	}


	if (Akabei.WallHit == false) {
		if (Akabei.juujiUp == true) {
			if (Akabei.md == 2) {
				if (mPac.x < Akabei.x) {
					Akabei.ed = 0;
					Akabei.WallHit = false;
					Akabei.juujiUp = false;
					Akabei.juuji = false;
				}
				else {
					Akabei.ed = 1;
					Akabei.WallHit = false;
					Akabei.juujiUp = false;
					Akabei.juuji = false;
				}
			}
		}
		else if (Akabei.juujiLeft == true) {
			if (Akabei.md == 0) {
				if (mPac.y < Akabei.y) {
					Akabei.ed = 2;
					Akabei.WallHit = false;
					Akabei.juujiLeft == false;
					Akabei.juuji = false;
				}
				else {
					Akabei.ed = 3;
					Akabei.WallHit = false;
					Akabei.juujiLeft = false;
					Akabei.juuji = false;
				}
			}
		}
		else if (Akabei.juujiDown == true) {
			if (Akabei.md == 3) {
				if (mPac.x < Akabei.x) {
					Akabei.ed = 0;
					Akabei.WallHit = false;
					Akabei.juujiDown = false;
					Akabei.juuji = false;
				}
				else {
					Akabei.ed = 1;
					Akabei.WallHit = false;
					Akabei.juujiDown = false;
					Akabei.juuji = false;
				}
			}
		}
		else if (Akabei.juujiRight == true) {
			if (Akabei.md == 1) {
				if (mPac.y < Akabei.y) {
					Akabei.ed = 2;
					Akabei.WallHit = false;
					Akabei.juujiRight = false;
					Akabei.juuji = false;
				}
				else {
					Akabei.ed = 3;
					Akabei.WallHit = false;
					Akabei.juujiRight = false;
					Akabei.juuji = false;
				}
			}
		}
	}
}

void PinkeyMove() {

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

void ModeChange() {
	if (EnemyMode == true) {//�꒣�胂�[�h
		ChaseModeTime = 1230;
		if (ScatterModeTime > 0 != PowerUpFlg) {
			ScatterMode();
			ScatterModeTime--;
		}
		else if (ScatterModeTime == 0) {
			EnemyMode = false;
		}
	}
	else if (EnemyMode == false) {//�ǐՃ��[�h
		ScatterModeTime = 480;
		if (ChaseModeTime > 0 != PowerUpFlg) {
			ChaseMode();
			ChaseModeTime--;
		}
		else if (ChaseModeTime == 0) {
			EnemyMode = true;
		}
	}
}

void ScatterMode() {

	int ax, ay, bx, by, cx, cy;

	ax = 2 * MAP_SIZE;
	ay = 2 * MAP_SIZE;

	bx = Pinkey.x;
	by = Pinkey.y;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, Pinkey.w, Pinkey.h,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(2 * MAP_SIZE + 4, 2 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));

				Pinkey.mx = Pinkey.x;		// �A�J�x�C��x���W��ۑ�
				Pinkey.my = Pinkey.y;		// �A�J�x�C��y���W��ۑ�
				Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�

				// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
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
				if (Pinkey.WallHit == true) {
					// �A�J�x�C�����ɐi��ł���ꍇ
					if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
						if (Pinkey.ed == 0) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 2) {
							Pinkey.ed = 1;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
						// �v���C���[�̈ʒu���A�J�x�C��艺�Ȃ�
						if (ay > Pinkey.y) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
						if (Pinkey.ed == 0) {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 3) {
							Pinkey.ed = 1;
							Pinkey.WallHit = false;
						}
					}

					// �A�J�x�C���E�ɐi��ł���ꍇ
					else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
						if (Pinkey.ed == 1) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 2) {
							Pinkey.ed = 0;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
						if (ay > Pinkey.y) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
						if (Pinkey.ed == 1) {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 3) {
							Pinkey.ed = 0;
							Pinkey.WallHit = false;
						}

					}
					else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
						// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
						if (mPac.x >= Pinkey.x) {
							if (Pinkey.ed == 2) {
								Pinkey.ed = 1;
								Pinkey.WallHit = false;
							}
						}
						else {
							if (Pinkey.ed == 2) {
								Pinkey.ed = 0;
								Pinkey.WallHit = false;
							}
						}
					}
					else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
						// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
						if (mPac.x >= Pinkey.x) {
							if (Pinkey.ed == 3) {
								Pinkey.ed = 1;
								Pinkey.WallHit = false;
							}
						}
						else {
							if (Pinkey.ed == 3) {
								Pinkey.ed = 0;
								Pinkey.WallHit = false;
							}
						}
					}
				}
			}
		}
	}
	/*// ���ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[(int)Pinkey.y / 16][((int)Pinkey.x / 16) - 1] == 1) {
		Pinkey.left = true;
	}
	else {
		Pinkey.left = false;
	}

	// �E�ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[(int)Pinkey.y / 16][((int)Pinkey.x / 16) + 1] == 1) {
		//DrawBox(Pinkey.x / 16, Pinkey.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Pinkey.right = true;
	}
	else {
		Pinkey.right = false;
	}

	// ��ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[((int)Pinkey.y / 16) - 1][(int)Pinkey.x / 16] == 1) {
		Pinkey.up = true;
	}
	else {
		Pinkey.up = false;
	}

	// ���ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[((int)Pinkey.y / 16) + 1][(int)Pinkey.x / 16] == 1) {
		Pinkey.bottom = true;
	}
	else {
		Pinkey.bottom = false;
	}*/
}

void ChaseMode() {
	int ax, ay, bx, by, cx, cy;

	ax = 2 * MAP_SIZE;
	ay = 2 * MAP_SIZE;

	bx = Pinkey.x;
	by = Pinkey.y;

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, Pinkey.w, Pinkey.h,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(mPac.x, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));

				Pinkey.mx = Pinkey.x;		// �A�J�x�C��x���W��ۑ�
				Pinkey.my = Pinkey.y;		// �A�J�x�C��y���W��ۑ�
				Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�

				// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
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
				if (Pinkey.WallHit == true) {
					// �A�J�x�C�����ɐi��ł���ꍇ
					if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
						if (Pinkey.ed == 0) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 2) {
							Pinkey.ed = 1;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
						// �v���C���[�̈ʒu���A�J�x�C��艺�Ȃ�
						if (mPac.y > Pinkey.y) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
						if (Pinkey.ed == 0) {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 3) {
							Pinkey.ed = 1;
							Pinkey.WallHit = false;
						}
					}

					// �A�J�x�C���E�ɐi��ł���ꍇ
					else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
						if (Pinkey.ed == 1) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 2) {
							Pinkey.ed = 0;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
						if (mPac.y > Pinkey.y) {
							Pinkey.ed = 3;
							Pinkey.WallHit = false;
						}
						else {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
					}
					else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
						if (Pinkey.ed == 1) {
							Pinkey.ed = 2;
							Pinkey.WallHit = false;
						}
						else if (Pinkey.ed == 3) {
							Pinkey.ed = 0;
							Pinkey.WallHit = false;
						}

					}
					else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
						// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
						if (ax >= Pinkey.x) {
							if (Pinkey.ed == 2) {
								Pinkey.ed = 1;
								Pinkey.WallHit = false;
							}
						}
						else {
							if (Pinkey.ed == 2) {
								Pinkey.ed = 0;
								Pinkey.WallHit = false;
							}
						}
					}
					else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
						// �v���C���[�̈ʒu���A�J�x�C���E�Ȃ�
						if (ax >= Pinkey.x) {
							if (Pinkey.ed == 3) {
								Pinkey.ed = 1;
								Pinkey.WallHit = false;
							}
						}
						else {
							if (Pinkey.ed == 3) {
								Pinkey.ed = 0;
								Pinkey.WallHit = false;
							}
						}
					}
				}
			}
		}
	}
	/*// ���ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[(int)Pinkey.y / 16][((int)Pinkey.x / 16) - 1] == 1) {
		Pinkey.left = true;
	}
	else {
		Pinkey.left = false;
	}

	// �E�ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[(int)Pinkey.y / 16][((int)Pinkey.x / 16) + 1] == 1) {
		//DrawBox(Pinkey.x / 16, Pinkey.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Pinkey.right = true;
	}
	else {
		Pinkey.right = false;
	}

	// ��ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[((int)Pinkey.y / 16) - 1][(int)Pinkey.x / 16] == 1) {
		Pinkey.up = true;
	}
	else {
		Pinkey.up = false;
	}

	// ���ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[((int)Pinkey.y / 16) + 1][(int)Pinkey.x / 16] == 1) {
		Pinkey.bottom = true;
	}
	else {
		Pinkey.bottom = false;
	}*/
}