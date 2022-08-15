#pragma once

/* 定数 */
const int ENEMY_IMAGE_MAX = 20;


/* 構造体 */
struct AKABEI {
	float x, y;
	float speed;
	int ImageCount;
};
extern struct AKABEI Akabei;


extern void Enemy_Initialize();
extern void Enemy_Finalize();
extern void Enemy_Update();
extern void Enemy_Draw();