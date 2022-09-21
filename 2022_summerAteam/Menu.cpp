#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"



static int mTitleHandle; //�摜�n���h���i�[�p�ϐ�
static int score_image[15];
int img_x = 338, img_y = 720;
int num14 = 1;
int num15 = 0;
int num16 = 0;
int num17 = 0;
int num18 = 0;



//������
void Menu_Initialize() {
	//mImageHandle = LoadGraph("images/Scene_Menu.png"); //�摜�̃��[�h

	mTitleHandle = LoadGraph("Item_images/PAC-MAN_logo.png"); //�摜�̃��[�h
	//mTitleHandle = LoadGraph("UI_images/num.png"); //�摜�̃��[�h
	LoadDivGraph("UI_images/num.png", 15, 15, 1, 16, 16, score_image);



	img_x = 338, img_y = 720;//�^�C�g���̏����ʒu
}
//�I������
void Menu_Finalize() {
	DeleteGraph(mTitleHandle); //�摜�̉��
}
//�X�V
void Menu_Update() {

	//�f�o�b�O�p
	if (CheckHitKey(KEY_INPUT_S) != 0) {//S�L�[��������Ă�����
		SceneMgr_ChangeScene(eScene_Game);//�V�[�������j���[�ɕύX
	}


	if (img_y <= 138) {

		img_y = 138;

		if (CheckHitKey(KEY_INPUT_G) != 0) {//G�L�[��������Ă�����
			SceneMgr_ChangeScene(eScene_Game);//�V�[�������j���[�ɕύX
		}
		if ((g_NowKey & PAD_INPUT_1) != 0 || (g_NowKey & PAD_INPUT_8) != 0) {//A�{�^���A�܂���START�{�^����������Ă�����
			SceneMgr_ChangeScene(eScene_Game);//�V�[�������j���[�ɕύX
		}

		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0 || (g_NowKey & PAD_INPUT_7) != 0) {// ESCAPE�L�[��������Ă��邩�𒲂ׂ�
			DxLib_End();
		}
	}
	else {
		img_y -= TITLE_SPD;
	}
}
//�`��
void Menu_Draw() {
	DrawGraph(img_x, img_y, mTitleHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "G�L�[�A�܂���A�{�^���������ăQ�[����ʂ�");
	DrawFormatString(10, 680, 0xffffff, "S�L�[�������ă^�C�g�����X�L�b�v(�Q�[����ʂ�)");
	DrawRotaGraph(900, 50, 2.0f, 0, score_image[num14], TRUE, FALSE);
	DrawRotaGraph(925, 50, 2.0f, 0, score_image[num15], TRUE, FALSE);
	DrawRotaGraph(950, 50, 2.0f, 0, score_image[num16], TRUE, FALSE);
	DrawRotaGraph(975, 50, 2.0f, 0, score_image[num17], TRUE, FALSE);
	DrawRotaGraph(1000, 50, 2.0f, 0, score_image[num18], TRUE, FALSE);
}