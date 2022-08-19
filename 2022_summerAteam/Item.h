#pragma once


#define MAP_SIZE 16 // スクリーンサイズから見たマップサイズを定義	//アイテム用
#define MAP_WIDTH 30 // マップの横幅をMAP_WIDTHと定義				//アイテム用
#define MAP_HEIGHT 33 // マップの縦幅をMAP_HEIGHTと定義				//アイテム用

#define DOT_IMAGE_MAX 2
#define CNT_SPEED 15

void Item_Initialize();
void Item_Finalize();
void Item_Update();
void Item_Draw();

void PowerdotAnim();