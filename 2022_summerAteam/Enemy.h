#pragma once

#define ENEMY_SIZE 16
#define ENEMY_CNT_SPEED 15

/* �萔 */
const int ENEMY_IMAGE_MAX = 20;		// �����X�^�[�̍ő�摜��
const int EYE_IMAGE_MAX = 4;		// �ڋʂ̍ő�摜��

/* �\���� */

// �A�J�x�C�̍\����
struct AKABEI {
	double x, y;			// x���W�Ay���W�p�ϐ�
	double w, h;
	double mx, my;		// x���W�Ay���W�ۑ��p�ϐ�
	int ed;				// �������Ǘ�����p�̕ϐ�
	int md;				// ������ۑ�����p�̕ϐ�
	float speed;		// �ړ����x
	int ImageCount;		// �A�j���[�V�����p�ϐ�
	int eyeImageCount;	// �ڋʂ̃A�j���[�V�����p�ϐ�
	bool WallHit;
	bool left, right, up, bottom;
	bool juuji;
	bool movefinish;
	int mapX, mapY;

};
extern struct AKABEI Akabei;
extern struct AKABEI Pinkey;
extern struct AKABEI Aosuke;
extern struct AKABEI Guzuta;


extern void Enemy_Initialize();		// ������
extern void Enemy_Finalize();		// �I������
extern void Enemy_Update();			// �X�V
extern void Enemy_Draw();			// �`��

extern void AkabeiChaseMode();		// �A�J�x�C���v���C���[��ǂ������鏈���i�ȈՔŁj
extern void AkabeiScatterMode();

extern void PinkeyMove();

extern void AosukeMove();
extern void AosukeChaseMode();
extern void AosukeScatterMode();

extern void GuzutaMove();
extern void GuzutaChaseMode();
extern void GuzutaScatterMode();

void ScatterMode();
void ChaseMode();
void IjikeMode();

void ModeChange();