#include "Item.h"
#include "Dxlib.h"
#include "Game.h"
#include "Player.h"
#include "UI.h"
#include "Enemy.h"

int Dot_Handle;
int Powerdot_Handle[DOT_IMAGE_MAX];
int Fruits_Handle[FRUIT_IMAGE_MAX];

int Cnt;
int Powerdot_Image;

int Score;//�X�R�A
int DotCnt;//�G�T���J�E���g����ϐ�
int DotsLeft;//�c��̃G�T��
int FruitCnt;//�t���[�c���J�E���g����ϐ�
int Round;//���E���h�`�F���W

bool PowerUpFlg;//�p�b�N�}���̃p���[�A�b�v�t���O

int PowerUpTime;//�p���[�A�b�v�̎���
int FruitTime;//�t���[�c�̏o������
struct FRUITS Fruits;		//�t���[�c�\����
int CntTime;//�J�E���g����ϐ�
bool FruiScoreUI;//�t���[�c��UI�̃t���O

int Item_Mapdata[MAP_HEIGHT][MAP_WIDTH]//�O=�G�T�Ȃ� �P=�G�T �Q=�p���[�G�T �R=�t���[�c
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//0
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },//1
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//2
 { 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0 },//3
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//4
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },//5
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//6
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//7
 { 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },//8
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//9
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//10
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//11
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//12
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//13
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//14

 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//15
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//16
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//17
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//18
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//19
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },//20
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//21
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//22
 { 0, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 0, 0 },//23
 { 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },//24
 { 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int HitCheck(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {

	//���̓����蔻��
	float CharaL = x1;		//�P��x��
	float CharaR = x1 + w1;	//�P��x�� + ��
	float ItemL = x2;		//�Q��x��
	float ItemR = x2 + w2;	//�Q��x�� + ��
	
	//�c�̓����蔻��
	float CharaU = y1;		//�P��y��
	float CharaD = y1 + h1;	//�P��y�� + ����
	float ItemU = y2;		//�Q��y��
	float ItemD = y2 + h2;	//�Q��y�� + ����
	
	if (CharaL >= ItemR) {	//�L�����̍��[���A�C�e���̉E�[��肪�E�ɂ���Ƃ�
		return 0;			//�������ĂȂ�
	}
	if (CharaR <= ItemL) {	//�L�����̉E�[���A�C�e���̍��[��荶�ɂ���Ƃ�
		return 0;			//�������ĂȂ�
	}
	if (CharaU >= ItemD) {	//�L�����̏�[���A�C�e���̉��[��艺�ɂ���Ƃ�
		return 0;			//�������ĂȂ�
	}
	if (CharaD <= ItemU) {	//�L�����̉��[���A�C�e���̏�[����ɂ���Ƃ�
		return 0;			//�������ĂȂ�
	}
	return 1;
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
	DotsLeft = 244;
	Round = 1;
	Fruits.fScore = Fruits.Cherry;
	CntTime = 120;
	FruiScoreUI = false;
}
void Item_Finalize() {
	DeleteGraph(Dot_Handle);
	//DeleteGraph(Powerdot_Handle[2]);
}
void Item_Update() {
	HitItem();//�A�C�e���̃q�b�g�֐�
	PowerdotAnim();//�p���[�G�T�̃A�j���[�V�����֐�
	PowerUp();//�p���[�A�b�v�̊֐�
	FruitTerget();//�t���[�c�o���֐�

	if (DotsLeft == 0) {//�c��G�T���Ȃ��Ȃ����Ƃ�
		RoundChange();//���E���h�`�F���W
	}
	if (FruiScoreUI == true) {
		ScoreUIEnabled();//�t���[�cUI
	}
}
void Item_Draw() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 1) {//�}�b�v�f�[�^���P��������
				//�G�T��`��
				//DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Dot_Handle, TRUE);
				//DrawBox(j * MAP_SIZE, i * MAP_SIZE, j * MAP_SIZE * MAP_SIZE, i * MAP_SIZE * MAP_SIZE, GetColor(0, 255, 0), FALSE);//�f�o�b�O�p
				DrawRotaGraph(j * MAP_SIZE + 24, i * MAP_SIZE + 24, 1.0f, 0, Dot_Handle, TRUE);
			}
			if (Item_Mapdata[i][j] == 2) {//�}�b�v�f�[�^���Q��������
				//�p���[�G�T��`��
				DrawGraph(j * MAP_SIZE +16, i * MAP_SIZE + 16, Powerdot_Handle[Powerdot_Image], TRUE);
			}
			if (Item_Mapdata[i][j] == 3) {//�}�b�v�f�[�^���R��������
				//�t���[�c��`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, Fruits_Handle[Fruits.kind], TRUE, FALSE);
			}
		}
	}
	FruitsUI();//�t���[�c�̃X�R�A��\������֐�


	DrawFormatString(1000, 10, GetColor(255, 255, 255), "Score:%d", Score);//�Ńo�b�O
	DrawFormatString(1000, 30, GetColor(255, 255, 255), PowerUpFlg ? "PowerUp" : "Normal");//�Ńo�b�O
	DrawFormatString(1000, 50, GetColor(255, 255, 255), "PowerUpTime:%d", PowerUpTime);//�Ńo�b�O
	DrawFormatString(1000, 70, GetColor(255, 255, 255), "Dots Left:%d/244", DotsLeft);//�Ńo�b�O
	DrawFormatString(1000, 90, GetColor(255, 255, 255), "Fruits Target:%d", FruitTime);//�Ńo�b�O
	DrawFormatString(1000, 110, GetColor(255, 255, 255), "ROUND:%d", Round);//�Ńo�b�O
	DrawFormatString(1000, 130, GetColor(255, 255, 255), "CntTime:%d", CntTime);//�Ńo�b�O
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
			if (HitCheck(mPac.x - 24, mPac.y - 24, mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				if (Item_Mapdata[i][j] == 1) {//�H�ׂ��̂��G�T�������̏ꍇ
					Item_Mapdata[i][j] = 4;//�G�T������
					Score += 10;//�X�R�A�{�P�O
					DotCnt++;	//�H�ׂ����{�P
					DotsLeft--;	//�c��̌��[�P
					esa = 1;
				}
				if (Item_Mapdata[i][j] == 2) {//�H�ׂ��̂��p���[�G�T�������ꍇ
					Item_Mapdata[i][j] = 5;//�p���[�G�T������
					Score += 50;//�X�R�A�{�T�O
					DotCnt++;	//�H�ׂ����{�P
					DotsLeft--;	//�c��̌��[�P
					PowerUpFlg = true;//�p���[�A�b�v�t���O��true�ɂ���
					esa = 2;
					Run = false;
				}
			}
			//�t���[�c�p�̃q�b�g�`�F�b�N
			if (HitCheck(mPac.x - 4, mPac.y - 4, mPac.w - 8, mPac.h - 8, j * MAP_SIZE - 8, i * MAP_SIZE - 8, MAP_SIZE, MAP_SIZE))
			{
				if (Item_Mapdata[i][j] == 3) {//�H�ׂ��̂��t���[�c�������ꍇ
					Item_Mapdata[i][j] = 0;//�t���[�c������
					Score += Fruits.fScore;//�X�R�A�{�P�O�O
					FruitCnt++;
					FruitTime = 660;
					FruiScoreUI = true;
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

void FruitTerget() {

	//if (DotCnt >= 58 && FruitCnt == 0) {//�G�T���T�W�H�ׂ���
	if (DotsLeft <= 186 && FruitCnt == 0) {//�G�T���T�W�H�ׂ���
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
	//if (DotCnt >= 137 && FruitCnt == 1) {//�G�T���P�R�V�H�ׂ���
	if (DotsLeft <= 107 && FruitCnt == 1) {//�G�T���P�R�V�H�ׂ���
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

void RoundChange() {
	Cnt = 0;//������
	DotCnt = 0;//������
	FruitCnt = 0;//������
	PowerUpFlg = false;//������
	PowerUpTime = 510;//������
	FruitTime = 660;//������
	DotsLeft = 244;//������
	Round++;//������

	mPac.flg = true;
	mPac.type = 0;
	mPac.img = 3;
	mPac.x = 240.0f;
	mPac.y = 392.0f;

	if (Fruits.kind < 12) {
		Fruits.kind++; //�t���[�c�̎��++
	}
	if (Round == 1) {//���E���h�P
		Fruits.fScore = Fruits.Cherry;//�`�F���[
	}
	else if (Round == 2) {//���E���h�Q
		Fruits.fScore = Fruits.Strowberry;//�X�g���x���[
	}
	else if (Round == 3) {//���E���h�R�`
		Fruits.fScore = Fruits.Orenge;//�I�����W
	}
	else if (Round == 5) {//���E���h�T�`
		Fruits.fScore = Fruits.Apple;//�A�b�v��
	}
	else if (Round == 7) {//���E���h�V�`
		Fruits.fScore = Fruits.Melon;//������
	}
	else if (Round == 9) {//���E���h�X�`
		Fruits.fScore = Fruits.Garraxy;//�{�X�E�M�����N�V�A��
	}
	else if (Round == 11) {//���E���h�P�P�`
		Fruits.fScore = Fruits.Bell;//�x��
	}
	else if (Round == 13) {//���E���h�P�R�ȍ~
		Fruits.fScore = Fruits.Key;//��
	}

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 4) {
				Item_Mapdata[i][j] = 1;//�G�T�Ĕz�u
			}if (Item_Mapdata[i][j] == 5) {
				Item_Mapdata[i][j] = 2;//�p���[�G�T�Ĕz�u
			}
		}
	}
}

void ScoreUIEnabled() {
	if (CntTime > 0) {
		CntTime--;//�Q�b�Ԃ̊�
		//�X�R�AUI�\��
		DrawFormatString(15 * MAP_SIZE - 16, 19 * MAP_SIZE - 16, GetColor(255, 255, 255), "%d", Fruits.fScore);//�Ńo�b�O

	}
	else if (CntTime == 0) {
		FruiScoreUI = false;
		CntTime = 120;
	}
}

void FruitsUI() {

	for (int i = 1; i <= Round; i++) {
		DrawRotaGraph(454 + FRUIT_SIZE * i, 316, 1.0f, 0, Fruits_Handle[i - 1], TRUE, FALSE);
	}
}