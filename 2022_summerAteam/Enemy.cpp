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



float A, B, C;		// 三平方の定理用の変数
float dx, dy;		// 正規化用変数


// 初期化
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // モンスターの画像を読み込む
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // 目玉の画像を読み込む

	// アカベイの初期化
	Akabei.x = 440.0f;
	Akabei.y = 40.0f;
	Akabei.w = 16.0f;
	Akabei.h = 16.0f;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;
	Akabei.WallHit = false;
	Akabei.left = false;
	Akabei.right = false;
	Akabei.up = false;
	Akabei.bottom = false;

}


// 終了処理
void Enemy_Finalize() {
	for (int i = 0; i < ENEMY_IMAGE_MAX; i++) {
		DeleteGraph(MonsterImage[i]);	// モンスターの画像の解放
	}
	for (int i = 0; i < EYE_IMAGE_MAX; i++) {
		DeleteGraph(EyeImage[i]);		// 目玉の画像の解放
	}
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
	DrawFormatString(1000, 150, 255, "ed = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 210, 255, "mPac.x = %.1f", mPac.x);
	DrawFormatString(1000, 230, 255, "mPac.y = %.1f", mPac.y);
	DrawFormatString(1000, 250, 255, "Akabei.WallHit = %d", Akabei.WallHit);
	DrawFormatString(1000, 270, 255, "Akabei.left = %d", Akabei.left);
	DrawFormatString(1000, 290, 255, "Akabei.right = %d", Akabei.right);
	DrawFormatString(1000, 310, 255, "Akabei.up = %d", Akabei.up);
	DrawFormatString(1000, 330, 255, "Akabei.bottom = %d", Akabei.bottom);



	// アニメーション
	if (!PowerUpFlg) {
		if (Akabei.ImageCount == 0) {
			Akabei.ImageCount = 1;

		}
		else {
			Akabei.ImageCount = 0;
		}
	}
	else {
		if (PowerUpTime > 150) {
			if (Akabei.ImageCount == 16) {
				Akabei.ImageCount = 17;

			}
			else {
				Akabei.ImageCount = 16;
			}
		}
		else {
			if (Akabei.ImageCount < 19) {
				if (PowerUpTime % 5 == 0) {
					Akabei.ImageCount++;
				}
			}
			else
			{
				Akabei.ImageCount = 16;
			}
		}
	}
	//Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	//Akabei.my = Akabei.y;		// アカベイのy座標を保存

	AkabeiChasePlayer();		// アカベイが仮プレイヤーを追いかける処理
	//AkabeiMove();
}


// 描画
void Enemy_Draw() {

	if (!PowerUpFlg) {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);
	}
	else {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
	}
}


// 仮プレイヤーを追いかける処理
void AkabeiChasePlayer() {
	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存

	// 三平方の定理を使う
	A = mPac.x - Akabei.x;

	B = mPac.y - Akabei.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る


	if (Akabei.x < mPac.x - 16) {	// アカベイから見てプレイヤーは右側
		if (Akabei.WallHit == false) {
			Akabei.x += dx * Akabei.speed;
			Akabei.eyeImageCount = 1;
		}
		else {
			Akabei.WallHit = false;
		}
	}
	else if (Akabei.x > mPac.x + 16) {	// アカベイから見てプレイヤーは左側
		if (Akabei.WallHit == false) {
			Akabei.x += dx * Akabei.speed;
			Akabei.eyeImageCount = 3;
		}
		else {
			Akabei.WallHit = false;
		}
	}
	else if (Akabei.y < mPac.y - 16) {	// アカベイから見てプレイヤーは下側
		if (Akabei.WallHit == false) {
			Akabei.y += dy * Akabei.speed;
			Akabei.eyeImageCount = 2;
		}
		else {
			Akabei.WallHit = false;
		}
	}
	else if (Akabei.y > mPac.y + 16) {		// アカベイから見てプレイヤーは上側
		if (Akabei.WallHit == false) {
			Akabei.y += dy * Akabei.speed;
			Akabei.eyeImageCount = 0;
		}
		else {
			Akabei.WallHit = false;
		}
	}
}

void AkabeiMove() {
	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存
	Akabei.md = Akabei.ed;		// 敵の動く方向を保存

	// アカベイが壁を避けながら移動する処理
	if (Akabei.WallHit == false) {
		switch (Akabei.ed) {
		case 0:	// 左へ移動
			Akabei.x--;
			Akabei.eyeImageCount = 3;
			if (Akabei.left == true) {
				Akabei.ed = 3;
			}
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
	}
}