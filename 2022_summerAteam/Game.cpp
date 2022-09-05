#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"

static int mImageHandle; //画像ハンドル格納用変数
float mx = 0, my = 0;

/**********仮のマップチップ（消してもいい）**********/
int MapData[MAP_HEIGHT][MAP_WIDTH] = //マップデータ 1は壁がある 0は壁がない　
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },

 { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },

 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0,   0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0 },
 { 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0 },
 { 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/**********仮のマップチップ（消してもいい）**********/


void DrawMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {
				DrawBox(j * MAP_SIZE, i * MAP_SIZE, j *
					MAP_SIZE + MAP_SIZE, i * MAP_SIZE + MAP_SIZE, GetColor(0, 0, 255), TRUE);

				if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					mPac.x = mx;
					mPac.y = my;

					WarpTunnel();
				}

				//if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				//	Akabei.WallHit = true;
				//	// 元の場所に戻す
				//	Akabei.x = Akabei.mx;
				//	Akabei.y = Akabei.my;

				//}
			}
		}
	}
}


//初期化
void Game_Initialize() {
	Player_Initialize();
	Enemy_Initialize();
	Item_Initialize();//アイテム用
	//mImageHandle = LoadGraph("images/Scene_GameMain.png"); //画像のロード
}

//終了処理
void Game_Finalize() {
	Player_Finalize();
	Enemy_Finalize();
	Item_Finalize();//アイテム用
	//DeleteGraph(mImageHandle); //画像の解放
}

//更新
void Game_Update() {
	mx = mPac.x;
	my = mPac.y;

	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Escキーが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
	}
	Player_Update();
	Enemy_Update();
	DrawMap();
	Item_Update();//アイテム用

	if (MapData[(int)Akabei.y / 16][(int)Akabei.x / 16] == 1) {
		Akabei.x = Akabei.mx;
		Akabei.y = Akabei.my;
	}
}

//描画
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESCキーを押してメニュー画面へ");
	Player_Draw();
	Enemy_Draw();
	Item_Draw();
}

void WarpTunnel() {

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(mPac.x + 16, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 2) {//
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 3 && mPac.type == 3) {
								mPac.x = j * MAP_SIZE + 8;
								mPac.y = i * MAP_SIZE + 8;
							}
						}
					}

				}

			}
			if (HitCheck(mPac.x - 16, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {

				if (MapData[i][j] == 3) {
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 2 && mPac.type == 1) {
								mPac.x = j * MAP_SIZE + 8;
								mPac.y = i * MAP_SIZE + 8;
							}
						}
					}
				}
			}
		}
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
		if (MapData[(int)Akabei.y / 16][(int)Akabei.x / 16] == 1) {
			// 元の場所に戻す
			Akabei.x = Akabei.mx;
			Akabei.y = Akabei.my;

			// 進む方向を決める
			switch (Akabei.md) {
			case 0:
				//if (Akabei.ed == 0 && Akabei.left == true && Akabei.right == false && Akabei.up == false && Akabei.bottom == false) {
				//	Akabei.left = false;
				//	Akabei.ed = 3;
				//	Akabei.WallHit = false;
				//}

				// 左に進んでいる時に壁に当たった場合、進める方向は上か下になる
				if (Akabei.ed == 0) {
					if (Akabei.y > mPac.y) {	// アカベイの位置が仮プレイヤーより下なら
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
				//if (Akabei.ed == 1 && Akabei.right == true && Akabei.left == false && Akabei.up == false && Akabei.bottom == false) {
				//	Akabei.right = false;
				//	Akabei.ed = 2;
				//	Akabei.WallHit = false;
				//}

				// 右に進んでいる時に壁に当たった場合、進める方向は上か下になる
				if (Akabei.ed == 1) {
					if (Akabei.y > mPac.y) {	// アカベイの位置が仮プレイヤーより下なら
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
				//if (Akabei.ed == 2 && Akabei.up == true && Akabei.left == false && Akabei.right == false && Akabei.bottom == false) {
				//	Akabei.up = false;
				//	Akabei.ed = 0;
				//	Akabei.WallHit = false;
				//}

				// 上に進んでいる時に壁に当たった場合、進める方向は右か左になる
				if (Akabei.ed == 2) {
					if (Akabei.x > mPac.x) {	// アカベイの位置が仮プレイヤーより右なら
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
				//if (Akabei.ed == 3 && Akabei.bottom == true && Akabei.left == false && Akabei.right == false && Akabei.up == false) {
				//	Akabei.bottom = false;
				//	Akabei.ed = 1;
				//	Akabei.WallHit = false;
				//}

				// 下に進んでいる時に壁に当たった場合、進める方向は右か左になる
				if (Akabei.ed == 3) {
					if (Akabei.x > mPac.x) {	// アカベイの位置が仮プレイヤーより右なら
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
			Akabei.WallHit = false;
		}
		else {
			break;		// 移動先が壁じゃない場合は方向を変えるループから抜ける
		}
	}
}