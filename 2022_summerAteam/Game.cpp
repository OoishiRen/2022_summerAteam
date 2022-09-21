#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "UI.h"

static int mImageHandle; //画像ハンドル格納用変数
static int outerwall_left;//1
static int door;//4
static int outerwall_right;//6
static int outerwall_top;//5
static int outerwall_bottom;//7
static int outercorner_top_left;//8
static int outercorner_top_right;//9
static int outercorner_bottom_left;//10
static int outercorner_bottom_right;//11
static int outercorner_narrow_top_left;//12
static int outercorner_narrow_top_right;//13
static int outercorner_narrow_bottom_left;//14
static int outercorner_narrow_bottom_right;//15
static int outernose_left_top;//16
static int outernose_left_bottom;//17
static int outernose_top_left;//18
static int outernose_top_right;//19
static int outernose_right_top;// 20
static int outernose_right_bottom;//21
static int wall_top;//22
static int wall_bottom;//23
static int wall_left;//24
static int wall_right;//25
static int corner_bottom_left;//26
static int corner_bottom_right;//27
static int corner_top_left;//28
static int corner_top_right;//29
static int gate_left;//30
static int gate_right;//31
float mx = 0, my = 0;
int mimg = 0, mtype = 0;	//画像保存用

/**********仮のマップチップ（消してもいい）**********/
int MapData[MAP_HEIGHT][MAP_WIDTH] = //マップデータ 1は壁がある 0は壁がない　
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,18,   19,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 9, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,24,   25,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 0, 1, 0, 12,7, 7,13, 0, 1, 1, 1, 1, 1, 0,24,   25,0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 6, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0,24,   25,0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 14,5, 5,15, 0, 1, 1, 1, 1, 1, 0,26,   27,0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 0, 1, 0, 12,7, 7, 13,0, 28,29, 0, 8, 5, 5,5,   5, 5, 5, 9, 0,28,29, 0, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 14,5, 5, 15,0, 24,25, 0, 10,7, 7,13,  12,7, 7, 11,0,24,25, 0, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 24,25, 0, 0, 0, 0,6,   1, 0, 0, 0, 0,24,25, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 0, 10,7, 7, 7, 7,13, 0, 24,26, 22,22,29,0,6,   1, 0,28,22,22,27,25, 0,12, 7, 7, 7, 7,11, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 24,28, 23,23,27,0,14,  15,0,26,23,23,29,25, 0, 6, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 24,25, 0, 0, 0, 0,0,   0,0, 0, 0, 0, 24,25, 0, 6, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 24,25, 0, 12,7,30,4,   4,31,7,13, 0, 24,25, 0, 6, 0, 0, 0, 0, 0, 0 },
 { 5, 5, 5, 5, 5, 5,15, 0, 26,27, 0, 6, 0, 0,0,   0,0, 0, 1, 0, 26,27, 0,14, 5, 5, 5, 5, 5, 5 },

 { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  6, 0, 0, 0,   0, 0, 0,1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },

 { 7, 7, 7, 7, 7, 7,13, 0, 28,29, 0, 6, 0, 0,0,   0, 0, 0, 1, 0,28,29, 0,12 ,7, 7, 7, 7, 7, 7 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 24,25, 0, 14,5, 5,5,   5, 5, 5, 15,0,24,25, 0, 6, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 24,25, 0, 0, 0, 0,0,   0, 0, 0, 0, 0,24,25, 0, 6, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 0, 24,25, 0, 1, 1, 1,1,   1, 1, 1, 1, 0,24,25, 0, 6, 0, 0, 0, 0, 0, 0 },
 { 0, 8 ,5, 5, 5, 5,15, 0, 26,27, 0, 1, 1, 1,1,   1, 1, 1, 1, 0,26,27, 0,14, 5, 5, 5, 5, 9, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 0, 1, 0, 28,22,22,29,0, 28,22,22,22,29, 0,1,   1, 0,28,22,22,22,29, 0,28,22,22,29, 0, 6, 0 },
 { 0, 1, 0, 26,23,29,25,0, 26,23,23,23,27, 0,1,   1, 0,26,23,23,23,27, 0,24,28,23,27, 0, 6, 0 },
 { 0, 1, 0, 0, 0, 24,25,0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,24,25, 0, 0, 0, 6, 0 },
 { 0, 16,22,29,0, 24,25,0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0,24,25, 0, 28,22,20,0 },
 { 0, 17,23,27,0, 26,27,0, 1, 1, 0, 1, 1, 1, 1,   1, 1, 1, 1, 0, 1, 1, 0,26,27, 0, 26,23,21,0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,   1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,   1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 6, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 0, 10,7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,   7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 11,0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/**********仮のマップチップ（消してもいい）**********/


void DrawMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {
				//DrawBox(j * MAP_SIZE, i * MAP_SIZE, j *MAP_SIZE + MAP_SIZE, i * MAP_SIZE + MAP_SIZE, GetColor(0, 0, 255), TRUE);
					DrawGraph(j * MAP_SIZE, i * MAP_SIZE, outerwall_left, TRUE);
				if (HitCheck(Akabei.x - 8, Akabei.y - 8, Akabei.w, Akabei.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
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
				if (HitCheck(Aosuke.x, Aosuke.y, Aosuke.w, Aosuke.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Aosuke.x = Aosuke.mx;
					Aosuke.y = Aosuke.my;
					if (Aosuke.ed == Aosuke.md) {
						Aosuke.WallHit = true;
					}
				}
				if (HitCheck(Guzuta.x, Guzuta.y, Guzuta.w, Guzuta.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
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
						DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.h * 1.5f), mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.x == (j * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 0;
						}
					}
					else {
						DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.h * 1.5f), mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			case 1:
				if (HitCheck(mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.w , mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						DrawBox(mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.x + (mPac.w * 1.5f), mPac.y + (mPac.h * 0.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.y == (i * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 1;
						}
					}
					else {
						DrawBox(mPac.x + (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.x + (mPac.w * 1.5f), mPac.y + (mPac.h * 0.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			case 2:
				if (HitCheck(mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), mPac.x + (mPac.w * 0.5f), mPac.y + (mPac.h * 1.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.x == (j * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 2;
						}
					}
					else {
						DrawBox(mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), mPac.x + (mPac.w * 0.5f), mPac.y + (mPac.h * 1.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			case 3:
				if (HitCheck(mPac.x - (mPac.w * 1.5f), mPac.y - (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
					if (MapData[i][j] == 0) {
						DrawBox(mPac.x - (mPac.w * 1.5f), mPac.y - (mPac.h * 0.5f), mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), GetColor(0, 255, 0), FALSE);//デバッグ用
						if (mPac.y == (i * MAP_SIZE) + (MAP_SIZE * 0.5f)) {
							mPac.type = 3;
						}
					}
					else {
						DrawBox(mPac.x - (mPac.w * 1.5f), mPac.y - (mPac.h * 0.5f), mPac.x - (mPac.w * 0.5f), mPac.y + (mPac.h * 0.5f), GetColor(255, 0, 0), FALSE);//デバッグ用
					}
				}
				break;
			default:
				break;
			}
			if (HitCheck(mPac.x - (mPac.w * 0.5f), mPac.y - (mPac.h * 0.5f), mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] != 0) {
					mPac.x = mx;
					mPac.y = my;
					mPac.img = mimg;
					mPac.type = mPac.var;
				}
				WarpTunnel();
			}
			
			if (MapData[i][j] == 4) {
				DrawGraph(j* MAP_SIZE, i* MAP_SIZE, door, TRUE);
				if (Akabei.md == 2) {
					if (Akabei.juuji == false) {
						if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
							Akabei.x = Akabei.mx;
							Akabei.y = Akabei.my;
							Akabei.juujiUp = true;
							Akabei.juuji = true;
						}
					}
				}
			}

			if (MapData[i][j] == 5) {
				DrawGraph(j* FRUIT_SIZE, i* FRUIT_SIZE, outerwall_top, TRUE);
				if (Akabei.md == 0) {
					if (Akabei.juuji == false) {
						if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
							Akabei.x = Akabei.mx;
							Akabei.y = Akabei.my;
							Akabei.juujiLeft = true;
							Akabei.juuji = true;
						}
					}
				}
			}

			if (MapData[i][j] == 6) {
				DrawGraph(j* MAP_SIZE, i* MAP_SIZE, outerwall_right, TRUE);
				if (Akabei.md == 3) {
					if (Akabei.juuji == false) {
						if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
							Akabei.x = Akabei.mx;
							Akabei.y = Akabei.my;
							Akabei.juujiDown = true;
							Akabei.juuji = true;
						}
					}
				}
			}

			if (MapData[i][j] == 7) {
				DrawGraph(j* FRUIT_SIZE, i* FRUIT_SIZE, outerwall_bottom, TRUE);
				if (Akabei.md == 1) {
					if (Akabei.juuji == false) {
						if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
							Akabei.x = Akabei.mx;
							Akabei.y = Akabei.my;
							Akabei.juujiRight = true;
							Akabei.juuji = true;
						}
					}
				}
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
	outerwall_left = LoadGraph("Map_images/outerwall_left.png");
	outerwall_right = LoadGraph("Map_images/outerwall_right.png");
	outerwall_top = LoadGraph("Map_images/outerwall_top.png");
	outerwall_bottom = LoadGraph("Map_images/outerwall_bottom.png");
	outercorner_top_left= LoadGraph("Map_images/outercorner_top_left.png");
	outercorner_top_right = LoadGraph("Map_images/outercorner_top_right.png");
	outercorner_bottom_left = LoadGraph("Map_images/outercorner_bottom_left.png");
	outercorner_bottom_right = LoadGraph("Map_images/outercorner_bottom_right.png");
	outercorner_narrow_top_left = LoadGraph("Map_images/outercorner_narrow_top_left.png");
	outercorner_narrow_top_right = LoadGraph("Map_images/outercorner_narrow_top_right.png");
	outercorner_narrow_bottom_left = LoadGraph("Map_images/outercorner_narrow_bottom_left.png");
	outercorner_narrow_bottom_right = LoadGraph("Map_images/outercorner_narrow_bottom_right.png");
	outernose_left_top = LoadGraph("Map_images/outernose_left_top.png");
	outernose_left_bottom = LoadGraph("Map_images/outernose_left_bottom.png");
	outernose_top_left = LoadGraph("Map_images/outernose_top_left.png");
	outernose_top_right = LoadGraph("Map_images/outernose_top_right.png");
	outernose_right_top = LoadGraph("Map_images/outernose_right_top.png");
	outernose_right_bottom = LoadGraph("Map_images/outernose_right_bottom.png");
	wall_top = LoadGraph("Map_images/wall_top.png");
	wall_bottom = LoadGraph("Map_images/wall_bottom.png");
	wall_left = LoadGraph("Map_images/wall_left.png");
	wall_right = LoadGraph("Map_images/wall_right.png");
	corner_bottom_left = LoadGraph("Map_images/corner_bottom_left.png");
	corner_bottom_right = LoadGraph("Map_images/corner_bottom_right.png");
	corner_top_left = LoadGraph("Map_images/corner_top_left.png");
	corner_top_right = LoadGraph("Map_images/corner_top_right.png");
	gate_left = LoadGraph("Map_images/gate_left.png");
	gate_right = LoadGraph("Map_images/gate_right.png");
	door = LoadGraph("Map_images/door.png");
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
	mimg = mPac.img;
	mtype = mPac.type;

	AkabeiMapHitCheck();
	PinkeyMapHitCheck();
	AosukeMapHitCheck();
	GuzutaMapHitCheck();

	


	if (CheckHitKey(KEY_INPUT_B) != 0) {//Escキーが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0 || (g_NowKey & PAD_INPUT_7) != 0) {// ESCAPEキーが押されているかを調べる
		DxLib_End();
	}
	Player_Update();
	DrawMap();
	Enemy_Update();
	Item_Update();//アイテム用


}

//描画
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "Bキーを押してタイトル画面へ");
	Player_Draw();
	Enemy_Draw();
	Item_Draw();
	UI_Draw();

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			if (MapData[i][j] == 8) {//マップデータが8だったら
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, outercorner_top_left, TRUE);
			}
			if (MapData[i][j] == 9) {//マップデータが9だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_top_right, TRUE);
			}
			if (MapData[i][j] == 10) {//マップデータが10だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_bottom_left, TRUE);
			}
			if (MapData[i][j] == 11) {//マップデータが11だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_bottom_right, TRUE);
			}
			if (MapData[i][j] == 12) {//マップデータが12だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_narrow_top_left, TRUE);
			}
			if (MapData[i][j] == 13) {//マップデータが13だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_narrow_top_right, TRUE);
			}
			if (MapData[i][j] == 14) {//マップデータが14だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_narrow_bottom_left, TRUE);
			}
			if (MapData[i][j] == 15) {//マップデータが15だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outercorner_narrow_bottom_right, TRUE);
			}
			if (MapData[i][j] == 16) {//マップデータが16だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outernose_left_top, TRUE);
			}
			if (MapData[i][j] == 17) {//マップデータが17だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outernose_left_bottom, TRUE);
			}
			if (MapData[i][j] == 18) {//マップデータが18だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outernose_top_left, TRUE);
			}
			if (MapData[i][j] == 19) {//マップデータが19だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outernose_top_right, TRUE);
			}
			if (MapData[i][j] == 20) {//マップデータが20だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outernose_right_top, TRUE);
			}
			if (MapData[i][j] == 21) {//マップデータが21だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, outernose_right_bottom, TRUE);
			}
			if (MapData[i][j] == 22) {//マップデータが22だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, wall_top, TRUE);
			}
			if (MapData[i][j] == 23) {//マップデータが23だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, wall_bottom, TRUE);
			}
			if (MapData[i][j] == 24) {//マップデータが24だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, wall_left, TRUE);
			}
			if (MapData[i][j] == 25) {//マップデータが25だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, wall_right, TRUE);
			}
			if (MapData[i][j] == 26) {//マップデータが26だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, corner_bottom_left, TRUE);
			}
			if (MapData[i][j] == 27) {//マップデータが27だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, corner_bottom_right, TRUE);
			}
			if (MapData[i][j] == 28) {//マップデータが28だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, corner_top_left, TRUE);
			}
			if (MapData[i][j] == 29) {//マップデータが29だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, corner_top_right, TRUE);
			}
			if (MapData[i][j] == 30) {//マップデータが30だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, gate_left, TRUE);
			}
			if (MapData[i][j] == 31) {//マップデータが31だったら
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE, gate_right, TRUE);
			}
		}
	}
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

void AkabeiMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[Akabei.mapY][Akabei.mapX - 1] == 1 || MapData[Akabei.mapY][Akabei.mapX - 1] == 4) {
		Akabei.left = true;
	}
	else {
		Akabei.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[Akabei.mapY][Akabei.mapX + 1] == 1 || MapData[Akabei.mapY][Akabei.mapX + 1] == 4) {
		Akabei.right = true;
	}
	else {
		Akabei.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[Akabei.mapY - 1][Akabei.mapX] == 1 || MapData[Akabei.mapY - 1][Akabei.mapX] == 4) {
		Akabei.up = true;
	}
	else {
		Akabei.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[Akabei.mapY + 1][Akabei.mapX] == 1 || MapData[Akabei.mapY + 1][Akabei.mapX] == 4) {
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

void AosukeMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Aosuke.y / 16][((int)Aosuke.x / 16) - 1] == 1) {
		Aosuke.left = true;
	}
	else {
		Aosuke.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Aosuke.y / 16][((int)Aosuke.x / 16) + 1] == 1) {
		//DrawBox(Aosuke.x / 16, Aosuke.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Aosuke.right = true;
	}
	else {
		Aosuke.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Aosuke.y / 16) - 1][(int)Aosuke.x / 16] == 1) {
		Aosuke.up = true;
	}
	else {
		Aosuke.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Aosuke.y / 16) + 1][(int)Aosuke.x / 16] == 1) {
		Aosuke.bottom = true;
	}
	else {
		Aosuke.bottom = false;
	}
}

void GuzutaMapHitCheck() {
	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Guzuta.y / 16][((int)Guzuta.x / 16) - 1] == 1) {
		Guzuta.left = true;
	}
	else {
		Guzuta.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Guzuta.y / 16][((int)Guzuta.x / 16) + 1] == 1) {
		//DrawBox(Guzuta.x / 16, Guzuta.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Guzuta.right = true;
	}
	else {
		Guzuta.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Guzuta.y / 16) - 1][(int)Guzuta.x / 16] == 1) {
		Guzuta.up = true;
	}
	else {
		Guzuta.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Guzuta.y / 16) + 1][(int)Guzuta.x / 16] == 1) {
		Guzuta.bottom = true;
	}
	else {
		Guzuta.bottom = false;
	}
}