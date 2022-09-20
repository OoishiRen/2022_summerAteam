#pragma once

#define ENEMY_SIZE 16
#define ENEMY_CNT_SPEED 15

/* 定数 */
const int ENEMY_IMAGE_MAX = 20;		// モンスターの最大画像数
const int EYE_IMAGE_MAX = 4;		// 目玉の最大画像数

/* 構造体 */

// アカベイの構造体
struct AKABEI {
	double x, y;			// x座標、y座標用変数
	double w, h;
	double mx, my;		// x座標、y座標保存用変数
	int ed;				// 方向を管理する用の変数
	int md;				// 方向を保存する用の変数
	float speed;		// 移動速度
	int ImageCount;		// アニメーション用変数
	int eyeImageCount;	// 目玉のアニメーション用変数
	bool WallHit;
	bool juuji;
	bool juujiUp, juujiDown, juujiLeft, juujiRight;
	bool left, right, up, bottom;
	bool juujiUp2, juujiDown2, juujiLeft2, juujiRight2;
	int upnum, bottomnum;
	int leftnum, rightnum;
	int dup, dbottom, dleft, dright;
	int mapX, mapY;

};
extern struct AKABEI Akabei;
extern struct AKABEI Pinkey;
extern struct AKABEI Aosuke;
extern struct AKABEI Guzuta;


extern void Enemy_Initialize();		// 初期化
extern void Enemy_Finalize();		// 終了処理
extern void Enemy_Update();			// 更新
extern void Enemy_Draw();			// 描画

extern void AkabeiChasePlayer();		// アカベイがプレイヤーを追いかける処理（簡易版）
extern void AkabeiChasePlayer2();		// アカベイがプレイヤーを追いかける処理（簡易版）

extern void AkabeiMove();
extern void AkabeiMove2();

extern void PinkeyMove();

extern void AosukeMove();

extern void GuzutaMove();

void ScatterMode();
void ChaseMode();
void IjikeMode();

void ModeChange();