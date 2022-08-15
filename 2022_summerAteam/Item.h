#pragma once


#define MAP_SIZE 32 // スクリーンサイズから見たマップサイズを定義	//アイテム用
#define MAP_WIDTH 19 // マップの横幅をMAP_WIDTHと定義				//アイテム用
#define MAP_HEIGHT 15 // マップの縦幅をMAP_HEIGHTと定義				//アイテム用

void Item_Initialize();
void Item_Finalize();
void Item_Update();
void Item_Draw();