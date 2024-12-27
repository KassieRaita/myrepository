#include "Init.h"
#include <stdio.h>
const WCHAR* fileini = L"resource\\Init.ini";
HBITMAP tank1[4], tank2[4], tank3[4], bulletpic[2], block = NULL,mine;
HBITMAP logo = NULL, succpic = NULL, failpic = NULL;
int timestep = 20;
int nlife = 3;
int enemyfireper = 300;
int enemydirper = 200;
int birthtime = 5000;
int npscore = 10;
extern HINSTANCE hInst;

HBITMAP Getlogopic() {
	return logo;
}

HBITMAP Getsuccpic() {
	return succpic;
}

HBITMAP Getfailpic() {
	return failpic;
}

int Gettimestep() {
	return timestep;
}

int Getbirthtime() {
	return birthtime;
}

int Getpscore() {
	return npscore;
}

int Randomfire() {
	return((rand() % enemyfireper) == 0);
}

int Randomdir() {
	return(rand() % enemydirper == 0);
}

void Enemybirth(Entity* headenemy, int type, int w) {
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	memcpy(ent, headenemy, sizeof(Entity));
	ent->next = headenemy->next;
	headenemy->next = ent;
	ent->dir = Dir(rand() % 4);
	ent->x = (rand() % 3) * (w - ent->s) / 2 + ent->s / 2;
	ent->y = ent->s;
	switch (type) {
	case 0:
		ent->v = GetPrivateProfileInt(L"Enemy", L"vels", 2, fileini);
		memcpy(ent->a, tank2, 4 * sizeof(HBITMAP));
		ent->life = 1;
		break;
	case 1:
		ent->v = GetPrivateProfileInt(L"Enemy", L"velf", 4, fileini);
		memcpy(ent->a, tank3, 4 * sizeof(HBITMAP));
		ent->life = 2;
		break;
	}
}

void putmine(Entity* mi,int x1,int y1) {
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	memcpy(ent,mi , sizeof(Entity));
	ent->x = x1;
	ent->y = y1;
	ent->next = mi->next;
	mi->next = ent;
}

void Readini(LEVEL l) {
	switch (l)
	{
	case OPEN:
		fileini = L"resource\\init.ini";
		break;
	case LEVEL1:
		fileini = L"resource\\init.ini";
		break;
	case LEVEL2_OPEN:
		fileini = L"resource\\init2.ini";
		break;
	case LEVEL2:
		fileini = L"resource\\init2.ini";
		break;
	default:
		fileini = L"resource\\init.ini";
		break;
	}
	timestep = GetPrivateProfileInt(L"Global", L"timestep", timestep, fileini);
	npscore = GetPrivateProfileInt(L"Global", L"npscore", npscore, fileini);
	enemyfireper = GetPrivateProfileInt(L"Global", L"enemyfireper", enemyfireper, fileini);
	enemydirper= GetPrivateProfileInt(L"Global", L"enemydirper", enemydirper, fileini);
	birthtime= GetPrivateProfileInt(L"Global", L"birthtime", birthtime, fileini);
	nlife=GetPrivateProfileInt(L"Global", L"nlife", nlife, fileini);
}

void Initmap(LEVEL l, Entity* headblock) {
	const char* filename = "resource\\map.txt";
	switch (l) {
	case LEVEL1:
		filename = "resource\\map.txt";
		break;
	case LEVEL2:
		filename = "resource\\map2.txt";
		break;
	}
	FILE* f;
	fopen_s(&f, filename, "r");
	if (NULL == f)
		return;
	char line[64];
	int nline = 0;
	Entity* newblock = NULL;
	while (!feof(f)) {
		fgets(line, 64, f);
		for (int i = 0; line[i] != '\0'; i++) {
			if (line[i] == '1') {
				newblock = (Entity*)malloc(sizeof(Entity));
				memcpy(newblock, headblock, sizeof(Entity));
				newblock->x = newblock->s / 2 + newblock->s * i;
				newblock->y = newblock->s / 2 + newblock->s * nline;
				newblock->next = headblock->next;
				headblock->next = newblock;
			}
		}
		nline++;
	}
	fclose(f);
}

void Init(LEVEL l) {
	switch (l) {

	case OPEN:
		logo = (logo == NULL) ? (HBITMAP)LoadImage(NULL, L"resource\\logo.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) : logo;
		break;
	case LEVEL2_OPEN:
	case SUCCEED:
		succpic= (succpic == NULL) ? (HBITMAP)LoadImage(NULL, L"resource\\winpic.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) : succpic;
		break;
	case FAIL:
		failpic= (failpic == NULL) ? (HBITMAP)LoadImage(NULL, L"resource\\failpic.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) : failpic;
	}
	if (l != LEVEL1 && l != LEVEL2)
		return;
	Readini(l);
	tank1[0] = (HBITMAP)LoadImage(NULL, L"resource\\mytank-up.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank2[0] = (HBITMAP)LoadImage(NULL, L"resource\\en1-up.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank3[0] = (HBITMAP)LoadImage(NULL, L"resource\\en2-up.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank1[1] = (HBITMAP)LoadImage(NULL, L"resource\\mytank-down.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank2[1] = (HBITMAP)LoadImage(NULL, L"resource\\en1-down.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank3[1] = (HBITMAP)LoadImage(NULL, L"resource\\en2-down.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank1[2] = (HBITMAP)LoadImage(NULL, L"resource\\mytank-right.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank2[2] = (HBITMAP)LoadImage(NULL, L"resource\\en1-right.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank3[2] = (HBITMAP)LoadImage(NULL, L"resource\\en2-right.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank1[3] = (HBITMAP)LoadImage(NULL, L"resource\\mytank-left.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank2[3] = (HBITMAP)LoadImage(NULL, L"resource\\en1-left.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	tank3[3] = (HBITMAP)LoadImage(NULL, L"resource\\en2-left.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	bulletpic[0] = (HBITMAP)LoadImage(NULL, L"resource\\mybullet.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	bulletpic[1] = (HBITMAP)LoadImage(NULL, L"resource\\enbullet.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//background = (HBITMAP)LoadImage(NULL, L"resources\\floor.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//tree = (HBITMAP)LoadImage(NULL, L"resources\\tree.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	mine = (HBITMAP)LoadImage(NULL, L"resource\\bomb.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	block = (HBITMAP)LoadImage(NULL, L"resource\\hole.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
}

void InitEntity(Entity* headenemy, Entity* headblock, Entity* headbullet, Entity* headenemybullet, Entity* player,Entity*landmine, LEVEL l) {
	Destroy(headenemy);
	Destroy(headblock);
	Destroy(headbullet);
	Destroy(headenemybullet);
	
	headenemy->s = GetPrivateProfileInt(L"Enemy", L"size", 50, fileini);
	headenemy->dir = Dir(UP + rand() % 4);
	headenemy->v = rand() % 2 == 0 ? GetPrivateProfileInt(L"Enemy", L"velf", 4, fileini) : GetPrivateProfileInt(L"Enemy", L"vels", 4, fileini);
	memcpy(headenemy->a, tank2, 4 * sizeof(HBITMAP));
	headenemy->x = 0;
	headenemy->y = 0;
	headenemy->p = 0;
	headenemy->life = 1;
	headenemy->f = 0;
	
	headblock->s = GetPrivateProfileInt(L"Block", L"size", 50, fileini);
	headblock->dir = NONE;
	headblock->v = 0;
	headblock->a[0] = headblock->a[1] = headblock->a[2] = headblock->a[3] = block;
	headblock->x = 0;
	headblock->y = 0;
	headblock->p = 0;
	headblock->life = 0;
	headblock->f = 0;
	Initmap(l, headblock);

	headbullet->s = GetPrivateProfileInt(L"Bullet", L"size", 50, fileini);
	headbullet->a[0] = headbullet->a[2] = headbullet->a[3] = headbullet->a[1] = bulletpic[0];
	//memcpy(headbullet->a, bulletpic, 2 * sizeof(HBITMAP));
	headbullet->dir = NONE;
	headbullet->v = GetPrivateProfileInt(L"Bullet", L"vel", 6, fileini);
	headbullet->x = 0;
	headbullet->y = 0;
	headbullet->p = 0;
	headbullet->life = 1;
	headbullet->f = 0;

	memcpy(headenemybullet, headbullet, sizeof(Entity));
	headenemybullet->a[0] = headenemybullet->a[2] = headenemybullet->a[3] = headenemybullet->a[1] = bulletpic[1];

	player->s = GetPrivateProfileInt(L"Player", L"size", 50, fileini);
	memcpy(player->a, tank1, 4 * sizeof(HBITMAP));
	player->dir = UP;
	player->v = GetPrivateProfileInt(L"Player", L"vel", 2, fileini);
	player->x = 0;
	player->y = 0;
	player->p = 1;
	player->life = nlife;
	player->f = 0;

	landmine->dir = NONE;
	landmine->x = 0;
	landmine->y = 0;
	landmine->a[0] = landmine->a[1] = landmine->a[2] = landmine->a[3] = mine;
	landmine->s = headbullet->s;

}