#pragma once
#ifndef _INIT_H_
#define _INIT_H_
#include "framework.h"
#include "Entity.h"
#include "GameProc.h"

void Readini(LEVEL l);
void Initmap(LEVEL l, Entity* headblock);
void Init(LEVEL l);
void InitEntity(Entity* headenemy, Entity* headblock, Entity* headbullet, Entity* headenemybullet, Entity* player,Entity* landmine, LEVEL l);
int Randomfire();
int Randomdir();
int Getbirthtime();
void Enemybirth(Entity* headenemy, int type, int w);
void putmine(Entity* mi,int x1,int y1);
int Getpscore();
int Gettimestep();
HBITMAP Getlogopic();
HBITMAP Getsuccpic();
HBITMAP Getfailpic();


#endif
