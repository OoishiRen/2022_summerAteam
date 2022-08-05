#include<stdio.h>
#include"DxLib.h"
#include"Game.h"

int	g_OldKey;				
int	g_NowKey;				
int	g_KeyFlg;			

void InputKey(void) {
	//“ü—ÍƒL[æ“¾
	g_OldKey = g_NowKey;
	g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	g_KeyFlg = g_NowKey & ~g_OldKey;
}