#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"

static int mImageHandle; //�摜�n���h���i�[�p�ϐ�
float mx = 0, my = 0;

/**********���̃}�b�v�`�b�v�i�����Ă������j**********/
int MapData[MAP_HEIGHT][MAP_WIDTH] = //�}�b�v�f�[�^ 1�͕ǂ����� 0�͕ǂ��Ȃ��@
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },

 { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },

 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0 },
 { 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0 },
 { 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/**********���̃}�b�v�`�b�v�i�����Ă������j**********/


void DrawMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {
				DrawBox(j * MAP_SIZE, i * MAP_SIZE, j *
					MAP_SIZE + MAP_SIZE, i * MAP_SIZE + MAP_SIZE, GetColor(0, 0, 255), TRUE);

				if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					mPac.x = mx;
					mPac.y = my;

					WarpTunnel();
				}

				//if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				//	Akabei.WallHit = true;
				//	// ���̏ꏊ�ɖ߂�
				//	Akabei.x = Akabei.mx;
				//	Akabei.y = Akabei.my;

				//}
			}
		}
	}
}


//������
void Game_Initialize() {
	Player_Initialize();
	Enemy_Initialize();
	Item_Initialize();//�A�C�e���p
	//mImageHandle = LoadGraph("images/Scene_GameMain.png"); //�摜�̃��[�h
}

//�I������
void Game_Finalize() {
	Player_Finalize();
	Enemy_Finalize();
	Item_Finalize();//�A�C�e���p
	//DeleteGraph(mImageHandle); //�摜�̉��
}

//�X�V
void Game_Update() {
	mx = mPac.x;
	my = mPac.y;

	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Esc�L�[��������Ă�����
		SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
	}
	Player_Update();
	Enemy_Update();
	DrawMap();
	Item_Update();//�A�C�e���p

	if (MapData[(int)Akabei.y / 16][(int)Akabei.x / 16] == 1) {
		Akabei.x = Akabei.mx;
		Akabei.y = Akabei.my;
	}
}

//�`��
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESC�L�[�������ă��j���[��ʂ�");
	Player_Draw();
	Enemy_Draw();
	Item_Draw();
}

void WarpTunnel() {

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(mPac.x + 16, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 2) {//
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 3 && mPac.type == 3) {
								mPac.x = j * MAP_SIZE + 8;
								mPac.y = i * MAP_SIZE + 8;
							}
						}
					}

				}

			}
			if (HitCheck(mPac.x - 16, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {

				if (MapData[i][j] == 3) {
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 2 && mPac.type == 1) {
								mPac.x = j * MAP_SIZE + 8;
								mPac.y = i * MAP_SIZE + 8;
							}
						}
					}
				}
			}
		}
	}
}


void AkabeiMove() {
	Akabei.mx = Akabei.x;		// �A�J�x�C��x���W��ۑ�
	Akabei.my = Akabei.y;		// �A�J�x�C��y���W��ۑ�
	Akabei.md = Akabei.ed;		// �G�̓���������ۑ�

	// �A�J�x�C���ǂ�����Ȃ���ړ����鏈��
	while (1) {
		switch (Akabei.ed) {
		case 0:	// ���ֈړ�
			Akabei.x--;
			Akabei.eyeImageCount = 3;
			break;
		case 1:	// �E�ֈړ�
			Akabei.x++;
			Akabei.eyeImageCount = 1;
			break;
		case 2:	// ��ֈړ�
			Akabei.y--;
			Akabei.eyeImageCount = 0;
			break;
		case 3:	// ���ֈړ�
			Akabei.y++;
			Akabei.eyeImageCount = 2;
			break;
		}

		// �ǁi��ʒ[���炢�ɐݒ肵�Ă�j�ɓ���������
		if (MapData[(int)Akabei.y / 16][(int)Akabei.x / 16] == 1) {
			// ���̏ꏊ�ɖ߂�
			Akabei.x = Akabei.mx;
			Akabei.y = Akabei.my;

			// �i�ޕ��������߂�
			switch (Akabei.md) {
			case 0:
				//if (Akabei.ed == 0 && Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
				//	Akabei.left = false;
				//	Akabei.ed = 3;
				//	Akabei.WallHit = false;
				//}

				// ���ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͏ォ���ɂȂ�
				if (Akabei.ed == 0) {
					if (Akabei.y > mPac.y) {	// �A�J�x�C�̈ʒu�����v���C���[��艺�Ȃ�
						Akabei.ed = 2;	// ��ɕ�����ς���
					}
					else {
						Akabei.ed = 3;	// ���ɕ�����ς���
					}
				}
				else if (Akabei.ed == 2) {	// ���݂̈ʒu�̏オ�ǂ������̂ŉ��ɕ�����ς���
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {	// ���݂̈ʒu�̉����ǂ������̂ŏ�ɕ�����ς���
					Akabei.ed = 2;
				}
				break;

			case 1:
				//if (Akabei.ed == 1 && Akabei.right == true && Akabei.left == false && Akabei.up == false && Akabei.bottom == false) {
				//	Akabei.right = false;
				//	Akabei.ed = 2;
				//	Akabei.WallHit = false;
				//}

				// �E�ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͏ォ���ɂȂ�
				if (Akabei.ed == 1) {
					if (Akabei.y > mPac.y) {	// �A�J�x�C�̈ʒu�����v���C���[��艺�Ȃ�
						Akabei.ed = 2;		// ��ɕ�����ς���
					}
					else {
						Akabei.ed = 3;		// ���ɕ�����ς���
					}
				}
				else if (Akabei.ed == 2) {		// ���݂̈ʒu�̏オ�ǂ������̂ŉ��ɕ�����ς���
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {		// ���݂̈ʒu�̉����ǂ������̂ŏ�ɕ�����ς���
					Akabei.ed = 2;
				}
				break;

			case 2:
				//if (Akabei.ed == 2 && Akabei.up == true && Akabei.left == false && Akabei.right == false && Akabei.bottom == false) {
				//	Akabei.up = false;
				//	Akabei.ed = 0;
				//	Akabei.WallHit = false;
				//}

				// ��ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͉E�����ɂȂ�
				if (Akabei.ed == 2) {
					if (Akabei.x > mPac.x) {	// �A�J�x�C�̈ʒu�����v���C���[���E�Ȃ�
						Akabei.ed = 0;		// ���ɕ�����ς���
					}
					else {
						Akabei.ed = 1;		// �E�ɕ�����ς���
					}
				}
				else if (Akabei.ed == 0) {		// ���݂̈ʒu�̍����ǂ������̂ŉE�ɕ�����ς���
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {		// ���݂̈ʒu�̉E���ǂ������̂ō��ɕ�����ς���
					Akabei.ed = 0;
				}
				break;

			case 3:
				//if (Akabei.ed == 3 && Akabei.bottom == true && Akabei.left == false && Akabei.right == false && Akabei.up == false) {
				//	Akabei.bottom = false;
				//	Akabei.ed = 1;
				//	Akabei.WallHit = false;
				//}

				// ���ɐi��ł��鎞�ɕǂɓ��������ꍇ�A�i�߂�����͉E�����ɂȂ�
				if (Akabei.ed == 3) {
					if (Akabei.x > mPac.x) {	// �A�J�x�C�̈ʒu�����v���C���[���E�Ȃ�
						Akabei.ed = 0;		// ���ɕ�����ς���
					}
					else {
						Akabei.ed = 1;		// �E�ɕ�����ς���
					}
				}
				else if (Akabei.ed == 0) {		// ���݂̈ʒu�̍����ǂ������̂ŉE�ɕ�����ς���
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {		// ���݂̈ʒu�̉E���ǂ������̂ō��ɕ�����ς���
					Akabei.ed = 0;
				}
				break;
			}
			Akabei.WallHit = false;
		}
		else {
			break;		// �ړ��悪�ǂ���Ȃ��ꍇ�͕�����ς��郋�[�v���甲����
		}
	}
}