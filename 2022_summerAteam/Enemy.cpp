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

	DrawFormatString(500, 30, GetColor(255, 255, 255),
		EnemyMode ? "Scatter %d" : "Chase %d", EnemyMode ? ScatterModeTime : ChaseModeTime);


	// �A�j���[�V����
	if (!PowerUpFlg) {

		if (Akabei.ImageCount == 0) {
			Akabei.ImageCount = 1;

		}
		else {
			Akabei.ImageCount = 0;
		}

		if (eCnt < ENEMY_CNT_SPEED) {
			eCnt++;
		}
		else if (eCnt == ENEMY_CNT_SPEED) {
			eCnt = 0;
		}
		if (eCnt < ENEMY_CNT_SPEED / 2) {
			Pinkey.ImageCount = 3;
		}
		else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
			Pinkey.ImageCount = 2;
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

	ModeChange();

	AkabeiMove2();
}


// �`��
void Enemy_Draw() {

	if (!PowerUpFlg) {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);
		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, EyeImage[Pinkey.eyeImageCount], TRUE);
	}
	else {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);
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
		ChaseModeTime = 1230;
		if (ScatterModeTime > 0 != PowerUpFlg) {
			ScatterMode();
			ScatterModeTime--;
		}
		else if (PowerUpFlg == true) {
			ScatterMode();
		}
		else if (ScatterModeTime == 0) {
			EnemyMode = false;
		}
	}
	else if (EnemyMode == false) {//�ǐՃ��[�h
		ScatterModeTime = 480;
		if (ChaseModeTime > 0 != PowerUpFlg) {
			ChaseMode();
			//ScatterMode();
			ChaseModeTime--;
		}
		else if (PowerUpFlg == true) {
			ChaseMode();
			//ScatterMode();
		}
		else if (ChaseModeTime == 0) {
			EnemyMode = true;
		}
	}
}

void ScatterMode() {

	int ax, ay, bx, by, cx, cy;
	int difx, dify, difc;
	int goal;
	ax = 2 * MAP_SIZE;
	ay = 2 * MAP_SIZE;

	bx = Pinkey.x;
	by = Pinkey.y;

	Pinkey.mx = Pinkey.x;		// �A�J�x�C��x���W��ۑ�
	Pinkey.my = Pinkey.y;		// �A�J�x�C��y���W��ۑ�
	Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(2 * MAP_SIZE + 4, 2 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));


			}
		}
	}
	 //�s���L�[���ǂ�����Ȃ���ړ����鏈��
	switch (Pinkey.ed) {
	case 0:	// ���ֈړ�
		Pinkey.x -= Pinkey.speed;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// �E�ֈړ�
		Pinkey.x += Pinkey.speed;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// ��ֈړ�
		Pinkey.y -= Pinkey.speed;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// ���ֈړ�
		Pinkey.y += Pinkey.speed;
		Pinkey.eyeImageCount = 2;
		break;
	}
	if (Pinkey.WallHit == true) {//�ǂɓ���������
		if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			if (Pinkey.ed == 0) {// �s���L�[�����ɐi��ł���ꍇ
				Pinkey.ed = 3;//���ֈړ�
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 2) {//��ֈړ����Ă���ꍇ
				Pinkey.ed = 1;//�E�ֈړ�
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
			// �꒣��̈ʒu���s���L�[��艺�Ȃ�
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
		// �s���L�[�E�ɐi��ł���ꍇ
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
			//�꒣��̈ʒu���s���L�[����Ȃ�
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
			// �꒣��̈ʒu���s���L�[���E�Ȃ�
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
			// �꒣��̈ʒu���s���L�[���E�Ȃ�
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

void ChaseMode() {
	int ax, ay, bx, by, cx, cy;

	ax = 2 * MAP_SIZE;
	ay = 2 * MAP_SIZE;

	bx = Pinkey.x;
	by = Pinkey.y;


	Pinkey.mx = Pinkey.x;		// �A�J�x�C��x���W��ۑ�
	Pinkey.my = Pinkey.y;		// �A�J�x�C��y���W��ۑ�
	Pinkey.md = Pinkey.ed;		// �G�̓���������ۑ�


	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(mPac.x, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
			}
		}
	}


	// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
	switch (Pinkey.ed) {
	case 0:	// ���ֈړ�
		Pinkey.x -= Pinkey.speed;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// �E�ֈړ�
		Pinkey.x += Pinkey.speed;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// ��ֈړ�
		Pinkey.y -= Pinkey.speed;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// ���ֈړ�
		Pinkey.y += Pinkey.speed;
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