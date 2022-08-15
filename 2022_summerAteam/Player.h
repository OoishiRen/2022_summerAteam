#pragma once

/*************************
** �萔 **
*************************/

#define PAC_IMAGE_MAX	12	//�p�b�N�}���摜�i�[�p

/*********************************
**     �\����     **
**********************************/

struct PAC {
	bool flg;		//�~�X������:false
	bool dir;		//����
	int type;		//��ԑJ��
	int img;		//�摜���ʗp
	float x, y, w, h;	//���W
	float speed;		//�ړ���
	int cnt;		//�^�C��
};

extern struct PAC mPac;		//�p�b�N�}���\����

void Player_Initialize();	//������
void Player_Finalize();		//�I������
void Player_Update();		//�X�V
void Player_Draw();			//�`��

void Pac_Anim(float val);	//�p�b�N�}���̃A�j���[�V����
void Pac_Move(int Type);			//�p�b�N�}���̈ړ�