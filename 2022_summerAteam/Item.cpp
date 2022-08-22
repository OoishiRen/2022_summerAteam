#include "Item.h"
#include "Dxlib.h"
#include "Game.h"
#include "Player.h"

int Dot_Handle;
int Powerdot_Handle[DOT_IMAGE_MAX];

int Cnt;
int Powerdot_Image;

int Score;

bool PowerUpFlg;

int PowerUpTime;

int Item_Mapdata[MAP_HEIGHT][MAP_WIDTH]
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },

 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },

 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 0, 0 },
 { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int HitCheck(float Cx, float Cy, float Cw, float Ch, float Ix, float Iy, float Iw, float Ih) {

	//横の当たり判定
	float CharaL = Cx;		//キャラのx軸
	float CharaR = Cx + Cw;	//キャラのx軸 + 幅
	float ItemL = Ix;		//アイテムのx軸
	float ItemR = Ix + Iw;	//アイテムのx軸 + 幅

	//縦の当たり判定
	float CharaU = Cy;		//キャラのy軸
	float CharaD = Cy + Ch;	//キャラのy軸 + 高さ
	float ItemU = Iy;		//アイテムのy軸
	float ItemD = Iy + Ih;	//アイテムのy軸 + 高さ

	if (CharaL >= ItemR) {
		return 0;
	}
	if (CharaR <= ItemL) {
		return 0;
	}
	if (CharaU >= ItemD) {
		return 0;
	}
	if (CharaD <= ItemU) {
		return 0;
	}
}

void Item_Initialize() {
	Dot_Handle = LoadGraph("Item_images/dot.png");
	LoadDivGraph("Item_Images/big_dot2.png", 2, 2, 1, 16, 16, Powerdot_Handle);
	Cnt = 0;
	Score = 0;
	PowerUpFlg = false;
	PowerUpTime = 510;
}
void Item_Finalize() {
	DeleteGraph(Dot_Handle);
	//DeleteGraph(Powerdot_Handle[2]);
}
void Item_Update() {
	HitItem();//アイテムのヒット関数
	PowerdotAnim();//パワーエサのアニメーション関数
	PowerUp();//パワーアップの関数
}
void Item_Draw() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 1) {//マップデータが１だったら
				//エサを描画
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Dot_Handle, TRUE);
			}
			if (Item_Mapdata[i][j] == 2) {//マップデータが２だったら
				//パワーエサを描画
				DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Powerdot_Handle[Powerdot_Image], TRUE);
			}
		}
	}


	DrawFormatString(0, 10, GetColor(255, 255, 255), "Score:%d", Score);//でバッグ
	DrawFormatString(0, 30, GetColor(255, 255, 255), PowerUpFlg ? "PowerUp" : "Normal");//でバッグ
	DrawFormatString(0, 50, GetColor(255, 255, 255), "PowerUpTime:%d", PowerUpTime);//でバッグ
}

void PowerdotAnim() {

	//Cntが１５になるまで増やす
	if (Cnt < CNT_SPEED) {
		Cnt++;
	}
	else if (Cnt == CNT_SPEED) {
		Cnt = 0;
	}

	if (Cnt < CNT_SPEED / 2) {	//Cntが１５以下だったら
		Powerdot_Image = 0;		//表示
	}
	else if (Cnt > CNT_SPEED / 2 && Cnt < CNT_SPEED) {	//Cntが15以上なら
		Powerdot_Image = 1;		//非表示
	}
}

void HitItem() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			//ヒットチェック
			if (HitCheck(mPac.x, mPac.y, mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, j, i)) {
				if (Item_Mapdata[i][j] == 1) {//食べたのがエサだったの場合
					Item_Mapdata[i][j] = 0;//エサを消す
					Score += 10;//スコア＋１０
				}
				if (Item_Mapdata[i][j] == 2) {//食べたのがパワーエサだった場合
					Item_Mapdata[i][j] = 0;//パワーエサを消す
					Score += 50;//スコア＋５０
					PowerUpFlg = true;//パワーアップフラグをtrueにする
				}
			}
		}
	}
}

void PowerUp() {
	if (PowerUpFlg == true && PowerUpTime != 0) {//パワーアップフラグがtrueになったら
		PowerUpTime--;			//５１０フレームの間パワーアップ
	}
	else if (PowerUpTime == 0) {//５１０フレーム経過で
		PowerUpFlg = false;		//パワーアップフラグをfalseにする
		PowerUpTime = 510;
	}
}