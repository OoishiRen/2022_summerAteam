#include <math.h>

#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Game.h"
#include "Input.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "UI.h"

extern float HitCheckEnemy(PAC* p, AKABEI* e);

/*************************
** �ϐ� **
*************************/
static int pac_image[PAC_IMAGE_MAX];		//�p�b�N�}���摜�i�[�p
static int dying_image[DYING_IMAGE_MAX];	//�~�X���̉摜�i�[�p

float count = 0;							//�t���[���J�E���g�p
bool respawn = false;						//���X�|�[���p�ϐ�

/*************************
** �\���� **
*************************/
struct PAC mPac;							//�p�b�N�}���\����

int EnemyCntTime;
bool EnemyScoreUI;
int UIx, UIy;
int ESC;

//������
void Player_Initialize() {
	//�摜�ǂݍ���
	LoadDivGraph("player_images/pacman.png", 12, 12, 1, 16, 16, pac_image);
	LoadDivGraph("player_images/dying.png", 11, 11, 1, 16, 16, dying_image);

	//�\���̂̏�����
	mPac.flg = true;
	mPac.type = 3;
	mPac.var = 3;
	mPac.img = 3;
	mPac.x = 240.0f;
	mPac.y = 392.0f;
	mPac.w = PAC_WIDTH;
	mPac.h = PAC_HEIGHT;
	mPac.speed = PAC_SPEED;
	mPac.cnt = PAC_LIFE;

	//�J�E���g������
	count = 0;


	EnemyCntTime = 120;
	EnemyScoreUI = false;
}

//�I������
void Player_Finalize() {

	for (int i = 0;i < PAC_IMAGE_MAX;i++) {
		DeleteGraph(pac_image[i]);
	}
	for (int i = 0; i < DYING_IMAGE_MAX; i++) {
		DeleteGraph(pac_image[i]);
	}
}

//�X�V
void Player_Update() {

	//�����
	if (g_NowKey & PAD_INPUT_UP && mPac.type != 0 && !pac_y0) {
		mPac.var = 0;	//���O����(��)
		/*mPac.type = 0;
		mPac.img = (3 * mPac.type);*/
	}
	//�E����
	if (g_NowKey & PAD_INPUT_RIGHT && mPac.type != 1 && !pac_x1) {
		mPac.var = 1;	//���O����(�E)
		/*mPac.type = 1;
		mPac.img = (3 * mPac.type);*/
	}
	//������
	if (g_NowKey & PAD_INPUT_DOWN && mPac.type != 2 && !pac_y1) {
		mPac.var = 2;	//���O����(��)
		/*mPac.type = 2;
		mPac.img = (3 * mPac.type);*/
	}
	//������
	if (g_NowKey & PAD_INPUT_LEFT && mPac.type != 3 && !pac_x0) {
		mPac.var = 3;	//���O����(��)
		/*mPac.type = 3;
		mPac.img = (3 * mPac.type);*/
	}

	if (!Run)
	{
		// ������Ă��Ȃ�
		if (ESC > 0)
			ESC = -1;		// ESC�L�[�����ꂽ�u��
		else
			ESC = 0;		// ESC�L�[������Ă�����
	}
	else
	{
		// ������Ă���
		ESC++;				// ESC�L�[��������Ă���Ԃ͒l�𑝂₵������
	}
	if (ESC == 1) {
		Score += 200;
		/*UIx = mPac.x;
		UIy = mPac.y;*/
	}
	

	//�����蔻��(�΃A�J�x�C)
	if (HitCheckEnemy(&mPac, &Akabei)) {
		mPac.flg = false;
	}
	//�����蔻��(�΃s���L�[)
	/*if (PowerUpFlg == false) {
		if (HitCheckEnemy(&mPac, &Pinkey)) {
			mPac.flg = false;
		}
	}
	else if (PowerUpFlg == true) {
		if (HitCheckEnemy(&mPac, &Pinkey)) {
			EnemyScoreUI = true;
			UIx = mPac.x;
			UIy = mPac.y;
		}
	}*/
	if (HitCheckEnemy(&mPac, &Pinkey)) {
		if (PowerUpFlg == false) {
			mPac.flg = false;
		}
		else if (Run == false) {
			EnemyScoreUI = true;
			Run = true;
			//Score += 200;
			UIx = mPac.x;
			UIy = mPac.y;
		}
	}
	if (HitCheckEnemy(&mPac, &Aosuke)) {
		if (PowerUpFlg == false) {
			mPac.flg = false;
		}
		else if (Run == false) {
			EnemyScoreUI = true;
			Run = true;
			//Score += 200;
			UIx = mPac.x;
			UIy = mPac.y;
		}
	}
	if (HitCheckEnemy(&mPac, &Guzuta)) {
		if (PowerUpFlg == false) {
			mPac.flg = false;
		}
		else if (Run == false) {
			EnemyScoreUI = true;
			Run = true;
			//Score += 200;
			UIx = mPac.x;
			UIy = mPac.y;
		}
	}


	Pac_Anim(mPac.speed);	//�p�b�N�}���̃A�j���[�V����
	Pac_Move(mPac.type);	//�p�b�N�}���̓���

	//�~�X�������̃A�j���[�V�����I����̃��X�|�[��
	if (respawn) {
		respawn = false;
		mPac.flg = true;
		mPac.type = 3;
		mPac.var = 3;
		mPac.img = 3;
		mPac.speed = PAC_SPEED;
		mPac.x = 240.0f;
		mPac.y = 392.0f;
	}

}

void EnemyScoreUIEnabled() {
	if (EnemyCntTime > 0) {
		EnemyCntTime--;//�Q�b�Ԃ̊�
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				//�X�R�AUI�\��
				DrawFormatString(UIx - 8, UIy - 8, GetColor(255, 255, 255), "200");//�Ńo�b�O
			}
		}

	}
	else if (EnemyCntTime == 0) {
		EnemyScoreUI = false;
		EnemyCntTime = 120;
	}
}

//�`��
void Player_Draw() {
	//�p�b�N�}���̕\��
	if (mPac.flg) {	//�Q�[����
		DrawRotaGraph(mPac.x, mPac.y, 1.0f, 0, pac_image[mPac.img], TRUE, FALSE);
	}
	else {			//�~�X��
		DrawRotaGraph(mPac.x, mPac.y, 1.0f, 0, dying_image[mPac.img], TRUE, FALSE);
	}

	DrawFormatString(1000, 400, GetColor(255, 255, 255), "mPac.var:%d", mPac.var);		//�f�o�b�O�p
	DrawFormatString(1000, 420, GetColor(255, 255, 255), "mPac.type:%d", mPac.type);	//�f�o�b�O�p
}

/*************************************
** �p�b�N�}���̃A�j���[�V���� **
* ���@�� :�p�b�N�}���̓����X�s�[�h
* �߂�l :����
**************************************/
void Pac_Anim(float val) {

	//1�t���[��1�J�E���g
	count += 1.0f;

	if (mPac.flg) {		//�����c���Ă���ꍇ
		if (count / val == 0.75f) {
			count = 0;
			if (mPac.img < (3 * (mPac.type + 1)) - 1) {
				mPac.img++;
			}
			else {
				mPac.img = (3 * mPac.type);
			}
		}
	}
	else {			//�~�X�����ꍇ
		mPac.speed = 0.0f;	//�p�b�N�}���̓������~

		if (count / (PAC_SPEED * 2) == 0.75f) {
			count = 0;
			if (mPac.img < 11) {
				mPac.img++;
			}
			else if(mPac.cnt > 0){
				respawn = true;
				mPac.cnt--;
				mPac.img = 0;
			}
		}
	}
}

/*************************************
** �p�b�N�}���̈ړ� **
* ���@�� :�p�b�N�}���̓�������
* �߂�l :����
**************************************/
void Pac_Move(int Type) {
	switch (Type)
	{
	case 0:
		mPac.y -= mPac.speed;
		break;
	case 1:
		mPac.x += mPac.speed;
		break;
	case 2:
		mPac.y += mPac.speed;
		break;
	case 3:
		mPac.x -= mPac.speed;
		break;
	default:
		break;
	}
}

/*************************************
** �p�b�N�}���ƃA�J�x�C�̓����蔻�� **
* ���@�� :�p�b�N�}���̒l,�A�J�x�C�̒l
* �߂�l :TRUE->�q�b�g,FALSE->����
**************************************/
float HitCheckEnemy(PAC*p, AKABEI*e) {
	int sx1 = p->x - (int)(p->w * 0.5f);
	int sy1 = p->y - (int)(p->h * 0.5f);
	int sx2 = p->x + (int)(p->w * 0.5f);
	int sy2 = p->y + (int)(p->h * 0.5f);

	int dx1 = e->x - (int)(e->w * 0.5f);
	int dy1 = e->y - (int)(e->h * 0.5f);
	int dx2 = e->x + (int)(e->w * 0.5f);
	int dy2 = e->y + (int)(e->h * 0.5f);
	
	//��`���d�Ȃ��Ă���Γ�����
	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
		//�p���[�A�b�v���̓X���[�i���j
		if (!PowerUpFlg) {
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}
