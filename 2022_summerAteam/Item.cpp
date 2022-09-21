#include "Item.h"
#include "Dxlib.h"
#include "Game.h"
#include "Player.h"
#include "UI.h"
#include "Enemy.h"

int Dot_Handle;
int Powerdot_Handle[DOT_IMAGE_MAX];
int Fruits_Handle[FRUIT_IMAGE_MAX];

int Cnt;
int Powerdot_Image;

int Score;//スコア
int DotCnt;//エサをカウントする変数
int DotsLeft;//残りのエサ数
int FruitCnt;//フルーツをカウントする変数
int Round;//ラウンドチェンジ

bool PowerUpFlg;//パックマンのパワーアップフラグ

int PowerUpTime;//パワーアップの時間
int FruitTime;//フルーツの出現時間
struct FRUITS Fruits;		//フルーツ構造体
int CntTime;//カウントする変数
bool FruiScoreUI;//フルーツのUIのフラグ

int Item_Mapdata[MAP_HEIGHT][MAP_WIDTH]//０=エサなし １=エサ ２=パワーエサ ３=フルーツ
{
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//0
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },//1
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//2
 { 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0 },//3
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//4
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },//5
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//6
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//7
 { 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },//8
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//9
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//10
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//11
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//12
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//13
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//14

 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//15
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//16
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 ,  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//17
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//18
 { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },//19
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },//20
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//21
 { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 },//22
 { 0, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 0, 0 },//23
 { 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },//24
 { 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,   0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,   0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,   0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
 { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int HitCheck(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {

	//横の当たり判定
	float CharaL = x1;		//１のx軸
	float CharaR = x1 + w1;	//１のx軸 + 幅
	float ItemL = x2;		//２のx軸
	float ItemR = x2 + w2;	//２のx軸 + 幅
	
	//縦の当たり判定
	float CharaU = y1;		//１のy軸
	float CharaD = y1 + h1;	//１のy軸 + 高さ
	float ItemU = y2;		//２のy軸
	float ItemD = y2 + h2;	//２のy軸 + 高さ
	
	if (CharaL >= ItemR) {	//キャラの左端がアイテムの右端よりが右にあるとき
		return 0;			//当たってない
	}
	if (CharaR <= ItemL) {	//キャラの右端がアイテムの左端より左にあるとき
		return 0;			//当たってない
	}
	if (CharaU >= ItemD) {	//キャラの上端がアイテムの下端より下にあるとき
		return 0;			//当たってない
	}
	if (CharaD <= ItemU) {	//キャラの下端がアイテムの上端より上にあるとき
		return 0;			//当たってない
	}
	return 1;
}

void Item_Initialize() {
	Dot_Handle = LoadGraph("Item_images/dot.png");
	LoadDivGraph("Item_Images/big_dot2.png", 2, 2, 1, 16, 16, Powerdot_Handle);
	LoadDivGraph("Item_Images/fruit.png", 13, 13, 1, 16, 16, Fruits_Handle);
	Cnt = 0;
	Score = 0;
	DotCnt = 0;
	FruitCnt = 0;
	PowerUpFlg = false;
	PowerUpTime = 510;
	FruitTime = 660;
	DotsLeft = 244;
	Round = 1;
	Fruits.fScore = Fruits.Cherry;
	CntTime = 120;
	FruiScoreUI = false;
}
void Item_Finalize() {
	DeleteGraph(Dot_Handle);
	//DeleteGraph(Powerdot_Handle[2]);
}
void Item_Update() {
	HitItem();//アイテムのヒット関数
	PowerdotAnim();//パワーエサのアニメーション関数
	PowerUp();//パワーアップの関数
	FruitTerget();//フルーツ出現関数

	if (DotsLeft == 0) {//残りエサがなくなったとき
		RoundChange();//ラウンドチェンジ
	}
	if (FruiScoreUI == true) {
		ScoreUIEnabled();//フルーツUI
	}
}
void Item_Draw() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 1) {//マップデータが１だったら
				//エサを描画
				//DrawGraph(j * MAP_SIZE, i * MAP_SIZE, Dot_Handle, TRUE);
				//DrawBox(j * MAP_SIZE, i * MAP_SIZE, j * MAP_SIZE * MAP_SIZE, i * MAP_SIZE * MAP_SIZE, GetColor(0, 255, 0), FALSE);//デバッグ用
				DrawRotaGraph(j * MAP_SIZE + 24, i * MAP_SIZE + 24, 1.0f, 0, Dot_Handle, TRUE);
			}
			if (Item_Mapdata[i][j] == 2) {//マップデータが２だったら
				//パワーエサを描画
				DrawGraph(j * MAP_SIZE +16, i * MAP_SIZE + 16, Powerdot_Handle[Powerdot_Image], TRUE);
			}
			if (Item_Mapdata[i][j] == 3) {//マップデータが３だったら
				//フルーツを描画
				DrawRotaGraph(j * FRUIT_SIZE, i * FRUIT_SIZE - 8, 1.0f, 0, Fruits_Handle[Fruits.kind], TRUE, FALSE);
			}
		}
	}
	FruitsUI();//フルーツのスコアを表示する関数


	DrawFormatString(1000, 10, GetColor(255, 255, 255), "Score:%d", Score);//でバッグ
	DrawFormatString(1000, 30, GetColor(255, 255, 255), PowerUpFlg ? "PowerUp" : "Normal");//でバッグ
	DrawFormatString(1000, 50, GetColor(255, 255, 255), "PowerUpTime:%d", PowerUpTime);//でバッグ
	DrawFormatString(1000, 70, GetColor(255, 255, 255), "Dots Left:%d/244", DotsLeft);//でバッグ
	DrawFormatString(1000, 90, GetColor(255, 255, 255), "Fruits Target:%d", FruitTime);//でバッグ
	DrawFormatString(1000, 110, GetColor(255, 255, 255), "ROUND:%d", Round);//でバッグ
	DrawFormatString(1000, 130, GetColor(255, 255, 255), "CntTime:%d", CntTime);//でバッグ
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
			if (HitCheck(mPac.x - 24, mPac.y - 24, mPac.w, mPac.h, j * MAP_SIZE, i * MAP_SIZE, MAP_SIZE, MAP_SIZE)) {
				if (Item_Mapdata[i][j] == 1) {//食べたのがエサだったの場合
					Item_Mapdata[i][j] = 4;//エサを消す
					Score += 10;//スコア＋１０
					DotCnt++;	//食べた個数＋１
					DotsLeft--;	//残りの個数ー１
					esa = 1;
				}
				if (Item_Mapdata[i][j] == 2) {//食べたのがパワーエサだった場合
					Item_Mapdata[i][j] = 5;//パワーエサを消す
					Score += 50;//スコア＋５０
					DotCnt++;	//食べた個数＋１
					DotsLeft--;	//残りの個数ー１
					PowerUpFlg = true;//パワーアップフラグをtrueにする
					esa = 2;
					Run = false;
				}
			}
			//フルーツ用のヒットチェック
			if (HitCheck(mPac.x - 4, mPac.y - 4, mPac.w - 8, mPac.h - 8, j * MAP_SIZE - 8, i * MAP_SIZE - 8, MAP_SIZE, MAP_SIZE))
			{
				if (Item_Mapdata[i][j] == 3) {//食べたのがフルーツだった場合
					Item_Mapdata[i][j] = 0;//フルーツを消す
					Score += Fruits.fScore;//スコア＋１００
					FruitCnt++;
					FruitTime = 660;
					FruiScoreUI = true;
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

void FruitTerget() {

	//if (DotCnt >= 58 && FruitCnt == 0) {//エサを５８個食べたら
	if (DotsLeft <= 186 && FruitCnt == 0) {//エサを５８個食べたら
		if (FruitTime >= 0) {			//
			FruitTime--;				//６６０フレームの間
			Item_Mapdata[19][15] = 3;	//フルーツが出現する
		}
		else if (FruitTime == -1) {		//６６０フレーム経過すると

			Item_Mapdata[19][15] = 0;	//フルーツが消える
			FruitCnt++;
			FruitTime = 660;
		}
	}
	//if (DotCnt >= 137 && FruitCnt == 1) {//エサを１３７個食べたら
	if (DotsLeft <= 107 && FruitCnt == 1) {//エサを１３７個食べたら
		if (FruitTime >= 0) {			//
			FruitTime--;				//６６０フレームの間
			Item_Mapdata[19][15] = 3;	//フルーツが出現する
		}
		else if (FruitTime == -1) {		//660フレーム経過すると

			Item_Mapdata[19][15] = 0;	//フルーツが消える
			FruitCnt++;
			FruitTime = 660;
		}
	}
}

void RoundChange() {
	Cnt = 0;//初期化
	DotCnt = 0;//初期化
	FruitCnt = 0;//初期化
	PowerUpFlg = false;//初期化
	PowerUpTime = 510;//初期化
	FruitTime = 660;//初期化
	DotsLeft = 244;//初期化
	Round++;//初期化

	mPac.flg = true;
	mPac.type = 0;
	mPac.img = 3;
	mPac.x = 240.0f;
	mPac.y = 392.0f;

	if (Fruits.kind < 12) {
		Fruits.kind++; //フルーツの種類++
	}
	if (Round == 1) {//ラウンド１
		Fruits.fScore = Fruits.Cherry;//チェリー
	}
	else if (Round == 2) {//ラウンド２
		Fruits.fScore = Fruits.Strowberry;//ストロベリー
	}
	else if (Round == 3) {//ラウンド３～
		Fruits.fScore = Fruits.Orenge;//オレンジ
	}
	else if (Round == 5) {//ラウンド５～
		Fruits.fScore = Fruits.Apple;//アップル
	}
	else if (Round == 7) {//ラウンド７～
		Fruits.fScore = Fruits.Melon;//メロン
	}
	else if (Round == 9) {//ラウンド９～
		Fruits.fScore = Fruits.Garraxy;//ボス・ギャラクシアン
	}
	else if (Round == 11) {//ラウンド１１～
		Fruits.fScore = Fruits.Bell;//ベル
	}
	else if (Round == 13) {//ラウンド１３以降
		Fruits.fScore = Fruits.Key;//鍵
	}

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 4) {
				Item_Mapdata[i][j] = 1;//エサ再配置
			}if (Item_Mapdata[i][j] == 5) {
				Item_Mapdata[i][j] = 2;//パワーエサ再配置
			}
		}
	}
}

void ScoreUIEnabled() {
	if (CntTime > 0) {
		CntTime--;//２秒間の間
		//スコアUI表示
		DrawFormatString(15 * MAP_SIZE - 16, 19 * MAP_SIZE - 16, GetColor(255, 255, 255), "%d", Fruits.fScore);//でバッグ

	}
	else if (CntTime == 0) {
		FruiScoreUI = false;
		CntTime = 120;
	}
}

void FruitsUI() {

	for (int i = 1; i <= Round; i++) {
		DrawRotaGraph(454 + FRUIT_SIZE * i, 316, 1.0f, 0, Fruits_Handle[i - 1], TRUE, FALSE);
	}
}