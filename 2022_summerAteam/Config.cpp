#include "Config.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"

static int mImageHandle; //�摜�n���h���i�[�p�ϐ�h
//������
void Config_Initialize() {
	//mImageHandle = LoadGraph("images/Help.png");//�摜�̃��[�h
}
//�I������
void Config_Finalize() {
	//DeleteGraph(mImageHandle);//�摜�̉��
}
//�X�V
void Config_Update() {
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Esc�L�[��������Ă�����orB�{�^����������Ă�����
		SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
	}
}
//�`��
void Config_Draw() {
	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESC�L�[�������ă��j���[��ʂ�");
}