#pragma once

#define MAP_HEIGHT 33
#define MAP_WIDTH 30

void Game_Initialize();//������
void Game_Finalize();//�I������
void Game_Update();//�X�V
void Game_Draw();//�`��

void WarpTunnel();

extern int MapData[MAP_HEIGHT][MAP_WIDTH];

extern void AkabeiMapHitCheck();
extern void PinkeyMapHitCheck();
extern void AosukeMapHitCheck();
extern void GuzutaMapHitCheck();