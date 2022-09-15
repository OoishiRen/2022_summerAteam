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

	// ピンキーの初期化
	Pinkey.x = 240.0f;
	Pinkey.y = 240.0f;
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

	DrawFormatString(500, 30, GetColor(255, 255, 255),
		EnemyMode ? "Scatter %d" : "Chase %d", EnemyMode ? ScatterModeTime : ChaseModeTime);


	// アニメーション
	if (!PowerUpFlg) {

		if (Akabei.ImageCount == 0) {
			Akabei.ImageCount = 1;

		}
		else {
			Akabei.ImageCount = 0;
		}

		if (eCnt < ENEMY_CNT_SPEED) {
			eCnt++;
		}
		else if (eCnt == ENEMY_CNT_SPEED) {
			eCnt = 0;
		}
		if (eCnt < ENEMY_CNT_SPEED / 2) {
			Pinkey.ImageCount = 3;
		}
		else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
			Pinkey.ImageCount = 2;
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

	ModeChange();

	AkabeiMove2();
}


// 描画
void Enemy_Draw() {

	if (!PowerUpFlg) {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, EyeImage[Akabei.eyeImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);
		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, EyeImage[Pinkey.eyeImageCount], TRUE);
	}
	else {
		DrawRotaGraph(Akabei.x, Akabei.y, 1, 0, MonsterImage[Akabei.ImageCount], TRUE);

		DrawRotaGraph(Pinkey.x, Pinkey.y, 1, 0, MonsterImage[Pinkey.ImageCount], TRUE);
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

// 左右上下の当たり判定を壁が当たった時だけ判断して進む方向を決める処理
// 途中の通路には入らずに壁に当たり進む
void AkabeiMove2() {
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
	//	if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
	//		if (Akabei.ed == 0) {
	//			if (mPac.y > Akabei.y) {
	//				Akabei.ed = 3;
	//				Akabei.WallHit = false;
	//			}
	//		}
	//		else if (Akabei.ed == 2) {
	//			if (mPac.x > Akabei.x) {
	//				Akabei.ed = 1;
	//				Akabei.WallHit = false;
	//			}
	//			else {
	//				Akabei.ed = 0;
	//				Akabei.WallHit = false;
	//			}
	//		}
	//	}
	//}
}


void ModeChange() {
	if (EnemyMode == true) {//縄張りモード
		ChaseModeTime = 1230;
		if (ScatterModeTime > 0 != PowerUpFlg) {
			ScatterMode();
			ScatterModeTime--;
		}
		else if (PowerUpFlg == true) {
			ScatterMode();
		}
		else if (ScatterModeTime == 0) {
			EnemyMode = false;
		}
	}
	else if (EnemyMode == false) {//追跡モード
		ScatterModeTime = 480;
		if (ChaseModeTime > 0 != PowerUpFlg) {
			ChaseMode();
			//ScatterMode();
			ChaseModeTime--;
		}
		else if (PowerUpFlg == true) {
			ChaseMode();
			//ScatterMode();
		}
		else if (ChaseModeTime == 0) {
			EnemyMode = true;
		}
	}
}

void ScatterMode() {

	int ax, ay, bx, by, cx, cy;
	int difx, dify, difc;
	int goal;
	ax = 2 * MAP_SIZE;
	ay = 2 * MAP_SIZE;

	bx = Pinkey.x;
	by = Pinkey.y;

	Pinkey.mx = Pinkey.x;		// アカベイのx座標を保存
	Pinkey.my = Pinkey.y;		// アカベイのy座標を保存
	Pinkey.md = Pinkey.ed;		// 敵の動く方向を保存
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(2 * MAP_SIZE + 4, 2 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));


			}
		}
	}
	 //ピンキーが壁を避けながら移動する処理
	switch (Pinkey.ed) {
	case 0:	// 左へ移動
		Pinkey.x -= Pinkey.speed;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Pinkey.x += Pinkey.speed;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Pinkey.y -= Pinkey.speed;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Pinkey.y += Pinkey.speed;
		Pinkey.eyeImageCount = 2;
		break;
	}
	if (Pinkey.WallHit == true) {//壁に当たったら
		if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			if (Pinkey.ed == 0) {// ピンキーが左に進んでいる場合
				Pinkey.ed = 3;//下へ移動
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 2) {//上へ移動している場合
				Pinkey.ed = 1;//右へ移動
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
			// 縄張りの位置がピンキーより下なら
			if (ay > Pinkey.y) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
			if (Pinkey.ed == 0) {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 3) {
				Pinkey.ed = 1;
				Pinkey.WallHit = false;
			}
		}
		// ピンキー右に進んでいる場合
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
			if (Pinkey.ed == 1) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 2) {
				Pinkey.ed = 0;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
			//縄張りの位置がピンキーより上なら
			if (ay > Pinkey.y) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
			if (Pinkey.ed == 1) {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 3) {
				Pinkey.ed = 0;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			// 縄張りの位置がピンキーより右なら
			if (ax >= Pinkey.x) {
				if (Pinkey.ed == 2) {
					Pinkey.ed = 1;
					Pinkey.WallHit = false;
				}
			}
			else {
				if (Pinkey.ed == 2) {
					Pinkey.ed = 0;
					Pinkey.WallHit = false;
				}
			}
		}
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
			// 縄張りの位置がピンキーより右なら
			if (ax >= Pinkey.x) {
				if (Pinkey.ed == 3) {
					Pinkey.ed = 1;
					Pinkey.WallHit = false;
				}
			}
			else {
				if (Pinkey.ed == 3) {
					Pinkey.ed = 0;
					Pinkey.WallHit = false;
				}
			}
		}
	}
}

void ChaseMode() {
	int ax, ay, bx, by, cx, cy;

	ax = 2 * MAP_SIZE;
	ay = 2 * MAP_SIZE;

	bx = Pinkey.x;
	by = Pinkey.y;


	Pinkey.mx = Pinkey.x;		// アカベイのx座標を保存
	Pinkey.my = Pinkey.y;		// アカベイのy座標を保存
	Pinkey.md = Pinkey.ed;		// 敵の動く方向を保存


	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(mPac.x, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
			}
		}
	}


	// アカベイが壁を避けながら移動する処理
	switch (Pinkey.ed) {
	case 0:	// 左へ移動
		Pinkey.x -= Pinkey.speed;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Pinkey.x += Pinkey.speed;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Pinkey.y -= Pinkey.speed;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Pinkey.y += Pinkey.speed;
		Pinkey.eyeImageCount = 2;
		break;
	}

	if (Pinkey.WallHit == true) {
		// アカベイが左に進んでいる場合
		if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			if (Pinkey.ed == 0) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 2) {
				Pinkey.ed = 1;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
			// プレイヤーの位置がアカベイより下なら
			if (mPac.y > Pinkey.y) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
			if (Pinkey.ed == 0) {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 3) {
				Pinkey.ed = 1;
				Pinkey.WallHit = false;
			}
		}

		// アカベイが右に進んでいる場合
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
			if (Pinkey.ed == 1) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 2) {
				Pinkey.ed = 0;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
			if (mPac.y > Pinkey.y) {
				Pinkey.ed = 3;
				Pinkey.WallHit = false;
			}
			else {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
		}
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
			if (Pinkey.ed == 1) {
				Pinkey.ed = 2;
				Pinkey.WallHit = false;
			}
			else if (Pinkey.ed == 3) {
				Pinkey.ed = 0;
				Pinkey.WallHit = false;
			}

		}
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			// プレイヤーの位置がアカベイより右なら
			if (mPac.x >= Pinkey.x) {
				if (Pinkey.ed == 2) {
					Pinkey.ed = 1;
					Pinkey.WallHit = false;
				}
			}
			else {
				if (Pinkey.ed == 2) {
					Pinkey.ed = 0;
					Pinkey.WallHit = false;
				}
			}
		}
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
			// プレイヤーの位置がアカベイより右なら
			if (mPac.x >= Pinkey.x) {
				if (Pinkey.ed == 3) {
					Pinkey.ed = 1;
					Pinkey.WallHit = false;
				}
			}
			else {
				if (Pinkey.ed == 3) {
					Pinkey.ed = 0;
					Pinkey.WallHit = false;
				}
			}
		}
	}
}