#pragma once

#define MAP_HEIGHT 33
#define MAP_WIDTH 30

void Game_Initialize();//初期化
void Game_Finalize();//終了処理
void Game_Update();//更新
void Game_Draw();//描画

void WarpTunnel();

extern int MapData[MAP_HEIGHT][MAP_WIDTH];

extern void AkabeiMapHitCheck();
extern void PinkeyMapHitCheck();
extern void AosukeMapHitCheck();
extern void GuzutaMapHitCheck();