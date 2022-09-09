#include "DxLib.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "math.h"
#include "Game.h"
#include "Item.h"
#include "Player.h"

struct AKABEI Akabei;					// アカベイの構造体の宣言
struct AKABEI Pinkey;
struct AKABEI Aosuke;
struct AKABEI Guzuta;

int MonsterImage[ENEMY_IMAGE_MAX];		// モンスターの画像格納用変数
int EyeImage[EYE_IMAGE_MAX];			// 目玉の画像格納用変数



float A, B, C;		// 三平方の定理用の変数
float dx, dy;		// 正規化用変数

int eCnt;

int ScatterModeTime;//縄張りモードの時間（フレーム）
int ChaseModeTime;//追跡モードの時間（フレーム）
bool EnemyMode; // True = 縄張りモード、false = 追跡モード

// 初期化
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // モンスターの画像を読み込む
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // 目玉の画像を読み込む

	// アカベイの初期化
	Akabei.x = 240.0f;
	Akabei.y = 200.0f;
	Akabei.w = ENEMY_SIZE;
	Akabei.h = ENEMY_SIZE;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 1.5f;
	Akabei.WallHit = false;
	Akabei.left = false;
	Akabei.right = false;
	Akabei.up = false;
	Akabei.bottom = false;

	// ピンキーの初期化
	Pinkey.x = 140.0f;
	Pinkey.y = 40.0f;
	Pinkey.w = ENEMY_SIZE;
	Pinkey.h = ENEMY_SIZE;
	Pinkey.ed = 2;
	Pinkey.ImageCount = 2;
	Pinkey.eyeImageCount = 0;
	Pinkey.speed = 1.5f;
	Pinkey.WallHit = false;
	Pinkey.left = false;
	Pinkey.right = false;
	Pinkey.up = false;
	Pinkey.bottom = false;

	// アオスケの初期化
	Aosuke.x = 200.0f;
	Aosuke.y = 260.0f;
	Aosuke.w = ENEMY_SIZE;
	Aosuke.h = ENEMY_SIZE;
	Aosuke.ed = 2;
	Aosuke.ImageCount = 4;
	Aosuke.eyeImageCount = 0;
	Aosuke.speed = 1.5f;
	Aosuke.WallHit = false;
	Aosuke.left = false;
	Aosuke.right = false;
	Aosuke.up = false;
	Aosuke.bottom = false;

	// グズタの初期化
	Guzuta.x = 280.0f;
	Guzuta.y = 260.0f;
	Guzuta.w = ENEMY_SIZE;
	Guzuta.h = ENEMY_SIZE;
	Guzuta.ed = 2;
	Guzuta.ImageCount = 6;
	Guzuta.eyeImageCount = 0;
	Guzuta.speed = 1.5f;
	Guzuta.WallHit = false;
	Guzuta.left = false;
	Guzuta.right = false;
	Guzuta.up = false;
	Guzuta.bottom = false;

	eCnt = 0;
	ScatterModeTime = 480;
	ChaseModeTime = 1230;
	EnemyMode = true;
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

	Pinkey.mx = Pinkey.x;
	Pinkey.my = Pinkey.y;

	// デバッグ用の変数の表示
	DrawFormatString(1000, 30, 255, "A = %.1f", A);
	DrawFormatString(1000, 50, 255, "B = %.1f", B);
	DrawFormatString(1000, 70, 255, "C = %.1f", C);
	DrawFormatString(1000, 90, 255, "dx = %.1f", dx);
	DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	DrawFormatString(1000, 150, 255, "ed = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Aosuke.x = %.1f", Aosuke.x / 16);
	DrawFormatString(1000, 190, 255, "Guzuta.x = %.1f", Guzuta.x / 16);
	DrawFormatString(1000, 210, 255, "mPac.x = %.1f", mPac.x / 16);
	DrawFormatString(1000, 230, 255, "mPac.y = %.1f", mPac.y / 16);
	DrawFormatString(1000, 250, 255, "Akabei.WallHit = %d", Akabei.WallHit);
	DrawFormatString(1000, 270, 255, "Akabei.left = %d", Akabei.left);
	DrawFormatString(1000, 290, 255, "Akabei.right = %d", Akabei.right);
	DrawFormatString(1000, 310, 255, "Akabei.up = %d", Akabei.up);
	DrawFormatString(1000, 330, 255, "Akabei.bottom = %d", Akabei.bottom);
	DrawFormatString(1000, 350, 255, "DotCnt = %d", DotCnt);

	DrawFormatString(500, 30, GetColor(255, 255, 255),
		EnemyMode ? "Scatter %d" : "Chase %d", EnemyMode ? ScatterModeTime : ChaseModeTime);



	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, Pinkey.w, Pinkey.h,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {

				if (MapData[i][j] == 1) {
					Pinkey.x = Pinkey.mx;
					Pinkey.y = Pinkey.my;
				}
			}
		}
	}

	ModeChange();

	// アニメーション
	if (!PowerUpFlg) {

		if (eCnt < ENEMY_CNT_SPEED) {
			eCnt++;
		}
		else if (eCnt == ENEMY_CNT_SPEED) {
			eCnt = 0;
		}

		if (eCnt < ENEMY_CNT_SPEED / 2) {
			Akabei.ImageCount = 1;
			Pinkey.ImageCount = 3;
			Aosuke.ImageCount = 5;
			Guzuta.ImageCount = 7;
		}
		else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
			Akabei.ImageCount = 0;
			Pinkey.ImageCount = 2;
			Aosuke.ImageCount = 4;
			Guzuta.ImageCount = 6;
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


			if (eCnt < ENEMY_CNT_SPEED) {
				eCnt++;
			}
			else if (eCnt == ENEMY_CNT_SPEED) {
				eCnt = 0;
			}
			if (eCnt < ENEMY_CNT_SPEED / 2) {
				Pinkey.ImageCount = 16;
			}
			else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
				Pinkey.ImageCount = 17;
			}

		}
		else {
			if (Akabei.ImageCount < 19) {
				if (PowerUpTime % 5 == 0) {
					Akabei.ImageCount++;
					Pinkey.ImageCount++;
				}
			}
			else
			{
				Akabei.ImageCount = 16;
				Pinkey.ImageCount = 16;
			}
		}


	}
	//Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	//Akabei.my = Akabei.y;		// アカベイのy座標を保存

	//AkabeiChasePlayer();		// アカベイが仮プレイヤーを追いかける処理
	AkabeiMove();
	AosukeMove();
	GuzutaMove();
}


// 描画
void Enemy_Draw() {

	if (!PowerUpFlg) {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);
		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, EyeImage[Pinkey.eyeImageCount], TRUE);


		DrawRotaGraph(Aosuke.x, Aosuke.y, 1, 0, MonsterImage[Aosuke.ImageCount], TRUE);
		DrawRotaGraph(Aosuke.x, Aosuke.y, 1, 0, EyeImage[Aosuke.eyeImageCount], TRUE);

		DrawRotaGraph(Guzuta.x, Guzuta.y, 1, 0, MonsterImage[Guzuta.ImageCount], TRUE);
		DrawRotaGraph(Guzuta.x, Guzuta.y, 1, 0, EyeImage[Guzuta.eyeImageCount], TRUE);

	}
	else {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);

		DrawRotaGraph(Aosuke.x, Aosuke.y, 1, 0, MonsterImage[Aosuke.ImageCount], TRUE);

		DrawRotaGraph(Guzuta.x, Guzuta.y, 1, 0, MonsterImage[Guzuta.ImageCount], TRUE);

	}
}


// 仮プレイヤーを追いかける処理
void AkabeiChasePlayer() {
	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存

	Pinkey.mx = Pinkey.x;		// Pinkeyのx座標を保存
	Pinkey.my = Pinkey.y;		// Pinkeyのy座標を保存

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

// 左右上下の当たり判定を壁が当たった時だけ判断して進む方向を決める処理
// 途中の通路には入らずに壁に当たり進む
void AkabeiMove() {
	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存
	Akabei.md = Akabei.ed;		// 敵の動く方向を保存



	// アカベイが壁を避けながら移動する処理
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

	if (Akabei.WallHit == true) {
		// アカベイが左に進んでいる場合
		if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
			if (Akabei.ed == 0) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 2) {
				Akabei.ed = 1;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
			// プレイヤーの位置がアカベイより下なら
			if (mPac.y > Akabei.y) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
			if (Akabei.ed == 0) {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 3) {
				Akabei.ed = 1;
				Akabei.WallHit = false;
			}
		}

		// アカベイが右に進んでいる場合
		else if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
			if (Akabei.ed == 1) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 2) {
				Akabei.ed = 0;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
			if (mPac.y > Akabei.y) {
				Akabei.ed = 3;
				Akabei.WallHit = false;
			}
			else {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
		}
		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
			if (Akabei.ed == 1) {
				Akabei.ed = 2;
				Akabei.WallHit = false;
			}
			else if (Akabei.ed == 3) {
				Akabei.ed = 0;
				Akabei.WallHit = false;
			}

		}
		else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
			// プレイヤーの位置がアカベイより右なら
			if (mPac.x >= Akabei.x) {
				if (Akabei.ed == 2) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
				}
			}
			else {
				if (Akabei.ed == 2) {
					Akabei.ed = 0;
					Akabei.WallHit = false;
				}
			}
		}
		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
			// プレイヤーの位置がアカベイより右なら
			if (mPac.x >= Akabei.x) {
				if (Akabei.ed == 3) {
					Akabei.ed = 1;
					Akabei.WallHit = false;
				}
			}
			else {
				if (Akabei.ed == 3) {
					Akabei.ed = 0;
					Akabei.WallHit = false;
				}
			}
		}
	}
	//else {
	//	if (Akabei.ed == 0) {
	//		if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x > Akabei.x) {
	//				Akabei.ed = 1;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x < Akabei.x) {
	//				Akabei.ed = 0;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//			if (mPac.y < Akabei.y) {
	//				Akabei.ed = 3;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//			if (mPac.y > Akabei.y) {
	//				Akabei.ed = 2;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {

	//		}
	//	}
	//	else if (Akabei.ed == 1) {
	//		if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x > Akabei.x) {
	//				Akabei.ed = 1;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x < Akabei.x) {
	//				Akabei.ed = 0;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//			if (mPac.y < Akabei.y) {
	//				Akabei.ed = 3;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//			if (mPac.y > Akabei.y) {
	//				Akabei.ed = 2;
	//			}
	//		}
	//	}
	//	else if (Akabei.ed == 2) {
	//		if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x > Akabei.x) {
	//				Akabei.ed = 1;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x < Akabei.x) {
	//				Akabei.ed = 0;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//			if (mPac.y < Akabei.y) {
	//				Akabei.ed = 3;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//			if (mPac.y > Akabei.y) {
	//				Akabei.ed = 2;
	//			}
	//		}
	//	}
	//	else if (Akabei.ed == 3) {
	//		if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x > Akabei.x) {
	//				Akabei.ed = 1;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
	//			if (mPac.x < Akabei.x) {
	//				Akabei.ed = 0;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//			if (mPac.y < Akabei.y) {
	//				Akabei.ed = 3;
	//			}
	//		}
	//		else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
	//			if (mPac.y > Akabei.y) {
	//				Akabei.ed = 2;
	//			}
	//		}
	//	}
	//}
}

void PinkeyMove() {

}

void AosukeMove() {
	Aosuke.mx = Aosuke.x;		// アオスケのx座標を保存
	Aosuke.my = Aosuke.y;		// アオスケのy座標を保存
	Aosuke.md = Aosuke.ed;		// アオスケの動く方向を保存



	// アオスケが壁を避けながら移動する処理
	switch (Aosuke.ed) {
	case 0:	// 左へ移動
		Aosuke.x--;
		Aosuke.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Aosuke.x++;
		Aosuke.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Aosuke.y--;
		Aosuke.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Aosuke.y++;
		Aosuke.eyeImageCount = 2;
		break;
	}

	if (DotCnt < 30) {
		if (Aosuke.WallHit == true) {
			if (Aosuke.ed == 2) {
				Aosuke.ed = 3;
				Aosuke.WallHit = false;
			}
			else if (Aosuke.ed == 3) {
				Aosuke.ed = 2;
				Aosuke.WallHit = false;
			}
		}
	}
	else {
		// MapDtaa[15][15]を目指して、MapData[12][15]を目指す
		if (Aosuke.x / 16 != 15.0f) {
			Aosuke.ed = 1;
		}
		else {
			Aosuke.ed = 2;
		}
	}
}

void GuzutaMove() {
	Guzuta.mx = Guzuta.x;		// グズタのx座標を保存
	Guzuta.my = Guzuta.y;		// グズタのy座標を保存
	Guzuta.md = Guzuta.ed;		// グズタの動く方向を保存



	// グズタが壁を避けながら移動する処理
	switch (Guzuta.ed) {
	case 0:	// 左へ移動
		Guzuta.x--;
		Guzuta.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Guzuta.x++;
		Guzuta.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Guzuta.y--;
		Guzuta.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Guzuta.y++;
		Guzuta.eyeImageCount = 2;
		break;
	}

	if (DotCnt < 90) {
		if (Guzuta.WallHit == true) {
			if (Guzuta.ed == 2) {
				Guzuta.ed = 3;
				Guzuta.WallHit = false;
			}
			else if (Guzuta.ed == 3) {
				Guzuta.ed = 2;
				Guzuta.WallHit = false;
			}
		}
	}
	else {
		// MapDtaa[15][15]を目指して、MapData[12][15]を目指す
		if (Guzuta.x / 16 != 15.0f) {
			Guzuta.ed = 0;
		}
		else {
			Guzuta.ed = 2;
		}
	}
}

void ModeChange() {
	if (EnemyMode == true) {//縄張りモード
		ChaseModeTime = 1230;
		if (ScatterModeTime > 0 != PowerUpFlg) {
			ScatterModeTime--;
		}
		else if (ScatterModeTime == 0) {
			EnemyMode = false;
		}
	}
	else if (EnemyMode == false) {//追跡モード
		ScatterModeTime = 480;
		if (ChaseModeTime > 0 != PowerUpFlg) {
			ChaseModeTime--;
		}
		else if (ChaseModeTime == 0) {
			EnemyMode = true;
		}
	}
}

void ScatterMode() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, Pinkey.w, Pinkey.h,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {

				if (MapData[i][j] == 1) {
					Pinkey.x = Pinkey.mx;
					Pinkey.y = Pinkey.my;
				}
			}
		}
	}
}