#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"

struct AKABEI Akabei;					// アカベイの構造体の宣言
int MonsterImage[ENEMY_IMAGE_MAX];		// モンスターの画像格納用変数
int EyeImage[EYE_IMAGE_MAX];			// 目玉の画像格納用変数


/* 仮プレイヤー（イジケ） */
float PlayerX;		// 仮プレイヤーのx座標
float PlayerY;		// 仮プレイヤーのy座標
int PlayerImage;	// 仮プレイヤーの画像格納用変数

float A, B, C;		// 三平方の定理用の変数
float dx, dy;		// 正規化用変数


// 初期化
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // モンスターの画像を読み込む
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // 目玉の画像を読み込む

	// アカベイの初期化
	Akabei.x = 1000.0f;
	Akabei.y = 400.0f;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;

	// 仮プレイヤーの初期化
	PlayerImage = MonsterImage[19];
	PlayerX = 900.0f;
	PlayerY = 400.0f;
}


// 終了処理
void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);	// モンスターの画像の解放
	}
	for (int i = 0; i < EYE_IMAGE_MAX; i++) {
		DeleteGraph(EyeImage[i]);		// 目玉の画像の解放
	}
	DeleteGraph(PlayerImage);		// 仮プレイヤーの画像の解放
}


//更新
void Enemy_Update() {

	// デバッグ用の変数の表示
	DrawFormatString(1000, 30, 255, "A = %.1f", A);
	DrawFormatString(1000, 50, 255, "B = %.1f", B);
	DrawFormatString(1000, 70, 255, "C = %.1f", C);
	DrawFormatString(1000, 90, 255, "dx = %.1f", dx);
	DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	DrawFormatString(1000, 150, 255, "nd = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 210, 255, "mPac.x = %.1f", mPac.x);
	DrawFormatString(1000, 230, 255, "mPac.y = %.1f", mPac.y);


	// アニメーション
	if (Akabei.ImageCount == 0) {
		Akabei.ImageCount = 1;

	}
	else {
		Akabei.ImageCount = 0;
	}


	// 仮プレイヤーの横移動
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		PlayerX--;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		PlayerX++;
	}

	if (CheckHitKey(KEY_INPUT_UP)) {
		PlayerY--;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN)) {
		PlayerY++;
	}

	//AkabeiChasePlayer();		// アカベイが仮プレイヤーを追いかける処理

}


// 描画
void Enemy_Draw() {

	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
	DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

	DrawRotaGraph(PlayerX, PlayerY, 1, 0, PlayerImage, TRUE);
}


// 仮プレイヤーを追いかける処理
void AkabeiChasePlayer() {
	// 三平方の定理を使う
	A = PlayerX - Akabei.x;

	B = PlayerY - Akabei.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る


	if (Akabei.x < PlayerX - 16) {	// アカベイから見てプレイヤーは右側
		Akabei.x += dx * Akabei.speed;
		Akabei.eyeImageCount = 1;
	}
	else if (Akabei.x > PlayerX + 16) {	// アカベイから見てプレイヤーは左側
		Akabei.x += dx * Akabei.speed;
		Akabei.eyeImageCount = 3;
	}

	if (Akabei.y < PlayerY - 16) {	// アカベイから見てプレイヤーは下側
		Akabei.y += dy * Akabei.speed;
		Akabei.eyeImageCount = 2;
	}
	else if (Akabei.y > PlayerY + 16) {		// アカベイから見てプレイヤーは上側
		Akabei.y += dy * Akabei.speed;
		Akabei.eyeImageCount = 0;
	}
}

void AkabeiMove() {
	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存
	Akabei.md = Akabei.ed;		// 敵の動く方向を保存

	// アカベイが壁を避けながら移動する処理
	while (1) {
		switch (Akabei.ed) {
		case 0:	// 左へ移動
			Akabei.x--;
			Akabei.eyeImageCount = 3;
			break;
		case 1:	// 右へ移動
			Akabei.x++;
			Akabei.eyeImageCount = 1;
			break;
		case 2:	// 上へ移動
			Akabei.y--;
			Akabei.eyeImageCount = 0;
			break;
		case 3:	// 下へ移動
			Akabei.y++;
			Akabei.eyeImageCount = 2;
			break;
		}

		// 壁（画面端くらいに設定してる）に当たったら
		if (Akabei.x > (1280 - 16 * 3) || Akabei.x < 900 || Akabei.y < 16 || Akabei.y >(720 - 16 * 10)) {
			// 元の場所に戻す
			Akabei.x = Akabei.mx;
			Akabei.y = Akabei.my;

			// 進む方向を決める
			switch (Akabei.md) {
			case 0:
				// 左に進んでいる時に壁に当たった場合、進める方向は上か下になる
				if (Akabei.ed == 0) {
					if (Akabei.y > PlayerY) {	// アカベイの位置が仮プレイヤーより下なら
						Akabei.ed = 2;	// 上に方向を変える
					}
					else {
						Akabei.ed = 3;	// 下に方向を変える
					}
				}
				else if (Akabei.ed == 2) {	// 現在の位置の上が壁だったので下に方向を変える
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {	// 現在の位置の下が壁だったので上に方向を変える
					Akabei.ed = 2;
				}
				break;

			case 1:
				// 右に進んでいる時に壁に当たった場合、進める方向は上か下になる
				if (Akabei.ed == 1) {
					if (Akabei.y > PlayerY) {	// アカベイの位置が仮プレイヤーより下なら
						Akabei.ed = 2;		// 上に方向を変える
					}
					else {
						Akabei.ed = 3;		// 下に方向を変える
					}
				}
				else if (Akabei.ed == 2) {		// 現在の位置の上が壁だったので下に方向を変える
					Akabei.ed = 3;
				}
				else if (Akabei.ed == 3) {		// 現在の位置の下が壁だったので上に方向を変える
					Akabei.ed = 2;
				}
				break;

			case 2:
				// 上に進んでいる時に壁に当たった場合、進める方向は右か左になる
				if (Akabei.ed == 2) {
					if (Akabei.x > PlayerX) {	// アカベイの位置が仮プレイヤーより右なら
						Akabei.ed = 0;		// 左に方向を変える
					}
					else {
						Akabei.ed = 1;		// 右に方向を変える
					}
				}
				else if (Akabei.ed == 0) {		// 現在の位置の左が壁だったので右に方向を変える
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {		// 現在の位置の右が壁だったので左に方向を変える
					Akabei.ed = 0;
				}
				break;

			case 3:
				// 下に進んでいる時に壁に当たった場合、進める方向は右か左になる
				if (Akabei.ed == 3) {
					if (Akabei.x > PlayerX) {	// アカベイの位置が仮プレイヤーより右なら
						Akabei.ed = 0;		// 左に方向を変える
					}
					else {
						Akabei.ed = 1;		// 右に方向を変える
					}
				}
				else if (Akabei.ed == 0) {		// 現在の位置の左が壁だったので右に方向を変える
					Akabei.ed = 1;
				}
				else if (Akabei.ed == 1) {		// 現在の位置の右が壁だったので左に方向を変える
					Akabei.ed = 0;
				}
				break;
			}
		}
		else {
			break;		// 移動先が壁じゃない場合は方向を変えるループから抜ける
		}
	}

}