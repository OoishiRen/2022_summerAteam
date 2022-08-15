#pragma once

/* �萔 */
const int ENEMY_IMAGE_MAX = 20;		// �����X�^�[�̍ő�摜��
const int EYE_IMAGE_MAX = 4;		// �ڋʂ̍ő�摜��

/* �\���� */

// �A�J�x�C�̍\����
struct AKABEI {
	float x, y;			// x���W�Ay���W�p�ϐ�
	float speed;		// �ړ����x
	int ImageCount;		// �A�j���[�V�����p�ϐ�
	int eyeImageCount;	// �ڋʂ̃A�j���[�V�����p�ϐ�
};
extern struct AKABEI Akabei;


extern void Enemy_Initialize();		// ������
extern void Enemy_Finalize();		// �I������
extern void Enemy_Update();			// �X�V
extern void Enemy_Draw();			// �`��

extern void AkabeiChasePlayer();		// �A�J�x�C���v���C���[��ǂ������鏈���i�ȈՔŁj