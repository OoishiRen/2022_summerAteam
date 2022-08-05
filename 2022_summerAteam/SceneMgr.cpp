#include "DxLib.h"
#include "Config.h"
#include "Game.h"
#include "Menu.h"
#include "SceneMgr.h"


static eScene mScene = eScene_Menu; //�V�[���Ǘ��ϐ�
static eScene mNextScene = eScene_None; //���̃V�[���Ǘ��ϐ�
static void SceneMgr_InitializeModule(eScene scene);//�w�胂�W���[��������������
static void SceneMgr_FinalizeModule(eScene scene);//�w�胂�W���[���̏I���������s��


//������
void SceneMgr_Initialize() {
	SceneMgr_InitializeModule(mScene);
}


//�I������
void SceneMgr_Finalize() {
	SceneMgr_FinalizeModule(mScene);
}

//�X�V
void SceneMgr_Update() {
	if (mNextScene != eScene_None) { //���̃V�[�����Z�b�g����Ă�����
		SceneMgr_FinalizeModule(mScene);//���݂̃V�[���̏I�����������s
		mScene = mNextScene; //���̃V�[�������݂̃V�[���Z�b�g
		mNextScene = eScene_None; //���̃V�[�������N���A
		SceneMgr_InitializeModule(mScene); //���݂̃V�[����������
	}
	switch (mScene) { //�V�[���ɂ���ď����𕪊�
	case eScene_Menu: //���݂̉�ʂ����j���[(�^�C�g��)�Ȃ�
		Menu_Update(); //���j���[��ʂ̍X�V����������
		break;//�ȉ���
	case eScene_Game:
		Game_Update();
		break;
	case eScene_Config:
		Config_Update();
		break;
	}
}
//�`��
void SceneMgr_Draw() {
	switch (mScene) { //�V�[���ɂ���ď����𕪊�
	case eScene_Menu: //���݂̉�ʂ����j���[��ʂȂ�
		Menu_Draw(); //���j���[��ʂ̕`�揈��������
		break;//�ȉ���
	case eScene_Game:
		Game_Draw();
		break;
	case eScene_Config:
		Config_Draw();
		break;
	}
}


// ���� nextScene �ɃV�[����ύX����
void SceneMgr_ChangeScene(eScene NextScene) {
	mNextScene = NextScene; //���̃V�[�����Z�b�g����
}

// ����scene���W���[��������������
static void SceneMgr_InitializeModule(eScene scene) {
	switch
		(scene) { //�V�[���ɂ���ď�������
	case eScene_Menu: //�w���ʂ����j���[��ʂȂ�
		Menu_Initialize(); //���j���[��ʂ̏���������������
		break;//�ȉ���
	case eScene_Game:
		Game_Initialize();
		break;
	case eScene_Config:
		Config_Initialize();
		break;
	}
}
// ����scene���W���[���̏I���������s��
static void SceneMgr_FinalizeModule(eScene scene) {
	switch
		(scene) { //�V�[���ɂ���ď����𕪊�
	case eScene_Menu: //�w���ʂ����j���[��ʂȂ�
		Menu_Finalize(); //���j���[��ʂ̏I����������������
		break;//�ȉ���
	case eScene_Game:
		Game_Finalize();
		break;
	case eScene_Config:
		Config_Finalize();
		break;
	}
}