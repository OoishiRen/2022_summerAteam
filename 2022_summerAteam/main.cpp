#include "DxLib.h"
#include "SceneMgr.h"
#include"Input.h"

static int mStartTime;      //����J�n����
static int mCount;          //�J�E���^
static float mFps;          //fps
static const int N = 60;	//���ς����T���v����
static const int FPS = 60;	//�ݒ肵��FPS

bool Update() {
	if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw() {
	DrawFormatString(0, 400, GetColor(255, 255, 255), "%.1f", mFps);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}


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

		Update();	//�X�V

		ClearDrawScreen();
		
		InputKey();
		SceneMgr_Update(); //�X�V
		SceneMgr_Draw(); //�`��

		Draw();		//�`��

		if (dNextTime + 16.66 > GetNowCount()) {
			WaitTimer((int)dNextTime - GetNowCount());
		}
		ScreenFlip();
		Wait();
	}
	SceneMgr_Finalize();
	DxLib_End(); // DX���C�u�����I������
	return 0;
}