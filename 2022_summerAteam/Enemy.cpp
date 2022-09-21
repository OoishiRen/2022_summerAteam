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

int px, py;

float A, B, C;		// 三平方の定理用の変数
float dx, dy;		// 正規化用変数

int eCnt;

int ScatterModeTime;//縄張りモードの時間（フレーム）
int ChaseModeTime;//追跡モードの時間（フレーム）
bool EnemyMode; // True = 縄張りモード、false = 追跡モード
int IjikeCnt;
bool Run;

// 初期化
void Enemy_Initialize() {
	LoadDivGraph("enemy_images/monster.png", 20, 20, 1, 16, 16, MonsterImage); // モンスターの画像を読み込む
	LoadDivGraph("enemy_images/eyes.png", 4, 4, 1, 16, 16, EyeImage);		   // 目玉の画像を読み込む

	// アカベイの初期化
	Akabei.x = 240.0f;
	Akabei.y = 200.0f;
	Akabei.w = 16.0f;
	Akabei.h = 16.0f;
	Akabei.ed = 0;
	Akabei.ImageCount = 0;
	Akabei.eyeImageCount = 3;
	Akabei.speed = 2.0f;
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

	// アオスケの初期化
	Aosuke.x = 200.0f;
	Aosuke.y = 260.0f;
	Aosuke.w = ENEMY_SIZE;
	Aosuke.h = ENEMY_SIZE;
	Aosuke.ed = 2;
	Aosuke.ImageCount = 4;
	Aosuke.eyeImageCount = 0;
	Aosuke.speed = 2.0f;
	Aosuke.WallHit = false;
	Aosuke.left = false;
	Aosuke.right = false;
	Aosuke.up = false;
	Aosuke.bottom = false;
	Aosuke.movefinish = false;

	// グズタの初期化
	Guzuta.x = 280.0f;
	Guzuta.y = 260.0f;
	Guzuta.w = ENEMY_SIZE;
	Guzuta.h = ENEMY_SIZE;
	Guzuta.ed = 2;
	Guzuta.ImageCount = 6;
	Guzuta.eyeImageCount = 0;
	Guzuta.speed = 2.0f;
	Guzuta.WallHit = false;
	Guzuta.left = false;
	Guzuta.right = false;
	Guzuta.up = false;
	Guzuta.bottom = false;
	Guzuta.movefinish = false;

	eCnt = 0;
	ScatterModeTime = 480;
	ChaseModeTime = 1230;
	EnemyMode = true;
	IjikeCnt = 20;
	Run = false;
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

	//DrawFormatString(1000, 110, 255, "dy = %.1f", dy);
	//DrawFormatString(1000, 130, 255, "md = %d", Akabei.md);
	//DrawFormatString(1000, 150, 255, "ed = %d", Akabei.ed);
	DrawFormatString(1000, 170, 255, "Akabei.x = %.1f", Akabei.x);
	DrawFormatString(1000, 190, 255, "Akabei.y = %.1f", Akabei.y);
	DrawFormatString(1000, 210, 255, "Akabei.x = %.1f", Akabei.x / 16);
	DrawFormatString(1000, 230, 255, "Akabei.y = %.1f", Akabei.y / 16);
	DrawFormatString(1000, 250, 255, "Akabei.WallHit = %d", Akabei.WallHit);
	DrawFormatString(1000, 270, 255, "Akabei.left = %d", Akabei.left);
	DrawFormatString(1000, 290, 255, "Akabei.right = %d", Akabei.right);
	DrawFormatString(1000, 310, 255, "Akabei.up = %d", Akabei.up);
	DrawFormatString(1000, 330, 255, "Akabei.bottom = %d", Akabei.bottom);
	DrawFormatString(1000, 350, 255, "Akabei.mapY = %d", Akabei.mapY);
	DrawFormatString(1000, 370, 255, "%d", IjikeCnt);


	DrawFormatString(500, 30, GetColor(255, 255, 255),
		EnemyMode ? "Scatter %d" : "Chase %d", EnemyMode ? ScatterModeTime : ChaseModeTime);

	PinkeyWarp();

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
			//if (Akabei.ImageCount == 16) {
			//	Akabei.ImageCount = 17;
			//	Aosuke.ImageCount = 17;
			//	Guzuta.ImageCount = 17;
			//}
			//else {
			//	Akabei.ImageCount = 16;
			//	Aosuke.ImageCount = 16;
			//	Guzuta.ImageCount = 16;
			//}


			if (eCnt < ENEMY_CNT_SPEED) {
				eCnt++;
			}
			else if (eCnt == ENEMY_CNT_SPEED) {
				eCnt = 0;
			}
			if (eCnt < ENEMY_CNT_SPEED / 2) {
				Akabei.ImageCount = 16;
				Pinkey.ImageCount = 16;
				Aosuke.ImageCount = 16;
				Guzuta.ImageCount = 16;
			}
			else if (eCnt > ENEMY_CNT_SPEED / 2 && eCnt < ENEMY_CNT_SPEED) {
				Akabei.ImageCount = 17;
				Pinkey.ImageCount = 17;
				Aosuke.ImageCount = 17;
				Guzuta.ImageCount = 17;
			}

		}
		else {
			if (Akabei.ImageCount < 19) {
				if (PowerUpTime % 5 == 0) {
					Akabei.ImageCount++;
					Pinkey.ImageCount++;
					Aosuke.ImageCount++;
					Guzuta.ImageCount++;
				}
			}
			else
			{
				Akabei.ImageCount = 16;
				Pinkey.ImageCount = 16;
				Aosuke.ImageCount = 16;
				Guzuta.ImageCount = 16;
			}
		}


	}

	ModeChange();
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
void AkabeiChaseMode() {
	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存
	Akabei.md = Akabei.ed;		// 敵の動く方向を保存

	Akabei.mapX = (int)Akabei.x / 16;
	Akabei.mapY = (int)Akabei.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Akabei.x;

	B = mPac.y - Akabei.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	DrawLine(mPac.x, mPac.y, Akabei.x, Akabei.y, GetColor(255, 0, 0));

	// アカベイが壁を避けながら移動する処理
	switch (Akabei.ed) {
	case 0:	// 左へ移動
		Akabei.x -= Akabei.speed;
		Akabei.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Akabei.x += Akabei.speed;
		Akabei.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Akabei.y -= Akabei.speed;
		Akabei.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Akabei.y += Akabei.speed;
		Akabei.eyeImageCount = 2;
		break;
	}

	// 左と上に壁があった場合
	if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Akabei.md == 0) {
				Akabei.ed = 3;				// 下に方向を変える
				Akabei.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Akabei.md == 2) {
				Akabei.ed = 1;				// 右に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Akabei.md == 0) {
				Akabei.ed = 2;				// 上に方向を変える
				Akabei.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Akabei.md == 3) {
				Akabei.ed = 1;				// 右に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Akabei.md == 1) {
				Akabei.ed = 3;				// 下に方向を変える
				Akabei.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Akabei.md == 2) {	// 上に進んでいたら
				Akabei.ed = 0;				// 左に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Akabei.md == 1) {	// 右に進んでいたら
				Akabei.ed = 2;				// 上に方向を変える
				Akabei.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Akabei.md == 3) {	// 下に進んでいたら
				Akabei.ed = 0;				// 左に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			if (Akabei.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					Akabei.ed = 1;				// 右に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;				// 左に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// 壁に当たった場合
		if (Akabei.WallHit == true) {
			// 上から入ってきたら
			if (Akabei.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					Akabei.ed = 1;				// 右に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;				// 左に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより上なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			if (Akabei.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					Akabei.ed = 3;				// 下に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 2;				// 上に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			if (Akabei.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					Akabei.ed = 3;				// 下に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 2;				// 上に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 2) {	// 上に移動していたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 3) {	// 下に移動していたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// 壁に当たっていなかったら
		if (Akabei.WallHit == false)
		{		// アカベイが進んでいる方向によって処理を決める
			switch (Akabei.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (dy < 0.0f) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (dy < 0.0f) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (dx < 0.0f) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (dx < 0.0f) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void AkabeiScatterMode() {

	int NawabariX, NawabariY;

	NawabariX = MAP_SIZE * 28;
	NawabariY = MAP_SIZE * 2;

	Akabei.mx = Akabei.x;		// アカベイのx座標を保存
	Akabei.my = Akabei.y;		// アカベイのy座標を保存
	Akabei.md = Akabei.ed;		// 敵の動く方向を保存

	Akabei.mapX = (int)Akabei.x / 16;
	Akabei.mapY = (int)Akabei.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Akabei.x;

	B = mPac.y - Akabei.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	DrawLine(NawabariX, NawabariY, Akabei.x, Akabei.y, GetColor(255, 0, 0));


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

	// 左と上に壁があった場合
	if (Akabei.left == true && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Akabei.md == 0) {
				Akabei.ed = 3;				// 下に方向を変える
				Akabei.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Akabei.md == 2) {
				Akabei.ed = 1;				// 右に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Akabei.md == 0) {
				Akabei.ed = 2;				// 上に方向を変える
				Akabei.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Akabei.md == 3) {
				Akabei.ed = 1;				// 右に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == true && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Akabei.md == 1) {
				Akabei.ed = 3;				// 下に方向を変える
				Akabei.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Akabei.md == 2) {	// 上に進んでいたら
				Akabei.ed = 0;				// 左に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == true) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Akabei.md == 1) {	// 右に進んでいたら
				Akabei.ed = 2;				// 上に方向を変える
				Akabei.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Akabei.md == 3) {	// 下に進んでいたら
				Akabei.ed = 0;				// 左に方向を変える
				Akabei.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == true && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			if (Akabei.md == 2) {	// 上に進んでいたら
				// 
				if (NawabariX > Akabei.x) {
					Akabei.ed = 1;				// 右に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;				// 左に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Akabei.y) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Akabei.y) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == true) {
		// 壁に当たった場合
		if (Akabei.WallHit == true) {
			// 上から入ってきたら
			if (Akabei.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Akabei.x) {
					Akabei.ed = 1;				// 右に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 0;				// 左に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY < Akabei.y) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより上なら
				if (NawabariY < Akabei.y) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Akabei.left == false && Akabei.right == true && Akabei.up == false && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			if (Akabei.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Akabei.y) {
					Akabei.ed = 3;				// 下に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 2;				// 上に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (NawabariX < Akabei.x) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (NawabariX < Akabei.x) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// 壁に当たったら
		if (Akabei.WallHit == true) {
			if (Akabei.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Akabei.y) {
					Akabei.ed = 3;				// 下に方向を変える
					Akabei.WallHit = false;
				}
				else {
					Akabei.ed = 2;				// 上に方向を変える
					Akabei.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Akabei.md == 2) {	// 上に移動していたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Akabei.x) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
			else if (Akabei.md == 3) {	// 下に移動していたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Akabei.x) {
					// 壁に引っかからずに曲がれるように
					if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Akabei.left == false && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
		// 壁に当たっていなかったら
		if (Akabei.WallHit == false)
		{		// アカベイが進んでいる方向によって処理を決める
			switch (Akabei.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.x / 16.0f == (float)Akabei.mapX - 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (NawabariY < Akabei.y) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.x / 16.0f == (float)Akabei.mapX + 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (NawabariY < Akabei.y) {
						Akabei.ed = 2;				// 上に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 3;				// 下に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.y / 16.0f == (float)Akabei.mapY - 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (NawabariX < Akabei.x) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Akabei.y / 16.0f == (float)Akabei.mapY + 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (NawabariX < Akabei.x) {
						Akabei.ed = 0;				// 左に方向を変える
						Akabei.WallHit = false;
						break;
					}
					else {
						Akabei.ed = 1;				// 右に方向を変える
						Akabei.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void ModeChange() {
	if (EnemyMode == true) {//縄張りモード
		ChaseModeTime = 1230;//追跡モードの時間を初期化
		//縄張りモードの時間があるとき&&パックマンがパワーアップ上代じゃない時
		if (ScatterModeTime > 0 != PowerUpFlg) {
			// アカベイの縄張りモード
			AkabeiScatterMode();

			// ピンキーの縄張りモード
			ScatterMode();

			// アオスケの縄張りモード
			if (Aosuke.movefinish == false) {
				AosukeMove();
			}
			else {
				AosukeScatterMode();
			}

			// グズタの縄張りモード
			if (Guzuta.movefinish == false) {
				GuzutaMove();
			}
			else {
				GuzutaScatterMode();
			}

			ScatterModeTime--;
		}
		//パックマンがパワーアップモードの時
		else if (PowerUpFlg == true) {
			// アカベイの縄張りモード
			AkabeiScatterMode();

			// アオスケの縄張りモード
			if (Aosuke.movefinish == false) {
				AosukeMove();
			}
			else {
				AosukeScatterMode();
			}

			// グズタの縄張りモード
			if (Guzuta.movefinish == false) {
				GuzutaMove();
			}
			else {
				GuzutaScatterMode();
			}
			//イジケ状態にする
			IjikeMode();
		}//縄張りモードの時間が無くなったら
		else if (ScatterModeTime == 0) {
			EnemyMode = false;//追跡モードにする
		}
	}
	else if (EnemyMode == false) {//追跡モード
		ScatterModeTime = 480;//縄張りモードの時間を初期化
		//追跡モードの時間があるとき&&パックマンがパワーアップ上代じゃない時
		if (ChaseModeTime > 0 != PowerUpFlg) {
			// アカベイの追跡モード
			AkabeiChaseMode();

			// ピンキーの追跡モード
			ChaseMode();

			// アオスケの追跡モード
			if (Aosuke.movefinish == false) {
				AosukeMove();
			}
			else {
				AosukeChaseMode();
			}

			// グズタの追跡モード
			if (Guzuta.movefinish == false) {
				GuzutaMove();
			}
			else {
				GuzutaChaseMode();
			}
			ChaseModeTime--;//追跡モードの時間を減らす
		}
		//パックマンがパワーアップモードの時
		else if (PowerUpFlg == true) {

			// アカベイの縄張りモード
			AkabeiScatterMode();

			// アオスケの縄張りモード
			if (Aosuke.movefinish == false) {
				AosukeMove();
			}
			else {
				AosukeScatterMode();
			}

			// グズタの縄張りモード
			if (Guzuta.movefinish == false) {
				GuzutaMove();
			}
			else {
				GuzutaScatterMode();
			}
			//イジケ状態にする
			IjikeMode();
		}//追跡モードの時間が無くなったら
		else if (ChaseModeTime == 0) {
			EnemyMode = true;//縄張りモードにする
		}
	}
}

void ScatterMode() {
	Pinkey.mx = Pinkey.x;		// ピンキーのx座標を保存
	Pinkey.my = Pinkey.y;		// ピンキーのy座標を保存
	Pinkey.md = Pinkey.ed;		// 敵の動く方向を保存

	Pinkey.mapX = (int)Pinkey.x / 16;
	Pinkey.mapY = (int)Pinkey.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = 2 * MAP_SIZE - Pinkey.x;

	B = 2 * MAP_SIZE - Pinkey.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				DrawLine(2 * MAP_SIZE + 4, 2 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));


			}
		}
	}
	// ピンキーが壁を避けながら移動する処理
	switch (Pinkey.ed) {
	case 0:	// 左へ移動
		Pinkey.x--;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Pinkey.x++;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Pinkey.y--;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Pinkey.y++;
		Pinkey.eyeImageCount = 2;
		break;
	}
	// 左と上に壁があった場合
	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Pinkey.md == 0) {
				Pinkey.ed = 3;				// 下に方向を変える
				Pinkey.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Pinkey.md == 2) {
				Pinkey.ed = 1;				// 右に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Pinkey.md == 0) {
				Pinkey.ed = 2;				// 上に方向を変える
				Pinkey.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Pinkey.md == 3) {
				Pinkey.ed = 1;				// 右に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Pinkey.md == 1) {
				Pinkey.ed = 3;				// 下に方向を変える
				Pinkey.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Pinkey.md == 2) {	// 上に進んでいたら
				Pinkey.ed = 0;				// 左に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Pinkey.md == 1) {	// 右に進んでいたら
				Pinkey.ed = 2;				// 上に方向を変える
				Pinkey.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Pinkey.md == 3) {	// 下に進んでいたら
				Pinkey.ed = 0;				// 左に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 2) {	// 上に進んでいたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// 右に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// 左に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 0) {	// 左に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// 右に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// 壁に当たった場合
		if (Pinkey.WallHit == true) {
			// 上から入ってきたら
			if (Pinkey.md == 3) {	// 下に進んでいたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// 右に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// 左に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 0) {	// 左に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// 右に進んでいたら
				// パックマンの位置がピンキーより上なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 1) {	// 右に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// 下に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// 上に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 2) {	// 上に進んでいたら
				// パックマンの位置がピンキーより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// 下に進んでいたら
				// パックマンの位置がピンキーより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 0) {	// 左に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// 下に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// 上に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 2) {	// 上に移動していたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// 下に移動していたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// 壁に当たっていなかったら
		if (Pinkey.WallHit == false)
		{		// ピンキーが進んでいる方向によって処理を決める
			switch (Pinkey.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
					// パックマンの位置がピンキーより上なら
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
					// パックマンの位置がピンキーより上なら
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
					// パックマンの位置がピンキーより左なら
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
					// パックマンの位置がピンキーより左なら
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void ChaseMode() {

	Pinkey.mx = Pinkey.x;		// ピンキーのx座標を保存
	Pinkey.my = Pinkey.y;		// ピンキーのy座標を保存
	Pinkey.md = Pinkey.ed;		// 敵の動く方向を保存

	Pinkey.mapX = (int)Pinkey.x / 16;
	Pinkey.mapY = (int)Pinkey.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	if (mPac.var == 3) {

		A = mPac.x - 4 * MAP_SIZE - Pinkey.x;
		B = mPac.y - Pinkey.y;
	}//左
	else if (mPac.var == 1) {
		A = mPac.x + 4 * MAP_SIZE - Pinkey.x;
		B = mPac.y - Pinkey.y;
	}//右
	else if (mPac.var == 0) {
		A = mPac.x - Pinkey.x;
		B = mPac.y - 4 * MAP_SIZE - Pinkey.y;
	}//上
	else if (mPac.var == 2) {
		A = mPac.x - Pinkey.x;
		B = mPac.y + 4 * MAP_SIZE - Pinkey.y;
	}//下

	/*A = mPac.x - Pinkey.x;
	B = mPac.y - Pinkey.y;*/

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
				j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				if (mPac.var == 3) {
					DrawLine(mPac.x - 4 * MAP_SIZE, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
				}//左
				else if (mPac.var == 1) {
					DrawLine(mPac.x + 4 * MAP_SIZE, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
				}//右
				else if (mPac.var == 0) {
					DrawLine(mPac.x, mPac.y - 4 * MAP_SIZE, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
				}//上
				else if (mPac.var == 2) {
					DrawLine(mPac.x, mPac.y + 4 * MAP_SIZE, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
				}//下
				//DrawLine(mPac.x, mPac.y, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));


			}
		}
	}
	// ピンキーが壁を避けながら移動する処理
	switch (Pinkey.ed) {
	case 0:	// 左へ移動
		Pinkey.x--;
		Pinkey.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Pinkey.x++;
		Pinkey.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Pinkey.y--;
		Pinkey.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Pinkey.y++;
		Pinkey.eyeImageCount = 2;
		break;
	}
	// 左と上に壁があった場合
	if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Pinkey.md == 0) {
				Pinkey.ed = 3;				// 下に方向を変える
				Pinkey.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Pinkey.md == 2) {
				Pinkey.ed = 1;				// 右に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Pinkey.md == 0) {
				Pinkey.ed = 2;				// 上に方向を変える
				Pinkey.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Pinkey.md == 3) {
				Pinkey.ed = 1;				// 右に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Pinkey.md == 1) {
				Pinkey.ed = 3;				// 下に方向を変える
				Pinkey.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Pinkey.md == 2) {	// 上に進んでいたら
				Pinkey.ed = 0;				// 左に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Pinkey.md == 1) {	// 右に進んでいたら
				Pinkey.ed = 2;				// 上に方向を変える
				Pinkey.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Pinkey.md == 3) {	// 下に進んでいたら
				Pinkey.ed = 0;				// 左に方向を変える
				Pinkey.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 2) {	// 上に進んでいたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// 右に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// 左に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 0) {	// 左に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// 右に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		// 壁に当たった場合
		if (Pinkey.WallHit == true) {
			// 上から入ってきたら
			if (Pinkey.md == 3) {	// 下に進んでいたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					Pinkey.ed = 1;				// 右に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 0;				// 左に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 0) {	// 左に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 1) {	// 右に進んでいたら
				// パックマンの位置がピンキーより上なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 1) {	// 右に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// 下に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// 上に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 2) {	// 上に進んでいたら
				// パックマンの位置がピンキーより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// 下に進んでいたら
				// パックマンの位置がピンキーより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// 壁に当たったら
		if (Pinkey.WallHit == true) {
			if (Pinkey.md == 0) {	// 左に進んでいたら
				// パックマンの位置がピンキーより下なら
				if (dy > 0.0f) {
					Pinkey.ed = 3;				// 下に方向を変える
					Pinkey.WallHit = false;
				}
				else {
					Pinkey.ed = 2;				// 上に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Pinkey.md == 2) {	// 上に移動していたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else if (Pinkey.md == 3) {	// 下に移動していたら
				// パックマンの位置がピンキーより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		// 壁に当たっていなかったら
		if (Pinkey.WallHit == false)
		{		// ピンキーが進んでいる方向によって処理を決める
			switch (Pinkey.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
					// パックマンの位置がピンキーより上なら
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
					// パックマンの位置がピンキーより上なら
					if (dy < 0.0f) {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
					// パックマンの位置がピンキーより左なら
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
					// パックマンの位置がピンキーより左なら
					if (dx < 0.0f) {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
						break;
					}
					else {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void IjikeMode() {
	Pinkey.mx = Pinkey.x;		// ピンキーのx座標を保存
	Pinkey.my = Pinkey.y;		// ピンキーのy座標を保存
	Pinkey.md = Pinkey.ed;		// 敵の動く方向を保存

	Pinkey.mapX = (int)Pinkey.x / 16;
	Pinkey.mapY = (int)Pinkey.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Pinkey.x;

	B = mPac.y - Pinkey.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る
	// ピンキーが壁を避けながら移動する処理
	if (!Run) {
		if (IjikeCnt != 0) {
			IjikeCnt--;
			switch (Pinkey.ed) {
			case 0:	// 左へ移動
				Pinkey.x++;
				Pinkey.eyeImageCount = 3;
				break;
			case 1:	// 右へ移動
				Pinkey.x--;
				Pinkey.eyeImageCount = 1;
				break;
			case 2:	// 上へ移動
				Pinkey.y++;
				Pinkey.eyeImageCount = 0;
				break;
			case 3:	// 下へ移動
				Pinkey.y--;
				Pinkey.eyeImageCount = 2;
				break;
			}
		}
		else {
			switch (Pinkey.ed) {
			case 0:	// 左へ移動
				Pinkey.x--;
				Pinkey.eyeImageCount = 3;
				break;
			case 1:	// 右へ移動
				Pinkey.x++;
				Pinkey.eyeImageCount = 1;
				break;
			case 2:	// 上へ移動
				Pinkey.y--;
				Pinkey.eyeImageCount = 0;
				break;
			case 3:	// 下へ移動
				Pinkey.y++;
				Pinkey.eyeImageCount = 2;
				break;
			}
		}
		// 左と上に壁があった場合
		if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				// 右から入ってきたら、下に方向を変える
				if (Pinkey.md == 0) {
					Pinkey.ed = 3;				// 下に方向を変える
					Pinkey.WallHit = false;
				}
				// 下から入ってきたら、右に方向を変える
				else if (Pinkey.md == 2) {
					Pinkey.ed = 1;				// 右に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		// 左と下に壁があったら
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				// 右から入ってきたら、上に方向を変える
				if (Pinkey.md == 0) {
					Pinkey.ed = 2;				// 上に方向を変える
					Pinkey.WallHit = false;
				}
				// 上から入ってきたら、右に方向を変える
				else if (Pinkey.md == 3) {
					Pinkey.ed = 1;				// 右に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		// 右と上に壁がある場合
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				// 左から入ってきたら、下に方向を変える
				if (Pinkey.md == 1) {
					Pinkey.ed = 3;				// 下に方向を変える
					Pinkey.WallHit = false;
				}
				// 下から入ってきたら、左に方向を変える
				else if (Pinkey.md == 2) {	// 上に進んでいたら
					Pinkey.ed = 0;				// 左に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		// 右と下に壁がある場合
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				// 左から入ってきたら、上に方向を変える
				if (Pinkey.md == 1) {	// 右に進んでいたら
					Pinkey.ed = 2;				// 上に方向を変える
					Pinkey.WallHit = false;
				}
				// 上から入ってきたら、左に方向を変える
				else if (Pinkey.md == 3) {	// 下に進んでいたら
					Pinkey.ed = 0;				// 左に方向を変える
					Pinkey.WallHit = false;
				}
			}
		}
		// 上だけに壁があった場合
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				if (Pinkey.md == 2) {	// 上に進んでいたら
					// パックマンの位置がピンキーより右なら
					if (dx > 0.0f) {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
					}
					else {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else {	// 壁に当たっていなかったら
				if (Pinkey.md == 0) {	// 左に進んでいたら
					// パックマンの位置がピンキーより下なら
					if (dy > 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
							Pinkey.ed = 3;				// 下に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
				else if (Pinkey.md == 1) {	// 右に進んでいたら
					// パックマンの位置がピンキーより下なら
					if (dy > 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
							Pinkey.ed = 3;				// 下に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
			}
		}
		// 下だけに壁があった場合
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
			// 壁に当たった場合
			if (Pinkey.WallHit == true) {
				// 上から入ってきたら
				if (Pinkey.md == 3) {	// 下に進んでいたら
					// パックマンの位置がピンキーより右なら
					if (dx > 0.0f) {
						Pinkey.ed = 1;				// 右に方向を変える
						Pinkey.WallHit = false;
					}
					else {
						Pinkey.ed = 0;				// 左に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else {	// 壁に当たっていなかったら
				if (Pinkey.md == 0) {	// 左に進んでいたら
					// パックマンの位置がピンキーより下なら
					if (dy < 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
							Pinkey.ed = 2;				// 上に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
				else if (Pinkey.md == 1) {	// 右に進んでいたら
					// パックマンの位置がピンキーより上なら
					if (dy < 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
							Pinkey.ed = 2;				// 上に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
			}
		}
		//　右だけに壁があった場合
		else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				if (Pinkey.md == 1) {	// 右に進んでいたら
					// パックマンの位置がピンキーより下なら
					if (dy > 0.0f) {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
					}
					else {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else {	// 壁に当たっていなかったら
				if (Pinkey.md == 2) {	// 上に進んでいたら
					// パックマンの位置がピンキーより左なら
					if (dx < 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
							Pinkey.ed = 0;				// 左に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
				else if (Pinkey.md == 3) {	// 下に進んでいたら
					// パックマンの位置がピンキーより左なら
					if (dx < 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
							Pinkey.ed = 0;				// 左に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
			}
		}
		// 左だけに壁があった場合
		else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
			// 壁に当たったら
			if (Pinkey.WallHit == true) {
				if (Pinkey.md == 0) {	// 左に進んでいたら
					// パックマンの位置がピンキーより下なら
					if (dy > 0.0f) {
						Pinkey.ed = 3;				// 下に方向を変える
						Pinkey.WallHit = false;
					}
					else {
						Pinkey.ed = 2;				// 上に方向を変える
						Pinkey.WallHit = false;
					}
				}
			}
			else {	// 壁に当たっていなかったら
				if (Pinkey.md == 2) {	// 上に移動していたら
					// パックマンの位置がピンキーより右なら
					if (dx > 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
							Pinkey.ed = 1;				// 右に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
				else if (Pinkey.md == 3) {	// 下に移動していたら
					// パックマンの位置がピンキーより右なら
					if (dx > 0.0f) {
						// 壁に引っかからずに曲がれるように
						if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
							Pinkey.ed = 1;				// 右に方向を変える
							Pinkey.WallHit = false;
						}
					}
				}
			}
		}
		// 十字路の場合
		else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
			// 壁に当たっていなかったら
			if (Pinkey.WallHit == false)
			{		// ピンキーが進んでいる方向によって処理を決める
				switch (Pinkey.md) {
				case 0:	// 左に進んでいたら
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
						// パックマンの位置がピンキーより上なら
						if (dy < 0.0f) {
							Pinkey.ed = 2;				// 上に方向を変える
							Pinkey.WallHit = false;
							break;
						}
						else {
							Pinkey.ed = 3;				// 下に方向を変える
							Pinkey.WallHit = false;
							break;
						}
					}
				case 1:	// 右に進んでいたら
					// 壁に引っかからずに曲がれるように
					if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
						// パックマンの位置がピンキーより上なら
						if (dy < 0.0f) {
							Pinkey.ed = 2;				// 上に方向を変える
							Pinkey.WallHit = false;
							break;
						}
						else {
							Pinkey.ed = 3;				// 下に方向を変える
							Pinkey.WallHit = false;
							break;
						}
					}
				case 2:	// 上に進んでいたら
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
						// パックマンの位置がピンキーより左なら
						if (dx < 0.0f) {
							Pinkey.ed = 0;				// 左に方向を変える
							Pinkey.WallHit = false;
							break;
						}
						else {
							Pinkey.ed = 1;				// 右に方向を変える
							Pinkey.WallHit = false;
							break;
						}
					}
				case 3:	// 下に進んでいたら
					// 壁に引っかからずに曲がれるように
					if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
						// パックマンの位置がピンキーより左なら
						if (dx < 0.0f) {
							Pinkey.ed = 0;				// 左に方向を変える
							Pinkey.WallHit = false;
							break;
						}
						else {
							Pinkey.ed = 1;				// 右に方向を変える
							Pinkey.WallHit = false;
							break;
						}
					}
				}
			}
		}
	}
	int A2 = 15 * MAP_SIZE - Pinkey.x;
	int B2 = 15 * MAP_SIZE - Pinkey.y;
	int C2 = sqrtf(A2 * A2 + B2 * B2);	// A と B を２乗して足した値の平方根を求める
	int dx2 = A2 / C2;		// C を1（正規化）とするには、A を C で割る
	int dy2 = B2 / C2;		// C を1（正規化）とするには、B を C で割る
	if (Run == true) {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE,
					j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					DrawLine(15 * MAP_SIZE + 4, 15 * MAP_SIZE + 4, Pinkey.x, Pinkey.y, GetColor(246, 173, 198));
				}
			}
		}
		//// ピンキーが壁を避けながら移動する処理
		//switch (Pinkey.ed) {
		//case 0:	// 左へ移動
		//	Pinkey.x -= 1;
		//	Pinkey.eyeImageCount = 3;
		//	break;
		//case 1:	// 右へ移動
		//	Pinkey.x += 1;
		//	Pinkey.eyeImageCount = 1;
		//	break;
		//case 2:	// 上へ移動
		//	Pinkey.y -= 1;
		//	Pinkey.eyeImageCount = 0;
		//	break;
		//case 3:	// 下へ移動
		//	Pinkey.y += 1;
		//	Pinkey.eyeImageCount = 2;
		//	break;
		//}
		//// 左と上に壁があった場合
		//if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		// 右から入ってきたら、下に方向を変える
		//		if (Pinkey.md == 0) {
		//			Pinkey.ed = 3;				// 下に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//		// 下から入ってきたら、右に方向を変える
		//		else if (Pinkey.md == 2) {
		//			Pinkey.ed = 1;				// 右に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//	}
		//}
		//// 左と下に壁があったら
		//else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		// 右から入ってきたら、上に方向を変える
		//		if (Pinkey.md == 0) {
		//			Pinkey.ed = 2;				// 上に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//		// 上から入ってきたら、右に方向を変える
		//		else if (Pinkey.md == 3) {
		//			Pinkey.ed = 1;				// 右に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//	}
		//}
		//// 右と上に壁がある場合
		//else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == true && Pinkey.bottom == false) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		// 左から入ってきたら、下に方向を変える
		//		if (Pinkey.md == 1) {
		//			Pinkey.ed = 3;				// 下に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//		// 下から入ってきたら、左に方向を変える
		//		else if (Pinkey.md == 2) {	// 上に進んでいたら
		//			Pinkey.ed = 0;				// 左に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//	}
		//}
		//// 右と下に壁がある場合
		//else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == true) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		// 左から入ってきたら、上に方向を変える
		//		if (Pinkey.md == 1) {	// 右に進んでいたら
		//			Pinkey.ed = 2;				// 上に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//		// 上から入ってきたら、左に方向を変える
		//		else if (Pinkey.md == 3) {	// 下に進んでいたら
		//			Pinkey.ed = 0;				// 左に方向を変える
		//			Pinkey.WallHit = false;
		//		}
		//	}
		//}
		//// 上だけに壁があった場合
		//else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == true && Pinkey.bottom == false) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		if (Pinkey.md == 2) {	// 上に進んでいたら
		//			// パックマンの位置がピンキーより右なら
		//			if (dx2 > 0.0f) {
		//				Pinkey.ed = 1;				// 右に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//			else {
		//				Pinkey.ed = 0;				// 左に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//		}
		//	}
		//	else {	// 壁に当たっていなかったら
		//		if (Pinkey.md == 0) {	// 左に進んでいたら
		//			// パックマンの位置がピンキーより下なら
		//			if (dy2 > 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
		//					Pinkey.ed = 3;				// 下に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//		else if (Pinkey.md == 1) {	// 右に進んでいたら
		//			// パックマンの位置がピンキーより下なら
		//			if (dy2 > 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
		//					Pinkey.ed = 3;				// 下に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//	}
		//}
		//// 下だけに壁があった場合
		//else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == true) {
		//	// 壁に当たった場合
		//	if (Pinkey.WallHit == true) {
		//		// 上から入ってきたら
		//		if (Pinkey.md == 3) {	// 下に進んでいたら
		//			// パックマンの位置がピンキーより右なら
		//			if (dx2 > 0.0f) {
		//				Pinkey.ed = 1;				// 右に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//			else {
		//				Pinkey.ed = 0;				// 左に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//		}
		//	}
		//	else {	// 壁に当たっていなかったら
		//		if (Pinkey.md == 0) {	// 左に進んでいたら
		//			// パックマンの位置がピンキーより下なら
		//			if (dy2 < 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
		//					Pinkey.ed = 2;				// 上に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//		else if (Pinkey.md == 1) {	// 右に進んでいたら
		//			// パックマンの位置がピンキーより上なら
		//			if (dy2 < 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
		//					Pinkey.ed = 2;				// 上に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//	}
		//}
		////　右だけに壁があった場合
		//else if (Pinkey.left == false && Pinkey.right == true && Pinkey.up == false && Pinkey.bottom == false) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		if (Pinkey.md == 1) {	// 右に進んでいたら
		//			// パックマンの位置がピンキーより下なら
		//			if (dy2 > 0.0f) {
		//				Pinkey.ed = 3;				// 下に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//			else {
		//				Pinkey.ed = 2;				// 上に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//		}
		//	}
		//	else {	// 壁に当たっていなかったら
		//		if (Pinkey.md == 2) {	// 上に進んでいたら
		//			// パックマンの位置がピンキーより左なら
		//			if (dx2 < 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
		//					Pinkey.ed = 0;				// 左に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//		else if (Pinkey.md == 3) {	// 下に進んでいたら
		//			// パックマンの位置がピンキーより左なら
		//			if (dx2 < 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
		//					Pinkey.ed = 0;				// 左に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//	}
		//}
		//// 左だけに壁があった場合
		//else if (Pinkey.left == true && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		//	// 壁に当たったら
		//	if (Pinkey.WallHit == true) {
		//		if (Pinkey.md == 0) {	// 左に進んでいたら
		//			// パックマンの位置がピンキーより下なら
		//			if (dy2 > 0.0f) {
		//				Pinkey.ed = 3;				// 下に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//			else {
		//				Pinkey.ed = 2;				// 上に方向を変える
		//				Pinkey.WallHit = false;
		//			}
		//		}
		//	}
		//	else {	// 壁に当たっていなかったら
		//		if (Pinkey.md == 2) {	// 上に移動していたら
		//			// パックマンの位置がピンキーより右なら
		//			if (dx2 > 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
		//					Pinkey.ed = 1;				// 右に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//		else if (Pinkey.md == 3) {	// 下に移動していたら
		//			// パックマンの位置がピンキーより右なら
		//			if (dx2 > 0.0f) {
		//				// 壁に引っかからずに曲がれるように
		//				if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
		//					Pinkey.ed = 1;				// 右に方向を変える
		//					Pinkey.WallHit = false;
		//				}
		//			}
		//		}
		//	}
		//}
		//// 十字路の場合
		//else if (Pinkey.left == false && Pinkey.right == false && Pinkey.up == false && Pinkey.bottom == false) {
		//	// 壁に当たっていなかったら
		//	if (Pinkey.WallHit == false)
		//	{		// ピンキーが進んでいる方向によって処理を決める
		//		switch (Pinkey.md) {
		//		case 0:	// 左に進んでいたら
		//			// 壁に引っかからずに曲がれるように
		//			if (Pinkey.x / 16.0f == (float)Pinkey.mapX - 0.5f) {
		//				// パックマンの位置がピンキーより上なら
		//				if (dy2 < 0.0f) {
		//					Pinkey.ed = 2;				// 上に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//				else {
		//					Pinkey.ed = 3;				// 下に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//			}
		//		case 1:	// 右に進んでいたら
		//			// 壁に引っかからずに曲がれるように
		//			if (Pinkey.x / 16.0f == (float)Pinkey.mapX + 0.5f) {
		//				// パックマンの位置がピンキーより上なら
		//				if (dy2 < 0.0f) {
		//					Pinkey.ed = 2;				// 上に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//				else {
		//					Pinkey.ed = 3;				// 下に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//			}
		//		case 2:	// 上に進んでいたら
		//			// 壁に引っかからずに曲がれるように
		//			if (Pinkey.y / 16.0f == (float)Pinkey.mapY - 0.5f) {
		//				// パックマンの位置がピンキーより左なら
		//				if (dx2 < 0.0f) {
		//					Pinkey.ed = 0;				// 左に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//				else {
		//					Pinkey.ed = 1;				// 右に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//			}
		//		case 3:	// 下に進んでいたら
		//			// 壁に引っかからずに曲がれるように
		//			if (Pinkey.y / 16.0f == (float)Pinkey.mapY + 0.5f) {
		//				// パックマンの位置がピンキーより左なら
		//				if (dx2 < 0.0f) {
		//					Pinkey.ed = 0;				// 左に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//				else {
		//					Pinkey.ed = 1;				// 右に方向を変える
		//					Pinkey.WallHit = false;
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
	}

	if (Pinkey.x == 240) {
		Run = false;
	}
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
			if (Aosuke.y / 16.0f != 12.5f) {
				Aosuke.ed = 2;
			}
			else {
				Aosuke.movefinish = true;
				if (mPac.x > Aosuke.x) {
					Aosuke.ed = 1;
				}
				else {
					Aosuke.ed = 0;
				}
			}
		}
	}
}

void AosukeChaseMode() {
	Aosuke.mx = Aosuke.x;		// アカベイのx座標を保存
	Aosuke.my = Aosuke.y;		// アカベイのy座標を保存
	Aosuke.md = Aosuke.ed;		// 敵の動く方向を保存

	Aosuke.mapX = (int)Aosuke.x / 16;
	Aosuke.mapY = (int)Aosuke.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Aosuke.x;

	B = mPac.y - Aosuke.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	DrawLine(mPac.x, mPac.y, Aosuke.x, Aosuke.y, GetColor(24, 235, 249));

	// アカベイが壁を避けながら移動する処理
	switch (Aosuke.ed) {
	case 0:	// 左へ移動
		Aosuke.x -= Aosuke.speed;
		Aosuke.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Aosuke.x += Aosuke.speed;
		Aosuke.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Aosuke.y -= Aosuke.speed;
		Aosuke.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Aosuke.y += Aosuke.speed;
		Aosuke.eyeImageCount = 2;
		break;
	}

	// 左と上に壁があった場合
	if (Aosuke.left == true && Aosuke.right == false && Aosuke.up == true && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Aosuke.md == 0) {
				Aosuke.ed = 3;				// 下に方向を変える
				Aosuke.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Aosuke.md == 2) {
				Aosuke.ed = 1;				// 右に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Aosuke.left == true && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == true) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Aosuke.md == 0) {
				Aosuke.ed = 2;				// 上に方向を変える
				Aosuke.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Aosuke.md == 3) {
				Aosuke.ed = 1;				// 右に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Aosuke.left == false && Aosuke.right == true && Aosuke.up == true && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Aosuke.md == 1) {
				Aosuke.ed = 3;				// 下に方向を変える
				Aosuke.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Aosuke.md == 2) {	// 上に進んでいたら
				Aosuke.ed = 0;				// 左に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Aosuke.left == false && Aosuke.right == true && Aosuke.up == false && Aosuke.bottom == true) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Aosuke.md == 1) {	// 右に進んでいたら
				Aosuke.ed = 2;				// 上に方向を変える
				Aosuke.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Aosuke.md == 3) {	// 下に進んでいたら
				Aosuke.ed = 0;				// 左に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Aosuke.left == false && Aosuke.right == false && Aosuke.up == true && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			if (Aosuke.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					Aosuke.ed = 1;				// 右に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 0;				// 左に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX - 0.5f) {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX + 0.5f) {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Aosuke.left == false && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == true) {
		// 壁に当たった場合
		if (Aosuke.WallHit == true) {
			// 上から入ってきたら
			if (Aosuke.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					Aosuke.ed = 1;				// 右に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 0;				// 左に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX - 0.5f) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより上なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX + 0.5f) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Aosuke.left == false && Aosuke.right == true && Aosuke.up == false && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			if (Aosuke.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					Aosuke.ed = 3;				// 下に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 2;				// 上に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY - 0.5f) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY + 0.5f) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Aosuke.left == true && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			if (Aosuke.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					Aosuke.ed = 3;				// 下に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 2;				// 上に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 2) {	// 上に移動していたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY - 0.5f) {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 3) {	// 下に移動していたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY + 0.5f) {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Aosuke.left == false && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == false) {
		// 壁に当たっていなかったら
		if (Aosuke.WallHit == false)
		{		// アカベイが進んでいる方向によって処理を決める
			switch (Aosuke.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.x / 16.0f == (float)Aosuke.mapX - 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (dy < 0.0f) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.x / 16.0f == (float)Aosuke.mapX + 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (dy < 0.0f) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.y / 16.0f == (float)Aosuke.mapY - 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (dx < 0.0f) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.y / 16.0f == (float)Aosuke.mapY + 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (dx < 0.0f) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void AosukeScatterMode() {

	int NawabariX, NawabariY;

	NawabariX = MAP_SIZE * 28;
	NawabariY = MAP_SIZE * 31;

	Aosuke.mx = Aosuke.x;		// アカベイのx座標を保存
	Aosuke.my = Aosuke.y;		// アカベイのy座標を保存
	Aosuke.md = Aosuke.ed;		// 敵の動く方向を保存

	Aosuke.mapX = (int)Aosuke.x / 16;
	Aosuke.mapY = (int)Aosuke.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Aosuke.x;

	B = mPac.y - Aosuke.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	DrawLine(NawabariX, NawabariY, Aosuke.x, Aosuke.y, GetColor(24, 235, 249));


	// アカベイが壁を避けながら移動する処理
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

	// 左と上に壁があった場合
	if (Aosuke.left == true && Aosuke.right == false && Aosuke.up == true && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Aosuke.md == 0) {
				Aosuke.ed = 3;				// 下に方向を変える
				Aosuke.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Aosuke.md == 2) {
				Aosuke.ed = 1;				// 右に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Aosuke.left == true && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == true) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Aosuke.md == 0) {
				Aosuke.ed = 2;				// 上に方向を変える
				Aosuke.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Aosuke.md == 3) {
				Aosuke.ed = 1;				// 右に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Aosuke.left == false && Aosuke.right == true && Aosuke.up == true && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Aosuke.md == 1) {
				Aosuke.ed = 3;				// 下に方向を変える
				Aosuke.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Aosuke.md == 2) {	// 上に進んでいたら
				Aosuke.ed = 0;				// 左に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Aosuke.left == false && Aosuke.right == true && Aosuke.up == false && Aosuke.bottom == true) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Aosuke.md == 1) {	// 右に進んでいたら
				Aosuke.ed = 2;				// 上に方向を変える
				Aosuke.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Aosuke.md == 3) {	// 下に進んでいたら
				Aosuke.ed = 0;				// 左に方向を変える
				Aosuke.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Aosuke.left == false && Aosuke.right == false && Aosuke.up == true && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			if (Aosuke.md == 2) {	// 上に進んでいたら
				// 
				if (NawabariX > Aosuke.x) {
					Aosuke.ed = 1;				// 右に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 0;				// 左に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Aosuke.y) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX - 0.5f) {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Aosuke.y) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX + 0.5f) {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Aosuke.left == false && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == true) {
		// 壁に当たった場合
		if (Aosuke.WallHit == true) {
			// 上から入ってきたら
			if (Aosuke.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Aosuke.x) {
					Aosuke.ed = 1;				// 右に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 0;				// 左に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY < Aosuke.y) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX - 0.5f) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより上なら
				if (NawabariY < Aosuke.y) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.x / 16.0f == (float)Aosuke.mapX + 0.5f) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Aosuke.left == false && Aosuke.right == true && Aosuke.up == false && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			if (Aosuke.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Aosuke.y) {
					Aosuke.ed = 3;				// 下に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 2;				// 上に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (NawabariX < Aosuke.x) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY - 0.5f) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (NawabariX < Aosuke.x) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY + 0.5f) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Aosuke.left == true && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == false) {
		// 壁に当たったら
		if (Aosuke.WallHit == true) {
			if (Aosuke.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Aosuke.y) {
					Aosuke.ed = 3;				// 下に方向を変える
					Aosuke.WallHit = false;
				}
				else {
					Aosuke.ed = 2;				// 上に方向を変える
					Aosuke.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Aosuke.md == 2) {	// 上に移動していたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Aosuke.x) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY - 0.5f) {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
			else if (Aosuke.md == 3) {	// 下に移動していたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Aosuke.x) {
					// 壁に引っかからずに曲がれるように
					if (Aosuke.y / 16.0f == (float)Aosuke.mapY + 0.5f) {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Aosuke.left == false && Aosuke.right == false && Aosuke.up == false && Aosuke.bottom == false) {
		// 壁に当たっていなかったら
		if (Aosuke.WallHit == false)
		{		// アカベイが進んでいる方向によって処理を決める
			switch (Aosuke.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.x / 16.0f == (float)Aosuke.mapX - 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (NawabariY < Aosuke.y) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.x / 16.0f == (float)Aosuke.mapX + 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (NawabariY < Aosuke.y) {
						Aosuke.ed = 2;				// 上に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 3;				// 下に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.y / 16.0f == (float)Aosuke.mapY - 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (NawabariX < Aosuke.x) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Aosuke.y / 16.0f == (float)Aosuke.mapY + 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (NawabariX < Aosuke.x) {
						Aosuke.ed = 0;				// 左に方向を変える
						Aosuke.WallHit = false;
						break;
					}
					else {
						Aosuke.ed = 1;				// 右に方向を変える
						Aosuke.WallHit = false;
						break;
					}
				}
			}
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
			if (Guzuta.y / 16.0f != 12.5f) {
				Guzuta.ed = 2;
			}
			else {
				Guzuta.movefinish = true;
				if (mPac.x > Guzuta.x) {
					Guzuta.ed = 1;
				}
				else {
					Guzuta.ed = 0;
				}
			}
		}
	}
}

void GuzutaChaseMode() {
	Guzuta.mx = Guzuta.x;		// アカベイのx座標を保存
	Guzuta.my = Guzuta.y;		// アカベイのy座標を保存
	Guzuta.md = Guzuta.ed;		// 敵の動く方向を保存

	Guzuta.mapX = (int)Guzuta.x / 16;
	Guzuta.mapY = (int)Guzuta.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Guzuta.x;

	B = mPac.y - Guzuta.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	DrawLine(mPac.x, mPac.y, Guzuta.x, Guzuta.y, GetColor(233, 150, 122));

	// アカベイが壁を避けながら移動する処理
	switch (Guzuta.ed) {
	case 0:	// 左へ移動
		Guzuta.x -= Guzuta.speed;
		Guzuta.eyeImageCount = 3;
		break;
	case 1:	// 右へ移動
		Guzuta.x += Guzuta.speed;
		Guzuta.eyeImageCount = 1;
		break;
	case 2:	// 上へ移動
		Guzuta.y -= Guzuta.speed;
		Guzuta.eyeImageCount = 0;
		break;
	case 3:	// 下へ移動
		Guzuta.y += Guzuta.speed;
		Guzuta.eyeImageCount = 2;
		break;
	}

	// 左と上に壁があった場合
	if (Guzuta.left == true && Guzuta.right == false && Guzuta.up == true && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Guzuta.md == 0) {
				Guzuta.ed = 3;				// 下に方向を変える
				Guzuta.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Guzuta.md == 2) {
				Guzuta.ed = 1;				// 右に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Guzuta.left == true && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == true) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Guzuta.md == 0) {
				Guzuta.ed = 2;				// 上に方向を変える
				Guzuta.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Guzuta.md == 3) {
				Guzuta.ed = 1;				// 右に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Guzuta.left == false && Guzuta.right == true && Guzuta.up == true && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Guzuta.md == 1) {
				Guzuta.ed = 3;				// 下に方向を変える
				Guzuta.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Guzuta.md == 2) {	// 上に進んでいたら
				Guzuta.ed = 0;				// 左に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Guzuta.left == false && Guzuta.right == true && Guzuta.up == false && Guzuta.bottom == true) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Guzuta.md == 1) {	// 右に進んでいたら
				Guzuta.ed = 2;				// 上に方向を変える
				Guzuta.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Guzuta.md == 3) {	// 下に進んでいたら
				Guzuta.ed = 0;				// 左に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Guzuta.left == false && Guzuta.right == false && Guzuta.up == true && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			if (Guzuta.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					Guzuta.ed = 1;				// 右に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 0;				// 左に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX - 0.5f) {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX + 0.5f) {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Guzuta.left == false && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == true) {
		// 壁に当たった場合
		if (Guzuta.WallHit == true) {
			// 上から入ってきたら
			if (Guzuta.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					Guzuta.ed = 1;				// 右に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 0;				// 左に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX - 0.5f) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより上なら
				if (dy < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX + 0.5f) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Guzuta.left == false && Guzuta.right == true && Guzuta.up == false && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			if (Guzuta.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					Guzuta.ed = 3;				// 下に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 2;				// 上に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY - 0.5f) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (dx < 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY + 0.5f) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Guzuta.left == true && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			if (Guzuta.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (dy > 0.0f) {
					Guzuta.ed = 3;				// 下に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 2;				// 上に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 2) {	// 上に移動していたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY - 0.5f) {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 3) {	// 下に移動していたら
				// パックマンの位置がアカベイより右なら
				if (dx > 0.0f) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY + 0.5f) {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Guzuta.left == false && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == false) {
		// 壁に当たっていなかったら
		if (Guzuta.WallHit == false)
		{		// アカベイが進んでいる方向によって処理を決める
			switch (Guzuta.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.x / 16.0f == (float)Guzuta.mapX - 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (dy < 0.0f) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.x / 16.0f == (float)Guzuta.mapX + 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (dy < 0.0f) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.y / 16.0f == (float)Guzuta.mapY - 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (dx < 0.0f) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.y / 16.0f == (float)Guzuta.mapY + 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (dx < 0.0f) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void GuzutaScatterMode() {

	int NawabariX, NawabariY;

	NawabariX = MAP_SIZE * 2;
	NawabariY = MAP_SIZE * 31;

	Guzuta.mx = Guzuta.x;		// アカベイのx座標を保存
	Guzuta.my = Guzuta.y;		// アカベイのy座標を保存
	Guzuta.md = Guzuta.ed;		// 敵の動く方向を保存

	Guzuta.mapX = (int)Guzuta.x / 16;
	Guzuta.mapY = (int)Guzuta.y / 16;
	px = (int)mPac.x / 16;
	py = (int)mPac.y / 16;

	// 三平方の定理を使う
	A = mPac.x - Guzuta.x;

	B = mPac.y - Guzuta.y;

	C = sqrtf(A * A + B * B);	// A と B を２乗して足した値の平方根を求める

	dx = A / C;		// C を1（正規化）とするには、A を C で割る
	dy = B / C;		// C を1（正規化）とするには、B を C で割る

	DrawLine(NawabariX, NawabariY, Guzuta.x, Guzuta.y, GetColor(233, 150, 122));


	// アカベイが壁を避けながら移動する処理
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

	// 左と上に壁があった場合
	if (Guzuta.left == true && Guzuta.right == false && Guzuta.up == true && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 右から入ってきたら、下に方向を変える
			if (Guzuta.md == 0) {
				Guzuta.ed = 3;				// 下に方向を変える
				Guzuta.WallHit = false;
			}
			// 下から入ってきたら、右に方向を変える
			else if (Guzuta.md == 2) {
				Guzuta.ed = 1;				// 右に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 左と下に壁があったら
	else if (Guzuta.left == true && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == true) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 右から入ってきたら、上に方向を変える
			if (Guzuta.md == 0) {
				Guzuta.ed = 2;				// 上に方向を変える
				Guzuta.WallHit = false;
			}
			// 上から入ってきたら、右に方向を変える
			else if (Guzuta.md == 3) {
				Guzuta.ed = 1;				// 右に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 右と上に壁がある場合
	else if (Guzuta.left == false && Guzuta.right == true && Guzuta.up == true && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 左から入ってきたら、下に方向を変える
			if (Guzuta.md == 1) {
				Guzuta.ed = 3;				// 下に方向を変える
				Guzuta.WallHit = false;
			}
			// 下から入ってきたら、左に方向を変える
			else if (Guzuta.md == 2) {	// 上に進んでいたら
				Guzuta.ed = 0;				// 左に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 右と下に壁がある場合
	else if (Guzuta.left == false && Guzuta.right == true && Guzuta.up == false && Guzuta.bottom == true) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			// 左から入ってきたら、上に方向を変える
			if (Guzuta.md == 1) {	// 右に進んでいたら
				Guzuta.ed = 2;				// 上に方向を変える
				Guzuta.WallHit = false;
			}
			// 上から入ってきたら、左に方向を変える
			else if (Guzuta.md == 3) {	// 下に進んでいたら
				Guzuta.ed = 0;				// 左に方向を変える
				Guzuta.WallHit = false;
			}
		}
	}
	// 上だけに壁があった場合
	else if (Guzuta.left == false && Guzuta.right == false && Guzuta.up == true && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			if (Guzuta.md == 2) {	// 上に進んでいたら
				// 
				if (NawabariX > Guzuta.x) {
					Guzuta.ed = 1;				// 右に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 0;				// 左に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Guzuta.y) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX - 0.5f) {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Guzuta.y) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX + 0.5f) {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	// 下だけに壁があった場合
	else if (Guzuta.left == false && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == true) {
		// 壁に当たった場合
		if (Guzuta.WallHit == true) {
			// 上から入ってきたら
			if (Guzuta.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Guzuta.x) {
					Guzuta.ed = 1;				// 右に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 0;				// 左に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY < Guzuta.y) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX - 0.5f) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより上なら
				if (NawabariY < Guzuta.y) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.x / 16.0f == (float)Guzuta.mapX + 0.5f) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	//　右だけに壁があった場合
	else if (Guzuta.left == false && Guzuta.right == true && Guzuta.up == false && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			if (Guzuta.md == 1) {	// 右に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Guzuta.y) {
					Guzuta.ed = 3;				// 下に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 2;				// 上に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 2) {	// 上に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (NawabariX < Guzuta.x) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY - 0.5f) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 3) {	// 下に進んでいたら
				// パックマンの位置がアカベイより左なら
				if (NawabariX < Guzuta.x) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY + 0.5f) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	// 左だけに壁があった場合
	else if (Guzuta.left == true && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == false) {
		// 壁に当たったら
		if (Guzuta.WallHit == true) {
			if (Guzuta.md == 0) {	// 左に進んでいたら
				// パックマンの位置がアカベイより下なら
				if (NawabariY > Guzuta.y) {
					Guzuta.ed = 3;				// 下に方向を変える
					Guzuta.WallHit = false;
				}
				else {
					Guzuta.ed = 2;				// 上に方向を変える
					Guzuta.WallHit = false;
				}
			}
		}
		else {	// 壁に当たっていなかったら
			if (Guzuta.md == 2) {	// 上に移動していたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Guzuta.x) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY - 0.5f) {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
			else if (Guzuta.md == 3) {	// 下に移動していたら
				// パックマンの位置がアカベイより右なら
				if (NawabariX > Guzuta.x) {
					// 壁に引っかからずに曲がれるように
					if (Guzuta.y / 16.0f == (float)Guzuta.mapY + 0.5f) {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
					}
				}
			}
		}
	}
	// 十字路の場合
	else if (Guzuta.left == false && Guzuta.right == false && Guzuta.up == false && Guzuta.bottom == false) {
		// 壁に当たっていなかったら
		if (Guzuta.WallHit == false)
		{		// アカベイが進んでいる方向によって処理を決める
			switch (Guzuta.md) {
			case 0:	// 左に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.x / 16.0f == (float)Guzuta.mapX - 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (NawabariY < Guzuta.y) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			case 1:	// 右に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.x / 16.0f == (float)Guzuta.mapX + 0.5f) {
					// パックマンの位置がアカベイより上なら
					if (NawabariY < Guzuta.y) {
						Guzuta.ed = 2;				// 上に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 3;				// 下に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			case 2:	// 上に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.y / 16.0f == (float)Guzuta.mapY - 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (NawabariX < Guzuta.x) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			case 3:	// 下に進んでいたら
				// 壁に引っかからずに曲がれるように
				if (Guzuta.y / 16.0f == (float)Guzuta.mapY + 0.5f) {
					// パックマンの位置がアカベイより左なら
					if (NawabariX < Guzuta.x) {
						Guzuta.ed = 0;				// 左に方向を変える
						Guzuta.WallHit = false;
						break;
					}
					else {
						Guzuta.ed = 1;				// 右に方向を変える
						Guzuta.WallHit = false;
						break;
					}
				}
			}
		}
	}
}

void PinkeyWarp() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(Pinkey.x + 8, Pinkey.y, Pinkey.w, Pinkey.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 2) {//左のトンネルに入ったら
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 3 && Pinkey.md == 0) {//右のトンネルに
								//Pinkey.x = j * MAP_SIZE - 8;//ワープ
								Pinkey.x = j * MAP_SIZE + 4;//ワープ
								Pinkey.y = i * MAP_SIZE + 8;//ワープ
							}
						}
					}

				}

			}
			if (HitCheck(Pinkey.x - 8, Pinkey.y, Pinkey.w, Pinkey.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 3) {//右のトンネルに入ったら
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 2 && Pinkey.md == 1) {//左のトンネルに
								//Pinkey.x = j * MAP_SIZE + 16;//ワープ
								Pinkey.x = j * MAP_SIZE + 12;//ワープ
								Pinkey.y = i * MAP_SIZE + 8;//ワープ
							}
						}
					}
				}
			}
		}
	}
}