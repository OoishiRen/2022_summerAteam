#include "DxLib.h"
#include "SceneMgr.h"
#include"Input.h"

//�X�V

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetMainWindowText("Pac-Man");		//�^�C�g����ݒ�

	ChangeWindowMode(TRUE); //�E�B���h�E���[�h�ύX
	DxLib_Init(); //DX���C�u����������
	SetDrawScreen(DX_SCREEN_BACK); //����ʐݒ�
	SetGraphMode(1280, 720, 32);   // ��ʃ��[�h�̕ύX
	SceneMgr_Initialize(); 

	while (ProcessMessage() == 0 ) { //��ʍX�V &���b�Z�[�W����& ��ʏ���

		double dNextTime = GetNowCount();

		ClearDrawScreen();
		
		InputKey();
		SceneMgr_Update(); //�X�V
		SceneMgr_Draw(); //�`��
		if (dNextTime + 16.66 > GetNowCount()) {
			WaitTimer((int)dNextTime - GetNowCount());
		}
		ScreenFlip();
	}
	SceneMgr_Finalize();
	DxLib_End(); // DX���C�u�����I������
	return 0;
}