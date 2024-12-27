#pragma once
#ifndef _GAME_PROC_
#define _GAME_PROC_
#include "framework.h"
#include "Entity.h"
enum LEVEL{OPEN,LEVEL1,LEVEL2_OPEN,LEVEL2,SUCCEED,FAIL};

void Enterkey(int key);
void Newentity();
void Changewndsize(int w, int n);
void Changelevel(LEVEL newl);
void Resetplayer(Entity* player);
void Draw(HDC hdc, HWND hWnd);
void Drawscene(HDC hdc, LEVEL curl);
void Drawgamescene(HDC hdc);
void Update(int ts);
void Enemybirth();
void Destroy();

#endif
