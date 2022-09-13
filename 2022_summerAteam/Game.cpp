#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "UI.h"

static int mImageHandle; //�摜�n���h���i�[�p�ϐ�
static int MapWallLeft_Handle;
static int MapWallRight_Handle;
static int MapWallUP_Handle;
static int MapWallbottom_Handle;
static int MapWall_left_top;
static int MapWall_narrow_top_left;
static int MapWall__narrow_top_right;
static int MapWall_narrow_bottom_left;
static int MapWall_narrow_bottom_right;
static int MapWall_left_bottom;
static int MapWall_outerwall_left;
static int MapWall_gate_left;
static int MapWall_gate_right;
static int MapWall_outrcorner_top_left;
static int MapWall_left_bottom;
static int wall_left;
static int wall_right;
static int wall_top;
static int wall_bottom;

float mx = 0, my = 0;
int mimg = 0, mtype = 0;	//�摜�ۑ��p


/**********���̃}�b�v�`�b�v�i�����Ă������j**********/
int MapData[MAP_HEIGHT][MAP_WIDTH] = //�}�b�v�f�[�^ 1�͕ǂ����� 0�͕ǂ��Ȃ��@
{
 { 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0 },//0 4�͏�
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },//1
 { 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//2
 { 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//3
 { 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//4
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },//5
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//6
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//7
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 6, 0 },//8
 { 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 0, 0 },//9
 { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//10
 { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//11
 { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,   0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//12
 { 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 1, 0, 1, 1, 0, 0, 5, 5, 5, 5, 0, 0 },//13

 { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },//14

 { 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 6, 0 },//15
 { 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//16
 { 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//17
 { 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//18
 { 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 6, 0 },//19
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },//20
 { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//21
 { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//22
 { 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 6, 0 },//23
 { 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 6, 0 },//24
 { 1, 1, 1, 0, 1, 1, 0, 9, 10,0,1, 1, 1, 1,    1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 6, 0 },
 { 1, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 0, 0,1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 1, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 0, 0,1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 6, 0 },
 { 1, 0, 0, 0 ,0,11,11,11,11,11,11,11, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 6, 0 },
 { 1, 0, 0, 12,12, 12, 12, 12, 12, 12,12,12,   0, 0, 12,12,12,12,12,12, 12, 12,12, 0, 6, 0 },
 { 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,   5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0 },//5�͉�
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/**********���̃}�b�v�`�b�v�i�����Ă������j**********/


void DrawMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {
				//DrawBox(j * MAP_SIZE, i * MAP_SIZE, j *	MAP_SIZE + MAP_SIZE, i * MAP_SIZE + MAP_SIZE, GetColor(0, 0, 255), TRUE);

				if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Akabei.x = Akabei.mx;
					Akabei.y = Akabei.my;
					if (Akabei.ed == Akabei.md) {
						Akabei.WallHit = true;
					}
				}
			}

			if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 1) {
					mPac.x = mx;
					mPac.y = my;
					mPac.img = mimg;
					mPac.type = mtype;
				}
				WarpTunnel();
			}
		}
	}
}


//������
void Game_Initialize() {
	Player_Initialize();
	Enemy_Initialize();
	Item_Initialize();//�A�C�e���p
	UI_Initialize();
	//mImageHandle = LoadGraph("Item_images/pac-manRP2.png"); //�摜�̃��[�h
	MapWallUP_Handle = LoadGraph("Map_images/outerwall_top.png");//�O���
	MapWallLeft_Handle = LoadGraph("Map_images/outercorner_narrow_top_left.png");//�p��
	MapWallLeft_Handle = LoadGraph("Map_images/outerwall_left.png");//�O����
	MapWallRight_Handle = LoadGraph("Map_images/outerwall_right.png");//�O�E��
	MapWallbottom_Handle = LoadGraph("Map_images/outerwall_bottom.png");//�O����
	wall_left = LoadGraph("Map_images/wall_left.png");//����
	wall_right = LoadGraph("Map_images/wall_right.png");//�E��
	wall_top = LoadGraph("Map_images/wall_top.png");//���
	wall_bottom = LoadGraph("Map_images/wall_bottom.png");//����
	MapWall_left_top = LoadGraph("Map_images/outernose_left_top.png");//�o����
	MapWall_left_bottom = LoadGraph("Map_images/outernose_left_bottom.png");//�o����
	MapWall_outerwall_left = LoadGraph("Map_images/outercorner_top_right.png");//��p�E
	MapWall_outrcorner_top_left = LoadGraph("Map_images/outercorner_top_left.png");//��p��
	MapWallLeft_Handle = LoadGraph("Map_images/outerwall_left.png");//�p�E
	MapWallLeft_Handle = LoadGraph("Map_images/door.png");//door
	MapWallLeft_Handle = LoadGraph("Map_images/gete_left.png");//�Q�[�g��
	MapWallLeft_Handle = LoadGraph("Map_images/gete_right.png");//�Q�[�g�E
	MapWallLeft_Handle = LoadGraph("Map_images/outercorner_narrow_bottom_left.png");//���p(��)
	MapWallLeft_Handle = LoadGraph("Map_images/outercorner_bottom_left.png");//���p��
	MapWallLeft_Handle = LoadGraph("Map_images/outercorner_bottom_right.png");//���p�E
	MapWallLeft_Handle = LoadGraph("Map_images/outercorner_narrow_bottom_right.png");//���p(�E)
}

//�I������
void Game_Finalize() {

	Player_Finalize();
	Enemy_Finalize();
	Item_Finalize();//�A�C�e���p
	DeleteGraph(mImageHandle); //�摜�̉��
	DeleteGraph(MapWallLeft_Handle); //�摜�̉��
	DeleteGraph(MapWallRight_Handle);
	DeleteGraph(MapWallUP_Handle);
	DeleteGraph(MapWallbottom_Handle);
	DeleteGraph(MapWall_left_bottom);
}

//�X�V
void Game_Update() {
	mx = mPac.x;
	my = mPac.y;
	mimg = mPac.img;
	mtype = mPac.type;

	Pinkey.mx = Pinkey.x;
	Pinkey.my = Pinkey.y;


	// ���ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) - 1] == 1) {
		Akabei.left = true;
	}
	else {
		Akabei.left = false;
	}

	// �E�ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) + 1] == 1) {
		//DrawBox(Akabei.x / 16, Akabei.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Akabei.right = true;
	}
	else {
		Akabei.right = false;
	}

	// ��ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[((int)Akabei.y / 16) - 1][(int)Akabei.x / 16] == 1) {
		Akabei.up = true;
	}
	else {
		Akabei.up = false;
	}

	// ���ɕǂ�����Ƃ��Ƀt���O��true�ɂ���
	if (MapData[((int)Akabei.y / 16) + 1][(int)Akabei.x / 16] == 1) {
		Akabei.bottom = true;
	}
	else {
		Akabei.bottom = false;
	}


	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Esc�L�[��������Ă�����
		SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
	}
	Player_Update();
	Enemy_Update();
	DrawMap();
	DrawGraph(0,0,mImageHandle,TRUE);
	Item_Update();//�A�C�e���p


}

//�`��
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESC�L�[�������ă��j���[��ʂ�");
	Player_Draw();
	Enemy_Draw();
	Item_Draw();
	UI_Draw();
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {//�}�b�v�f�[�^���P��������
				//���̕ǂ�`��
				//DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Dot_Handle, TRUE);
				DrawRotaGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 8, 1.0f, 0, MapWallLeft_Handle, TRUE);
			}
			if (MapData[i][j] == 4) {//�}�b�v�f�[�^��4��������
				//��̕ǂ�`��
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWallUP_Handle, TRUE); 
			}
			if (MapData[i][j] == 5) {//�}�b�v�f�[�^��5��������
				//���̕ǂ�`��
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWallbottom_Handle, TRUE);
			}
			if (MapData[i][j] == 6) {//�}�b�v�f�[�^��6��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWallRight_Handle, TRUE, FALSE);
			}
			if (MapData[i][j] == 7) {//�}�b�v�f�[�^��7��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWall_left_top, TRUE, FALSE);
			}
			if(MapData[i][j] == 8) {//�}�b�v�f�[�^��8��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWall_left_bottom, TRUE, FALSE);
			}
			if (MapData[i][j] == 9) {//�}�b�v�f�[�^��9��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_left, TRUE, FALSE);
			}
			if (MapData[i][j] == 10) {//�}�b�v�f�[�^��10��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_right, TRUE, FALSE);
			}
			if (MapData[i][j] == 11) {//�}�b�v�f�[�^��11��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_top, TRUE, FALSE);
			}
			if (MapData[i][j] == 12) {//�}�b�v�f�[�^��12��������
				//�E�̕ǂ�`��
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_bottom, TRUE, FALSE);
			}
		}
	}
}

void WarpTunnel() {

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(mPac.x + 8, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 2) {//���̃g���l���ɓ�������
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 3 && mPac.type == 3) {//�E�̃g���l����
								//mPac.x = j * MAP_SIZE - 8;//���[�v
								mPac.x = j * MAP_SIZE + 4;//���[�v
								mPac.y = i * MAP_SIZE + 8;//���[�v
							}
						}
					}

				}

			}
			if (HitCheck(mPac.x - 8, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 3) {//�E�̃g���l���ɓ�������
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 2 && mPac.type == 1) {//���̃g���l����
								//mPac.x = j * MAP_SIZE + 16;//���[�v
								mPac.x = j * MAP_SIZE + 12;//���[�v
								mPac.y = i * MAP_SIZE + 8;//���[�v
							}
						}
					}
				}
			}
		}
	}
}