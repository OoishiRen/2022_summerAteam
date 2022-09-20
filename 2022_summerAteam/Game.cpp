#include "Game.h"
#include "SceneMgr.h"
#include"Input.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "UI.h"

static int mImageHandle; //画像ハンドル格納用変数
static int MapWallLeft_Handle;
static int MapWallRight_Handle;
static int MapWallUP_Handle;
static int MapWallbottom_Handle;
static int MapWall_left_top;
static int MapWall_narrow_top_left;
static int MapWall__narrow_top_right;
static int MapWall_narrow_bottom_left;
static int MapWall_narrow_bottom_right;
static int MapWall_left_bottom;
static int MapWall_outerwall_right;
static int MapWall_gate_left;
static int MapWall_gate_right;
static int MapWall_outrcorner_top_left;
static int MapWallgate_left;
static int MapWallgate_right;
static int MapWalldoor;
static int MapWall_narrow_left;
static int MapWalloutercorner_bottom_left;
static int MapWall_outerwall_Left;
static int MapWalloutercorner_bottom_right;
static int MapWallnarrow_right;
static int MapWallcorner_top_left;
static int MapWallcorner_top_right;
static int MapWallcorner_bottom_left;
static int MapWallcorner_bottom_right;
static int wall_left;
static int wall_right;
static int wall_top;
static int wall_bottom;

float mx = 0, my = 0;
int mimg = 0, mtype = 0;	//画像保存用


/**********仮のマップチップ（消してもいい）**********/
int MapData[MAP_HEIGHT][MAP_WIDTH] = //マップデータ 1は壁がある 0は壁がない　
{
 {18, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 17, 0 },//0 4は上
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },//1
 { 1, 0, 0, 0, 0, 0, 0, 18, 4, 4, 4,17,0, 1,    1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//2
 { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 6, 0, 1,    1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//3
 { 1, 0, 0, 0, 0, 0, 0, 19,5, 5, 5, 20,0, 1,    1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//4
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },//5
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,    1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//6
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,    1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 6, 0 },//7
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 6, 0 },//8
 { 0, 4, 4, 4, 4, 17, 0, 0, 0, 0, 0, 0, 0, 1,    1, 0, 1, 1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 0, 0 },//9
 { 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 1,    1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//10
 { 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//11
 { 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 18, 4,14,13,   13,15,4, 17,0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//12
 { 5, 5, 5, 5, 5, 20,0, 0, 0, 0, 1, 0, 0, 0,    0, 0, 0, 6, 0, 1, 1, 0, 0, 5, 5, 5, 5, 5, 0 },//13

 { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,    0 ,0 ,0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },//14

 { 4, 4, 4, 4, 4, 17, 0, 1, 1, 0, 1, 0, 0, 0,   0, 0, 0, 6, 0, 1, 1, 0, 0, 4, 4, 4, 4, 4, 0 },//15
 { 0, 0, 0, 0, 0, 6, 0, 1, 1, 0, 19, 5, 5, 5,   5, 5, 5, 20,0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//16
 { 0, 0, 0, 0, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//17
 { 0, 0, 0, 0, 0, 6, 0, 1, 1, 0, 1, 1, 1, 1,    1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//18
 { 0, 5, 5, 5, 5, 20, 0, 1, 1, 0, 1, 1, 1,1,    1, 1, 1, 1, 0, 1, 1, 0, 0, 5, 5, 5, 5, 0, 0 },//19
 { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },//20
 { 1, 0, 1, 1, 1, 1, 0, 0 ,0 ,0 ,0 ,0 ,0, 1,    1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 6, 0 },//21
 { 1, 0, 1, 1, 1, 1, 0, 25,11,11,11,26,0, 1,    1, 0, 25, 11,11,11,11,26, 1, 1, 1, 1, 0, 6, 0 },//22
 { 1, 0, 0, 0, 1, 1, 0, 23,12,12,12,24,0, 0,    0, 0, 23, 12,12,12,12,24, 1, 1, 0, 0, 0, 6, 0 },//23
 { 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1,    1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 6, 0 },//24
 { 1, 1, 1, 0, 1, 1, 0, 25,26,0, 19, 5, 5,27,   1, 1, 1, 1, 0, 25,26,0, 1, 1, 0, 1, 1, 6, 0 },
 { 1, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 0, 0,1,    1, 0, 0, 0, 0, 9, 10, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 1, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 0, 0,1,    1, 0, 0, 0, 0, 9, 10, 0, 0, 0, 0, 0, 0, 6, 0 },
 { 1, 0, 25,11,11,11,11,24,23,11,11,11,26,19,   20, 0, 25, 11,11,24, 23, 11, 11, 11, 11, 11, 26, 6, 0 },
 { 1, 0, 23,12,12,12,12,12,12,12,12,12,24,0,    0, 0, 23, 12,12,12, 12, 12, 12, 12, 12, 12, 24, 6, 0 },
 { 19,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 20,0 },//5は下
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/**********仮のマップチップ（消してもいい）**********/


void DrawMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {
				//DrawBox(j * MAP_SIZE, i * MAP_SIZE, j *	MAP_SIZE + MAP_SIZE, i * MAP_SIZE + MAP_SIZE, GetColor(0, 0, 255), TRUE);

				if (HitCheck(Akabei.x, Akabei.y, Akabei.w, Akabei.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
					Akabei.x = Akabei.mx;
					Akabei.y = Akabei.my;
					if (Akabei.ed == Akabei.md) {
						Akabei.WallHit = true;
					}
				}
			}

			if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE + 8, i * MAP_SIZE + 8, MAP_SIZE, MAP_SIZE)) {
				if (MapData[i][j] == 1) {
					mPac.x = mx;
					mPac.y = my;
					mPac.img = mimg;
					mPac.type = mtype;
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
	//mImageHandle = LoadGraph("Item_images/pac-manRP2.png"); //画像のロード
	MapWallUP_Handle     = LoadGraph("Map_images/outerwall_top.png");//外上壁※済
	MapWallLeft_Handle   = LoadGraph("Map_images/outercorner_narrow_top_left.png");//角左
	MapWallLeft_Handle   = LoadGraph("Map_images/outerwall_left.png");//外左壁※済
	MapWallRight_Handle  = LoadGraph("Map_images/outerwall_right.png");//外右壁※済
	MapWallbottom_Handle = LoadGraph("Map_images/outerwall_bottom.png");//外下壁※済
	wall_left   = LoadGraph("Map_images/wall_left.png");//左壁※済
	wall_right  = LoadGraph("Map_images/wall_right.png");//右壁※済
	wall_top    = LoadGraph("Map_images/wall_top.png");//上壁※済
	wall_bottom = LoadGraph("Map_images/wall_bottom.png");//下壁※済
	MapWall_left_top = LoadGraph("Map_images/outernose_left_top.png");//出るやつ※済
	MapWall_left_bottom = LoadGraph("Map_images/outernose_left_bottom.png");//出るやつ※済
	MapWall_outerwall_right = LoadGraph("Map_images/outercorner_top_right.png");//上角右※済
	MapWall_outrcorner_top_left = LoadGraph("Map_images/outercorner_top_left.png");//上角左※済
	MapWall_outerwall_Left = LoadGraph("Map_images/outerwall_left.png");//角左
	MapWalldoor = LoadGraph("Map_images/door.png");//door※済
	MapWallgate_left = LoadGraph("Map_images/gate_left.png");//ゲート左※済
	MapWallgate_right = LoadGraph("Map_images/gate_right.png");//ゲート右※済
	MapWall_narrow_left = LoadGraph("Map_images/outercorner_narrow_bottom_left.png");//下角(左)
	MapWallnarrow_right = LoadGraph("Map_images/outercorner_narrow_bottom_right.png");//下角(右)
	MapWall_narrow_top_left = LoadGraph("Map_images/outercorner_narrow_top_left.png");//上角(左)
	MapWall__narrow_top_right = LoadGraph("Map_images/outercorner_narrow_top_right.png");//上角(右)
	MapWalloutercorner_bottom_left = LoadGraph("Map_images/outercorner_bottom_left.png");//外下角左※済
	MapWalloutercorner_bottom_right = LoadGraph("Map_images/outercorner_bottom_right.png");//外下角右※済
	MapWallcorner_bottom_left = LoadGraph("Map_images/corner_bottom_left.png");//23
	MapWallcorner_bottom_right = LoadGraph("Map_images/corner_bottom_right.png");//24
	MapWallcorner_top_left = LoadGraph("Map_images/corner_top_left.png");//25
	MapWallcorner_top_right = LoadGraph("Map_images/corner_top_right.png");//26
}

//終了処理
void Game_Finalize() {

	Player_Finalize();
	Enemy_Finalize();
	Item_Finalize();//アイテム用
	DeleteGraph(mImageHandle); //画像の解放
	DeleteGraph(MapWallLeft_Handle); //画像の解放
	DeleteGraph(MapWallRight_Handle);
	DeleteGraph(MapWallUP_Handle);
	DeleteGraph(MapWallbottom_Handle);
	DeleteGraph(MapWall_left_bottom);
	DeleteGraph(MapWallnarrow_right);
}

//更新
void Game_Update() {
	mx = mPac.x;
	my = mPac.y;
	mimg = mPac.img;
	mtype = mPac.type;

	Pinkey.mx = Pinkey.x;
	Pinkey.my = Pinkey.y;


	// 左に壁があるときにフラグをtrueにする
	if (MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) - 1] == 1) {
		Akabei.left = true;
	}
	else {
		Akabei.left = false;
	}

	// 右に壁があるときにフラグをtrueにする
	if (MapData[(int)Akabei.y / 16][((int)Akabei.x / 16) + 1] == 1) {
		//DrawBox(Akabei.x / 16, Akabei.y / 16, 1, 1, GetColor(0, 255, 0), TRUE);
		Akabei.right = true;
	}
	else {
		Akabei.right = false;
	}

	// 上に壁があるときにフラグをtrueにする
	if (MapData[((int)Akabei.y / 16) - 1][(int)Akabei.x / 16] == 1) {
		Akabei.up = true;
	}
	else {
		Akabei.up = false;
	}

	// 下に壁があるときにフラグをtrueにする
	if (MapData[((int)Akabei.y / 16) + 1][(int)Akabei.x / 16] == 1) {
		Akabei.bottom = true;
	}
	else {
		Akabei.bottom = false;
	}


	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Escキーが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
	}
	Player_Update();
	Enemy_Update();
	DrawMap();
	DrawGraph(0,0,mImageHandle,TRUE);
	Item_Update();//アイテム用


}

//描画
void Game_Draw() {

	//DrawGraph(0, 0, mImageHandle, FALSE);
	DrawFormatString(10, 700, 0xffffff, "ESCキーを押してメニュー画面へ");
	Player_Draw();
	Enemy_Draw();
	Item_Draw();
	UI_Draw();
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (MapData[i][j] == 1) {//マップデータが１だったら
				//左の壁を描画
				//DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Dot_Handle, TRUE);
				DrawRotaGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 8, 1.0f, 0, MapWallLeft_Handle, TRUE);
			}
			if (MapData[i][j] == 4) {//マップデータが4だったら
				//上の壁を描画
				DrawRotaGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 8, 1.0f, 0, MapWallUP_Handle, TRUE);
			}
			if (MapData[i][j] == 5) {//マップデータが5だったら
				//下の壁を描画
				DrawRotaGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 8, 1.0f, 0, MapWallbottom_Handle, TRUE);
			}
			if (MapData[i][j] == 6) {//マップデータが6だったら
				//右の壁を描画
				DrawGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 0, MapWallRight_Handle, TRUE);
			}
			if (MapData[i][j] == 7) {//マップデータが7だったら
				//
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWall_left_top, TRUE, FALSE);
			}
			if(MapData[i][j] == 8) {//マップデータが8だったら
				//
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWall_left_bottom, TRUE, FALSE);
			}
			if (MapData[i][j] == 9) {//マップデータが9だったら
				//
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_left, TRUE, FALSE);
			}
			if (MapData[i][j] == 10) {//マップデータが10だったら
				//
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_right, TRUE, FALSE);
			}
			if (MapData[i][j] == 11) {//マップデータが11だったら
				//
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_top, TRUE, FALSE);
			}
			if (MapData[i][j] == 12) {//マップデータが12だったら
				//
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, wall_bottom, TRUE, FALSE);
			}
			if (MapData[i][j] == 13) {//マップデータが13だったら
				//
				DrawRotaGraph(j * MAP_SIZE+ 8, i * MAP_SIZE + 2, 1.0f, 0, MapWalldoor, TRUE);
			}
			if (MapData[i][j] == 14) {//マップデータが14だったら
				//
				DrawRotaGraph(j * MAP_SIZE + 8, i * MAP_SIZE+ 2, 1.0f, 0, MapWallgate_left, TRUE);
			}
			if (MapData[i][j] == 15) {//マップデータが15だったら
				//
				DrawRotaGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 2, 1.0f, 0, MapWallgate_right, TRUE);
			}
			if (MapData[i][j] == 16) {//マップデータが16だったら
				//
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWallLeft_Handle, TRUE);
			}
			if (MapData[i][j] == 17) {//マップデータが17だったら
				//外側の角右
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWall_outerwall_right, TRUE);
			}
			if (MapData[i][j] == 18) {//マップデータが18だったら
				//外側の角右
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWall_outrcorner_top_left, TRUE);
			}
			if (MapData[i][j] == 19) {//マップデータが19だったら
				//外側の角右
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWalloutercorner_bottom_left, TRUE);
			}
			if (MapData[i][j] == 20) {//マップデータが20だったら
				//外側の角右
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWalloutercorner_bottom_right, TRUE);
			}
			if (MapData[i][j] == 21) {//マップデータが21だったら
				//外側の角右
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, MapWall_narrow_left, TRUE);
			}
			if (MapData[i][j] == 22) {//マップデータが22だったら
				//外側の角右
				DrawRotaGraph(j * MAP_SIZE+8, i * MAP_SIZE+8, 1.0f, 0, MapWallnarrow_right, TRUE);
			}
			if(MapData[i][j] == 23) {//マップデータが23だったら
				//外側の角右
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWallcorner_bottom_left, TRUE, FALSE);
			}
			if(MapData[i][j] == 24) {//マップデータが24だったら
				//外側の角右
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWallcorner_bottom_right, TRUE, FALSE);
			}
			if (MapData[i][j] == 25) {//マップデータが25だったら
				//外側の角右
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWallcorner_top_left, TRUE, FALSE);
			}
			if (MapData[i][j] == 26) {//マップデータが26だったら
				//外側の角右
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWallcorner_top_right, TRUE, FALSE);
			}
			if (MapData[i][j] == 27) {//マップデータが27だったら
				//外側の角右
				DrawRotaGraph(j * FRUIT_SIZE+8, i * FRUIT_SIZE + 8, 1.0f, 0, MapWall__narrow_top_right, TRUE, FALSE);
			}
			if (MapData[i][j] == 28) {//マップデータが28だったら
				//外側の角右
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, MapWall_narrow_top_left, TRUE, FALSE);
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