#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"



static int mImageHandle; //�摜�n���h���i�[�p�ϐ�


//������
void Menu_Initialize() {

	//mImageHandle = LoadGraph("images/Scene_Menu.png"); //�摜�̃��[�h
	mImageHandle = LoadGraph("images/Title.png"); //�摜�̃��[�h
}
//�I������
void Menu_Finalize() {
	DeleteGraph(mImageHandle); //�摜�̉��
}
//�X�V
void Menu_Update() {
	if (CheckHitKey(KEY_INPUT_G) != 0) {//Esc�L�[��������Ă�����orB�{�^����������Ă�����
		SceneMgr_ChangeScene(eScene_Game);//�V�[�������j���[�ɕύX
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {// ESCAPE�L�[��������Ă��邩�𒲂ׂ�
			DxLib_End();
	}

	if ((g_NowKey & PAD_INPUT_7) != 0) {
		DxLib_End();
	}

	if (CheckHitKey(KEY_INPUT_C) != 0) {//Esc�L�[��������Ă�����orB�{�^����������Ă�����
		SceneMgr_ChangeScene(eScene_Config);//�V�[�������j���[�ɕύX
	}
}
//�`��
void Menu_Draw() {
	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 650, 0xffffff, "G�L�[�������ăQ�[����ʂ�");
	DrawFormatString(10, 700, 0xffffff, "C�L�[�������ăR���t�B�O��ʂ�");
}