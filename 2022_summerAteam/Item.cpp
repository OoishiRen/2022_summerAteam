#include "Item.h"
#include "Dxlib.h"
#include "Game.h"

int Dot_Handle;
int Powerdot_Handle;

int Item_Mapdata[MAP_HEIGHT][MAP_WIDTH]{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
	{0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

};

void Item_Initialize() {
	Dot_Handle = LoadGraph("Item_images/dot.png");
	Powerdot_Handle = LoadGraph("Item_Images/big_dot.png");
}
void Item_Finalize() {
	DeleteGraph(Dot_Handle);
	DeleteGraph(Powerdot_Handle);
}
void Item_Update() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (Item_Mapdata[i][j] == 1) {
				////DrawPixel(j * MAP_SIZE+16, i * MAP_SIZE+16, GetColor(255, 255, 255));//ピクセル
				DrawGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 8, Dot_Handle, TRUE);
			}
			if (Item_Mapdata[i][j] == 2) {
				////DrawPixel(j * MAP_SIZE+16, i * MAP_SIZE+16, GetColor(255, 255, 255));//ピクセル
				DrawGraph(j * MAP_SIZE + 8, i * MAP_SIZE + 8, Powerdot_Handle, TRUE);
			}
		}
	}
}
void Item_Draw() {

}