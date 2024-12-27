#include "Entity.h"
void DrawEntities(HDC hdc, const Entity* head) {
	Entity* ent = head->next;
	while (ent != NULL) {
		DrawEntity(hdc, ent);
		ent = ent->next;
	}
}

void DrawEntity(HDC hdc, const Entity* ent) {
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBITMAP bmp = ent->a[int(ent->dir) % 4];
	HBITMAP hbmold = (HBITMAP)SelectObject(hdcmem, bmp);
	BITMAP bm;
	GetObject(bmp, sizeof(bm), &bm);
	SetStretchBltMode(hdc, STRETCH_HALFTONE);
	TransparentBlt(hdc, ent->x - ent->s / 2, ent->y - ent->s / 2, ent->s,ent->s, hdcmem,0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
	SelectObject(hdcmem, hbmold);
	DeleteDC(hdcmem);
}

void MoveEntities(Entity* head, int ts) {
	if (head == NULL)
		return;
	Entity* ent = head->next;
	while (ent != NULL) {
		Move(ent, ts);
		ent = ent->next;
	}
}

void Move(Entity* ent, int ts) {
	if (ent->p)
		return;
	switch (ent->dir) {
	case UP:
		ent->y -= ent->v * ts;
		break;
	case DOWN:
		ent->y += ent->v * ts;
		break;
	case RIGHT:
		ent->x += ent->v * ts;
		break;
	case LEFT:
		ent->x -= ent->v * ts;
		break;
	}
}

void Fire(const Entity* ent, Entity* pheadb) {
	Entity* newbullet = (Entity*)malloc(sizeof(Entity));
	memcpy(newbullet, pheadb, sizeof(Entity));
	newbullet->dir = ent->dir;
	newbullet->x = ent->x;
	newbullet->y = ent->y;
	switch (ent->dir) {
	case UP:
		newbullet->y -= ent->s;
		break;
	case DOWN:
		newbullet->y += ent->s;
		break;
	case RIGHT:
		newbullet->x += ent->s;
		break;
	case LEFT:
		newbullet->x -= ent->s;
		break;
	}
	newbullet->next = pheadb->next;
	pheadb->next = newbullet;
}

void Destroy(Entity* phead)
{
	Entity* p = phead;
	Entity* pn = phead->next;
	while (pn != NULL) {
		p->next = pn->next;
		free(pn);
		pn = p->next;
	}
	phead->next = NULL;
}

void Destroy(Entity* phead, Entity* ent) {
	Entity* p = phead;
	Entity* pn = phead->next;
	while (pn != NULL) {
		if (pn == ent) {
			p->next = pn->next;
			free(pn);
			return;
		}
		p = pn;
		pn = pn->next;
	}
	//p = pn;
	//pn = pn->next;
}

int Iscollider(const Entity* ent1, const Entity* ent2) {
	if (ent1 == NULL || ent2 == NULL)
		return 0;
	if (((ent1->x + ent1->s / 2) <= (ent2->x - ent2->s / 2)) || ((ent1->x - ent1->s / 2) >= (ent2->x + ent2->s / 2)))
		return 0;
	if (((ent1->y + ent1->s / 2) <= (ent2->y - ent2->s / 2)) || ((ent1->y - ent1->s / 2) >= (ent2->y + ent2->s / 2)))
		return 0;
	return 1;
}

int Wallcollider(Entity* ent, int w, int h) {
	int bc = 0;
	switch (ent->dir) {
	case UP:
		if ((ent->y - ent->s / 2) < 0) {
			bc = 1;
			ent->y = ent->s / 2;
		}
		break;
	case DOWN:
		if ((ent->y + ent->s / 2) > h) {
			bc = 1;
			ent->y = h-ent->s / 2;
		}
		break;
	
	case LEFT:
		if ((ent->x - ent->s / 2) < 0) {
			bc = 1;
			ent->x = ent->s / 2;
		}
		break;
	case RIGHT:
		if ((ent->x + ent->s / 2) > w) {
			bc = 1;
			ent->x = w-ent->s / 2;
		}
		break;
	}
	return bc;
}