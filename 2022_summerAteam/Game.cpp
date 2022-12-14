#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "UI.h"

static int mImageHandle; //画像ハンドル格納用変数
float mx = 0, my = 0;
int mimg = 0, mtype = 0;	//画像保存用

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
 { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 4,   4, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
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

				if (HitCheck(Akabei.x, Akabei.y, ENEMY_SIZE, ENEMY_SIZE, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Akabei.x = Akabei.mx;
					Akabei.y = Akabei.my;
					if (Akabei.ed == Akabei.md) {
						Akabei.WallHit = true;
					}
				}

				if (HitCheck(Pinkey.x, Pinkey.y, ENEMY_SIZE, ENEMY_SIZE, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Pinkey.x = Pinkey.mx;
					Pinkey.y = Pinkey.my;
					if (Pinkey.ed == Pinkey.md) {
						Pinkey.WallHit = true;
					}
				}
				if (HitCheck(Aosuke.x, Aosuke.y, ENEMY_SIZE, ENEMY_SIZE, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Aosuke.x = Aosuke.mx;
					Aosuke.y = Aosuke.my;
					if (Aosuke.ed == Aosuke.md) {
						Aosuke.WallHit = true;
					}
				}
				if (HitCheck(Guzuta.x, Guzuta.y, ENEMY_SIZE, ENEMY_SIZE, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Guzuta.x = Guzuta.mx;
					Guzuta.y = Guzuta.my;
					if (Guzuta.ed == Guzuta.md) {
						Guzuta.WallHit = true;
					}
				}
			}

			switch (mPac.var)
			{
			case 0:
				if (HitCheck(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.w * 1.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						//DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.h * 1.5f), mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.x == (j * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 0;
						}
					}
					else {
						//DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.h * 1.5f), mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			case 1:
				if (HitCheck(mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						//DrawBox(mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.x + (mPac.w * 1.5f), mPac.y + (mPac.h * 0.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.y == (i * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 1;
						}
					}
					else {
						//DrawBox(mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.x + (mPac.w * 1.5f), mPac.y + (mPac.h * 0.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			case 2:
				if (HitCheck(mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						//DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), mPac.x + (mPac.w * 0.5f), mPac.y + (mPac.h * 1.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.x == (j * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 2;
						}
					}
					else {
						//DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), mPac.x + (mPac.w * 0.5f), mPac.y + (mPac.h * 1.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			case 3:
				if (HitCheck(mPac.x - (mPac.w * 1.5f), mPac.y - (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						//DrawBox(mPac.x - (mPac.w * 1.5f), mPac.y - (mPac.h * 0.5f), mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.y == (i * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 3;
						}
					}
					else {
						//DrawBox(mPac.x - (mPac.w * 1.5f), mPac.y - (mPac.h * 0.5f), mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			default:
				break;
			}
			if (HitCheck(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 1) {
					mPac.x = mx;
					mPac.y = my;
					mPac.img = mimg;
					mPac.type = mPac.var;
				}
				WarpTunnel();
			}

		}
	}
}


//初期化
void Game_Initialize() {
	Player_Initialize();
	Enemy_Initialize();
	Item_Initialize();//アイテム用
	UI_Initialize();
	//mImageHandle = LoadGraph("images/Scene_GameMain.png"); //画像のロード
}

//終了処理
void Game_Finalize() {
	Player_Finalize();
	Enemy_Finalize();
	Item_Finalize();//アイテム用
	UI_Finalize();
	//DeleteGraph(mImageHandle); //画像の解放
}

//更新
void Game_Update() {
	mx = mPac.x;
	my = mPac.y;
	mimg = mPac.img;
	mtype = mPac.type;

	AkabeiMapHitCheck();	// アカベイの上下左右に壁があるかどうか調べる処理
	PinkeyMapHitCheck();	// ピンキーの上下左右に壁があるかどうか調べる処理
	AosukeMapHitCheck();	// アオスケの上下左右に壁があるかどうか調べる処理
	GuzutaMapHitCheck();	// グズタの上下左右に壁があるかどうか調べる処理

	


	if (CheckHitKey(KEY_INPUT_B) != 0) {//Escキーが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0 || (g_NowKey & PAD_INPUT_7) != 0) {// ESCAPEキーが押されているかを調べる
		DxLib_End();
	}
	Player_Update();
	DrawMap();
	UI_Update();
	Enemy_Update();
	Item_Update();//アイテム用
	UI_Update();


}

//描画
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	//DrawFormatString(10, 700, 0xffffff, "Bキーを押してタイトル画面へ");
	Player_Draw();
	Item_Draw();
	UI_Draw();
	Enemy_Draw();
}

void WarpTunnel() {

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (HitCheck(mPac.x + 8, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 2) {//左のトンネルに入ったら
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 3 && mPac.type == 3) {//右のトンネルに
								//mPac.x = j * MAP_SIZE - 8;//ワープ
								mPac.x = j * MAP_SIZE + 4;//ワープ
								mPac.y = i * MAP_SIZE + 8;//ワープ
							}
						}
					}

				}

			}
			if (HitCheck(mPac.x - 8, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 3) {//右のトンネルに入ったら
					for (int i = 0; i < MAP_HEIGHT; i++) {
						for (int j = 0; j < MAP_WIDTH; j++) {
							if (MapData[i][j] == 2 && mPac.type == 1) {//左のトンネルに
								//mPac.x = j * MAP_SIZE + 16;//ワープ
								mPac.x = j * MAP_SIZE + 12;//ワープ
								mPac.y = i * MAP_SIZE + 8;//ワープ
							}
						}
					}
				}
			}
		}
	}
}

// アカベイの上下左右に壁があるかどうか調べる処理
void AkabeiMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) - 1] == 1 || MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) - 1] == 4) {
		Akabei.left = true;
	}
	else {
		Akabei.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) + 1] == 1 || MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) + 1] == 4) {
		Akabei.right = true;
	}
	else {
		Akabei.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Akabei.y / 16) - 1][(int)Akabei.x / 16] == 1 || MapData[((int)Akabei.y / 16) - 1][(int)Akabei.x / 16] == 4) {
		Akabei.up = true;
	}
	else {
		Akabei.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Akabei.y / 16) + 1][(int)Akabei.x / 16] == 1 || MapData[((int)Akabei.y / 16) + 1][(int)Akabei.x / 16] == 4) {
		Akabei.bottom = true;
	}
	else {
		Akabei.bottom = false;
	}
}

void PinkeyMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Pinkey.y / 16][((int)Pinkey.x / 16) - 1] == 1) {
		Pinkey.left = true;
	}
	else {
		Pinkey.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Pinkey.y / 16][((int)Pinkey.x / 16) + 1] == 1) {
		//DrawBox(Pinkey.x / 16, Pinkey.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Pinkey.right = true;
	}
	else {
		Pinkey.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Pinkey.y / 16) - 1][(int)Pinkey.x / 16] == 1) {
		Pinkey.up = true;
	}
	else {
		Pinkey.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Pinkey.y / 16) + 1][(int)Pinkey.x / 16] == 1) {
		Pinkey.bottom = true;
	}
	else {
		Pinkey.bottom = false;
	}
}

// アオスケの上下左右に壁があるかどうか調べる処理
void AosukeMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Aosuke.y / 16][((int)Aosuke.x / 16) - 1] == 1 || MapData[(int)Aosuke.y / 16][((int)Aosuke.x / 16) - 1] == 4) {
		Aosuke.left = true;
	}
	else {
		Aosuke.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Aosuke.y / 16][((int)Aosuke.x / 16) + 1] == 1 || MapData[(int)Aosuke.y / 16][((int)Aosuke.x / 16) + 1] == 4) {
		Aosuke.right = true;
	}
	else {
		Aosuke.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Aosuke.y / 16) - 1][(int)Aosuke.x / 16] == 1 || MapData[((int)Aosuke.y / 16) - 1][(int)Aosuke.x / 16] == 4) {
		Aosuke.up = true;
	}
	else {
		Aosuke.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Aosuke.y / 16) + 1][(int)Aosuke.x / 16] == 1 || MapData[((int)Aosuke.y / 16) + 1][(int)Aosuke.x / 16] == 4) {
		Aosuke.bottom = true;
	}
	else {
		Aosuke.bottom = false;
	}
}

// グズタの上下左右に壁があるかどうか調べる処理
void GuzutaMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Guzuta.y / 16][((int)Guzuta.x / 16) - 1] == 1 || MapData[(int)Guzuta.y / 16][((int)Guzuta.x / 16) - 1] == 4) {
		Guzuta.left = true;
	}
	else {
		Guzuta.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Guzuta.y / 16][((int)Guzuta.x / 16) + 1] == 1 || MapData[(int)Guzuta.y / 16][((int)Guzuta.x / 16) + 1] == 4) {
		Guzuta.right = true;
	}
	else {
		Guzuta.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Guzuta.y / 16) - 1][(int)Guzuta.x / 16] == 1 || MapData[((int)Guzuta.y / 16) - 1][(int)Guzuta.x / 16] == 4) {
		Guzuta.up = true;
	}
	else {
		Guzuta.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Guzuta.y / 16) + 1][(int)Guzuta.x / 16] == 1 || MapData[((int)Guzuta.y / 16) + 1][(int)Guzuta.x / 16] == 4) {
		Guzuta.bottom = true;
	}
	else {
		Guzuta.bottom = false;
	}
}