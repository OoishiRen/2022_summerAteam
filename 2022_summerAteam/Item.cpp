#include "Item.h"
#include "Dxlib.h"
#include "Game.h"
#include "Player.h"

int Dot_Handle;
int Powerdot_Handle[DOT_IMAGE_MAX];
int Fruits_Handle[FRUIT_IMAGE_MAX];

int Cnt;
int Powerdot_Image;

int Score;
int DotCnt;
int FruitCnt;

bool PowerUpFlg;

int PowerUpTime;
int FruitTime;

int Item_Mapdata[MAP_HEIGHT][MAP_WIDTH]
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },

 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },

 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,    0 ,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 0, 0 },
 { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int HitCheck(float Cx, float Cy, float Cw, float Ch, float Ix, float Iy, float Iw, float Ih) {

	//���̓����蔻��
	float CharaL = Cx;		//�L������x��
	float CharaR = Cx + Cw;	//�L������x�� + ��
	float ItemL = Ix;		//�A�C�e����x��
	float ItemR = Ix + Iw;	//�A�C�e����x�� + ��

	//�c�̓����蔻��
	float CharaU = Cy;		//�L������y��
	float CharaD = Cy + Ch;	//�L������y�� + ����
	float ItemU = Iy;		//�A�C�e����y��
	float ItemD = Iy + Ih;	//�A�C�e����y�� + ����

	if (CharaL >= ItemR) {
		return 0;
	}
	if (CharaR <= ItemL) {
		return 0;
	}
	if (CharaU >= ItemD) {
		return 0;
	}
	if (CharaD <= ItemU) {
		return 0;
	}
}

void Item_Initialize() {
	Dot_Handle = LoadGraph("Item_images/dot.png");
	LoadDivGraph("Item_Images/big_dot2.png", 2, 2, 1, 16, 16, Powerdot_Handle);
	LoadDivGraph("Item_Images/fruit.png", 13, 13, 1, 16, 16, Fruits_Handle);
	Cnt = 0;
	Score = 0;
	DotCnt = 0;
	FruitCnt = 0;
	PowerUpFlg = false;
	PowerUpTime = 510;
	FruitTime = 660;
}
void Item_Finalize() {
	DeleteGraph(Dot_Handle);
	//DeleteGraph(Powerdot_Handle[2]);
}
void Item_Update() {
	HitItem();//�A�C�e���̃q�b�g�֐�
	PowerdotAnim();//�p���[�G�T�̃A�j���[�V�����֐�
	PowerUp();//�p���[�A�b�v�̊֐�
	FruitAppear();
}
void Item_Draw() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 1) {//�}�b�v�f�[�^���P��������
				//�G�T��`��
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Dot_Handle, TRUE);
			}
			if (Item_Mapdata[i][j] == 2) {//�}�b�v�f�[�^���Q��������
				//�p���[�G�T��`��
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Powerdot_Handle[Powerdot_Image], TRUE);
			}
			if (Item_Mapdata[i][j] == 3) {//�}�b�v�f�[�^���Q��������
				//�t���[�c��`��
				DrawRotaGraph(j * FRUIT_SIZE /*+16*/, i * FRUIT_SIZE - 8, 2.0f, 0, Fruits_Handle[0], TRUE, FALSE);
			}
			//if (Fruits_Mapdata[i][j] == 1) {
			//	//DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, Fruits_Handle[0], TRUE);
			//}
		}
	}


	DrawFormatString(0, 10, GetColor(255, 255, 255), "Score:%d", Score);//�Ńo�b�O
	DrawFormatString(0, 30, GetColor(255, 255, 255), PowerUpFlg ? "PowerUp" : "Normal");//�Ńo�b�O
	DrawFormatString(0, 50, GetColor(255, 255, 255), "PowerUpTime:%d", PowerUpTime);//�Ńo�b�O
	DrawFormatString(0, 70, GetColor(255, 255, 255), "Dot:%d", DotCnt);//�Ńo�b�O
	DrawFormatString(0, 90, GetColor(255, 255, 255), "FruitTime:%d", FruitTime);//�Ńo�b�O
}

void PowerdotAnim() {

	//Cnt���P�T�ɂȂ�܂ő��₷
	if (Cnt < CNT_SPEED) {
		Cnt++;
	}
	else if (Cnt == CNT_SPEED) {
		Cnt = 0;
	}

	if (Cnt < CNT_SPEED / 2) {	//Cnt���P�T�ȉ���������
		Powerdot_Image = 0;		//�\��
	}
	else if (Cnt > CNT_SPEED / 2 && Cnt < CNT_SPEED) {	//Cnt��15�ȏ�Ȃ�
		Powerdot_Image = 1;		//��\��
	}
}

void HitItem() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			//�q�b�g�`�F�b�N
			if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, j, i)) {
				if (Item_Mapdata[i][j] == 1) {//�H�ׂ��̂��G�T�������̏ꍇ
					Item_Mapdata[i][j] = 0;//�G�T������
					Score += 10;//�X�R�A�{�P�O
					DotCnt++;	//�H�ׂ����{�P
				}
				if (Item_Mapdata[i][j] == 2) {//�H�ׂ��̂��p���[�G�T�������ꍇ
					Item_Mapdata[i][j] = 0;//�p���[�G�T������
					Score += 50;//�X�R�A�{�T�O
					DotCnt++;	//�H�ׂ����{�P
					PowerUpFlg = true;//�p���[�A�b�v�t���O��true�ɂ���
				}
				//�t���[�c�p�̃q�b�g�`�F�b�N
				if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE/*+16*/, i * MAP_SIZE, j - 8, i)) {
					if (Item_Mapdata[i][j] == 3) {//�H�ׂ��̂��t���[�c�������ꍇ
						Item_Mapdata[i][j] = 0;//�t���[�c������
						Score += 100;//�X�R�A�{�P�O�O
						FruitCnt++;
						FruitTime = 660;
					}
				}
			}
		}
	}
}

void PowerUp() {
	if (PowerUpFlg == true && PowerUpTime != 0) {//�p���[�A�b�v�t���O��true�ɂȂ�����
		PowerUpTime--;			//�T�P�O�t���[���̊ԃp���[�A�b�v
	}
	else if (PowerUpTime == 0) {//�T�P�O�t���[���o�߂�
		PowerUpFlg = false;		//�p���[�A�b�v�t���O��false�ɂ���
		PowerUpTime = 510;
	}
}

void FruitAppear() {

	if (DotCnt >= 58 && FruitCnt == 0) {//�G�T���T�W�H�ׂ邽��
		if (FruitTime >= 0) {			//
			FruitTime--;				//�U�U�O�t���[���̊�
			Item_Mapdata[19][15] = 3;	//�t���[�c���o������
		}
		else if (FruitTime == -1) {		//�U�U�O�t���[���o�߂����

			Item_Mapdata[19][15] = 0;	//�t���[�c��������
			FruitCnt++;
			FruitTime = 660;
		}
	}
	if (DotCnt >= 137 && FruitCnt == 1) {//�G�T���P�R�V�H�ׂ邽��
		if (FruitTime >= 0) {			//
			FruitTime--;				//�U�U�O�t���[���̊�
			Item_Mapdata[19][15] = 3;	//�t���[�c���o������
		}
		else if (FruitTime == -1) {		//660�t���[���o�߂����

			Item_Mapdata[19][15] = 0;	//�t���[�c��������
			FruitCnt++;
			FruitTime = 660;
		}
	}
}