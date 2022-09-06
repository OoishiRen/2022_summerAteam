#pragma once


#define MAP_SIZE 16 // スクリーンサイズから見たマップサイズを定義	//アイテム用
#define MAP_WIDTH 30 // マップの横幅をMAP_WIDTHと定義				//アイテム用
#define MAP_HEIGHT 33 // マップの縦幅をMAP_HEIGHTと定義				//アイテム用

#define FRUIT_SIZE 16

#define DOT_IMAGE_MAX 2
#define FRUIT_IMAGE_MAX 13
#define CNT_SPEED 15


struct FRUITS {
	int kind = 0;
	int fScore;
	int Cherry = 100;
	int Strowberry = 300;
	int Orenge = 500;
	int Apple = 700;
	int Melon = 1000;
	int Garraxy = 2000;
	int Bell = 3000;
	int Key = 5000;
};

extern struct FRUITS Fruits;		//フルーツ構造体

extern int Score;
extern int DotCnt;

extern int PowerUpTime;

extern bool PowerUpFlg;
extern int Round;

void Item_Initialize();
void Item_Finalize();
void Item_Update();
void Item_Draw();

void PowerdotAnim();

extern int HitCheck(float, float, float, float, float, float, float, float);
void HitItem();
void PowerUp();
void FruitTerget();
void RoundChange();

void ScoreUIEnabled();
void FruitsUI();