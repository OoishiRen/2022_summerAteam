#pragma once

/* �萔 */
const int ENEMY_IMAGE_MAX = 20;


/* �\���� */
struct AKABEI {
	int x, y;
	float speed;
	int ImageCount;
};
extern struct AKABEI Akabei;


extern void Enemy_Initialize();
extern void Enemy_Finalize();
extern void Enemy_Update();
extern void Enemy_Draw();