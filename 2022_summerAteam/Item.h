#pragma once


#define MAP_SIZE 16 // �X�N���[���T�C�Y���猩���}�b�v�T�C�Y���`	//�A�C�e���p
#define MAP_WIDTH 30 // �}�b�v�̉�����MAP_WIDTH�ƒ�`				//�A�C�e���p
#define MAP_HEIGHT 33 // �}�b�v�̏c����MAP_HEIGHT�ƒ�`				//�A�C�e���p

#define DOT_IMAGE_MAX 2
#define CNT_SPEED 15

void Item_Initialize();
void Item_Finalize();
void Item_Update();
void Item_Draw();

void PowerdotAnim();