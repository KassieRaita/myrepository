#pragma once
#ifndef  _ENTITY_H_
#define _ENTITY_H_
#include "framework.h"
enum Dir{UP,DOWN,RIGHT,LEFT,NONE};
typedef struct entity {
	int x, y;
	Dir dir;
	int v;
	int s;
	int p;
	HBITMAP a[4];
	int life;
	struct  entity* next;
	unsigned short f;
}Entity;
void DrawEntities(HDC hdc, const Entity* head);
void DrawEntity(HDC hdc, const Entity* ent);
void MoveEntities(Entity* head, int ts);
void Move(Entity* ent, int ts);
void Fire(const Entity* ent, Entity* pheadb);
void Destroy(Entity* phead);
void Destroy(Entity* phead, Entity* ent);
int Iscollider(const Entity* ent1, const Entity* ent2);
int Wallcollider(Entity* ent, int w, int h);

#endif 