#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"

static int mImageHandle; //�摜�n���h���i�[�p�ϐ�

//������
void Game_Initialize() {
	Player_Initialize();
	Enemy_Initialize();
	//mImageHandle = LoadGraph("images/Scene_GameMain.png"); //�摜�̃��[�h
}

//�I������
void Game_Finalize() {
	Player_Finalize();
	Enemy_Finalize();
	//DeleteGraph(mImageHandle); //�摜�̉��
}

//�X�V
void Game_Update() {
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Esc�L�[��������Ă�����
		SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
	}
	Player_Update();
	Enemy_Update();

}

//�`��
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESC�L�[�������ă��j���[��ʂ�");
	Player_Draw();
	Enemy_Draw();
}