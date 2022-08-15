#pragma once

/* 定数 */
const int ENEMY_IMAGE_MAX = 20;		// モンスターの最大画像数
const int EYE_IMAGE_MAX = 4;		// 目玉の最大画像数

/* 構造体 */

// アカベイの構造体
struct AKABEI {
	float x, y;			// x座標、y座標用変数
	float speed;		// 移動速度
	int ImageCount;		// アニメーション用変数
	int eyeImageCount;	// 目玉のアニメーション用変数
};
extern struct AKABEI Akabei;


extern void Enemy_Initialize();		// 初期化
extern void Enemy_Finalize();		// 終了処理
extern void Enemy_Update();			// 更新
extern void Enemy_Draw();			// 描画

extern void AkabeiChasePlayer();		// アカベイがプレイヤーを追いかける処理（簡易版）