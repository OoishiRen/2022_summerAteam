#pragma once
typedef enum
{
	eScene_Menu, //���j���[���
	eScene_Game, //�Q�[�����
	eScene_Config, //�ݒ���
	eScene_None, //����

} eScene;
void SceneMgr_Initialize();//������
void SceneMgr_Finalize();//�I������
void SceneMgr_Update();//�X�V
void SceneMgr_Draw();//�`��

// ���� nextScene�ɃV�[����ύX����
void SceneMgr_ChangeScene(eScene nextScene);