#pragma once

/*************************
** �萔 **
*************************/

#define PAC_IMAGE_MAX	12	//�p�b�N�}���摜�i�[�p
#define DYING_IMAGE_MAX	11	//�~�X���̉摜�i�[�p
#define PAC_WIDTH	16.0f	//�p�b�N�}���̉摜��
#define PAC_HEIGHT	16.0f	//�p�b�N�}���̉摜����
#define PAC_SPEED	4.0f	//�p�b�N�}���̑���
#define PAC_LIFE	3		//�p�b�N�}���̎c�@

/*********************************
**     �\����     **
**********************************/

struct PAC {
	bool flg;		//�~�X������:false
	bool dir;		//����
	int var;		//���O���͗p
	int type;		//��ԑJ��
	int img;		//�摜���ʗp
	float x, y, w, h;	//���W
	float speed;		//�ړ���
	int cnt;		//�c�@
};

extern struct PAC mPac;		//�p�b�N�}���\����

static bool pac_x0 = false, pac_x1 = false, pac_y0 = false, pac_y1 = false;

void Player_Initialize();	//������
void Player_Finalize();		//�I������
void Player_Update();		//�X�V
void Player_Draw();			//�`��

void Pac_Anim(float val);	//�p�b�N�}���̃A�j���[�V����
void Pac_Move(int Type);			//�p�b�N�}���̈ړ�