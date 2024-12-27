#include "GameProc.h"
#include "Init.h"

int nscore = 0;
int bfire = 0;
int bput = 0;
int wndwidth = 1264, wndheight = 732;
LEVEL curlevel = OPEN;
Entity* pheadblocks = NULL;
Entity* pheadenemys = NULL;
Entity* player = NULL;
Entity* pheadbullets = NULL;
Entity* penemybullets = NULL;
Entity* landmine=NULL;
extern int resettime;

void Enemybirth() {
	if (curlevel != LEVEL1 && curlevel != LEVEL2)
		return;
	int type = 0;
	switch (curlevel) {
	case LEVEL1:
		type = (rand() % 3) == 0 ? 1 : 0;
		break;
	case LEVEL2:
		type = (rand() % 3) == 0 ? 0 : 1;
	}
	Enemybirth(pheadenemys, type, wndwidth);
}

void Enterkey(int key) {
	switch (curlevel) {
	case OPEN:
		if (key == VK_RETURN)
			Changelevel(LEVEL1);
		break;
	case LEVEL2_OPEN:
		if (key == VK_RETURN)
			Changelevel(LEVEL2);
		break;

	case FAIL:
	case SUCCEED:
		if (key == VK_RETURN)
			Changelevel(OPEN);
		break;
	case LEVEL1:
	case LEVEL2:
		switch (key) {
		case VK_CONTROL:
			bput=1;
			break;
		case VK_LEFT:
			player->dir = LEFT;
			player->p = 0;
			break;
		case VK_RIGHT:
			player->dir = RIGHT;
			player->p = 0;
			break;
		case VK_DOWN:
			player->dir = DOWN;
			player->p = 0;
			break;
		case VK_UP:
			player->dir = UP;
			player->p = 0;
			break;
		case VK_SPACE:
			bfire = 1;
			break;
		}
	}
}

void Newentity() {
	if (pheadblocks == NULL) {
		pheadblocks = (Entity*)malloc(sizeof(Entity));
		pheadblocks->next = NULL;
	}
	if (pheadenemys == NULL) {
		pheadenemys = (Entity*)malloc(sizeof(Entity));
		pheadenemys->next = NULL;
	}
	if (player == NULL) {
		player = (Entity*)malloc(sizeof(Entity));
		player->next = NULL;
	}
	if (pheadbullets == NULL) {
		pheadbullets = (Entity*)malloc(sizeof(Entity));
		pheadbullets->next = NULL;
	}
	if (penemybullets == NULL) {
		penemybullets = (Entity*)malloc(sizeof(Entity));
		penemybullets->next = NULL;
	}
	if (landmine == NULL) {
		landmine = (Entity*)malloc(sizeof(Entity));
		landmine->next = NULL;
	}
}

void Changewndsize(int w, int n) {
	wndwidth = w, wndheight = n;
}

void Changelevel(LEVEL newl) {
	curlevel = newl;
	Init(newl);
	if (newl != LEVEL1 && newl != LEVEL2)
		return;
	landmine = NULL;
	Newentity();
	InitEntity(pheadenemys, pheadblocks, pheadbullets, penemybullets, player, landmine, newl);
	Resetplayer(player);
	nscore = 0;
	resettime = 1;
}

void Resetplayer(Entity* player) {
	player->p = 1;
	player->dir = UP;
	player->x = wndwidth / 2;
	player->y = wndheight - player->s;
}

void Draw(HDC hdc, HWND hWnd) {
	HDC memhdc = CreateCompatibleDC(hdc);
	RECT rectclient;
	GetClientRect(hWnd, &rectclient);
	HBITMAP bmpBuff = CreateCompatibleBitmap(hdc, wndwidth, wndheight);
	HBITMAP poldBMP = (HBITMAP)SelectObject(memhdc, bmpBuff);
	PatBlt(memhdc, 0, 0, wndwidth, wndheight, WHITENESS);

	Drawscene(memhdc, curlevel);

	BOOL tt = BitBlt(hdc, rectclient.left, rectclient.top, wndwidth, wndheight, memhdc, rectclient.left, rectclient.top, SRCCOPY);
	SelectObject(memhdc, poldBMP);
	DeleteDC(memhdc);
}

void Drawscene(HDC hdc, LEVEL curl) {
	HFONT hf;
	WCHAR str[32];
	SetBkMode(hdc, TRANSPARENT);
	long lfheight;
	lfheight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hfold;
	hf = CreateFont(lfheight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	hfold = (HFONT)SelectObject(hdc, hf);
	SetTextColor(hdc, RGB(122, 0, 0));
	HBITMAP cd = Getlogopic();
	switch (curl)
	{
	case OPEN:
		cd = Getlogopic();
		wsprintf(str, L"%s", L"按下回车键开始游戏");
		break;
	case LEVEL1:
	case LEVEL2:
		SetTextColor(hdc, RGB(100, 0, 0));
		lfheight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		DeleteObject(hf);
		hf = CreateFont(lfheight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		SelectObject(hdc, hf);
		wsprintf(str, L"life:%d score:%d", player->life, nscore);
		Drawgamescene(hdc);
		break;
	case LEVEL2_OPEN:
		cd = Getsuccpic();
		wsprintf(str, L"%s", L"按下回车键开始下一关");
		break;
	case SUCCEED:
		cd = Getsuccpic();
		wsprintf(str, L"%s", L"胜利！按下回车键重新开始游戏");
		break;
     case FAIL:
		cd =Getfailpic();
		wsprintf(str, L"%s", L"失败！按下回车键重新开始游戏");
		break;
	}
	if (curl == OPEN || curl == LEVEL2_OPEN || curl == FAIL || curl == SUCCEED) {
		HDC hdcmem = CreateCompatibleDC(hdc);
		HBITMAP hbmold = (HBITMAP)SelectObject(hdcmem, cd);
		BITMAP bm;
		GetObject(cd, sizeof(bm), &bm);
		SetStretchBltMode(hdc, STRETCH_HALFTONE);
		BitBlt(hdc, wndwidth / 2 - bm.bmWidth / 2, wndheight / 2 - bm.bmHeight / 2, wndwidth, wndheight, hdcmem, 0, 0, SRCCOPY);
		SelectObject(hdcmem, hbmold);
		DeleteDC(hdcmem);
	}
	TextOut(hdc, wndwidth / 2 - 100, wndheight - 100,str,wcslen(str));
	SelectObject(hdc, hfold);
	DeleteObject(hf);
}

void Drawgamescene(HDC hdc) {
	DrawEntities(hdc, pheadenemys);
	DrawEntities(hdc, pheadbullets);
	DrawEntities(hdc, penemybullets);
	DrawEntities(hdc, pheadblocks);
	DrawEntities(hdc, landmine);
	DrawEntity(hdc,player);
	
}

void Destroy() {

	Destroy(pheadblocks);
	Destroy(pheadenemys);
	Destroy(pheadbullets);
	Destroy(penemybullets);
	Destroy(landmine);
	free(pheadblocks);
	free(landmine);
	free(pheadenemys);
	free(pheadbullets);
	free(penemybullets);

	pheadblocks = NULL;
	pheadenemys = NULL;
	player = NULL;
	pheadbullets = NULL;
	penemybullets = NULL;
	landmine = NULL;

}

void Update(int ts) {
	if (curlevel != LEVEL1 && curlevel != LEVEL2)
		return;
	MoveEntities(pheadenemys, ts);
	MoveEntities(pheadbullets, ts);
	MoveEntities(penemybullets, ts);
	Move(player, ts);
	if (bfire) {
		Fire(player, pheadbullets);
		bfire = 0;
	}
	Entity* freep = NULL;
	if (bput) {
		putmine(landmine, player->x, player->y);
		bput = 0;
	}
	for (Entity* ip = pheadbullets->next; ip != NULL; ip = ip->next) {
		if (freep != NULL) {
			Destroy(pheadbullets, freep);
			freep = NULL;
		}
		for (Entity* jp = pheadenemys->next; jp != NULL; jp = jp->next) {
			if (Iscollider(ip, jp)) {
				freep = ip;
				jp->life --;
				if (jp->life <= 0) {
					Destroy(pheadenemys, jp);
					nscore++;
				}
				break;
			}
		}
	}
	if (freep != NULL) {
		Destroy(pheadbullets, freep);
		freep = NULL;
	}

	for (Entity* ip = landmine->next; ip != NULL; ip = ip->next) {
		if (freep != NULL) {
			Destroy(landmine, freep);
			freep = NULL;
		}
		for (Entity* jp = pheadenemys->next; jp != NULL; jp = jp->next) {
			if (Iscollider(ip, jp)) {
				freep = ip;
				Destroy(pheadenemys, jp);
				nscore++;
				break;
			}
		}
	}
	if (freep != NULL) {
		Destroy(landmine, freep);
		freep = NULL;
	}


	for (Entity* p = penemybullets->next; p != NULL; p = p->next) {
		if (Iscollider(p, player)) {
			Destroy(penemybullets, p);
			Resetplayer(player);
			player->life--;
		}
	}

	for (Entity* p = pheadenemys->next; p != NULL; p = p->next) {
		if (Randomfire())
			Fire(p, penemybullets);
		if (Iscollider(player, p)) {
			Resetplayer(player);
			player->life--;
		}
	}

	for (Entity* p = pheadenemys->next; p != NULL; p = p->next) {
		if (!Wallcollider(p, wndwidth, wndheight)) {
			int cg = 0;
			for (Entity* q = pheadblocks->next; q != NULL; q = q->next) {
				if (Iscollider(p, q)) {
					cg = 1;
					Move(p, -ts);
					break;
				}
			}
			if (Randomdir() || cg)
				p->dir = Dir((p->dir + 1 + rand() % 3) % 4);
		}
		else
			p->dir = Dir((p->dir + 1 + rand() % 3) % 4);
	}

	for (Entity* p = pheadblocks->next; p != NULL; p = p->next) {
		if (Iscollider(p, player)) {
			switch (player->dir)
			{
			case UP:
				player->y = p->y + (p->s + player->s) / 2;
				break;
			case DOWN:
				player->y = p->y - (p->s + player->s) / 2;
				break;
			case LEFT:
				player->x = p->x + (p->s + player->s) / 2;
				break;
			case RIGHT:
				player->x = p->x - (p->s + player->s) / 2;
				break;
			}
			player->p = 1;
			break;
		}
	}

	for (Entity* p = pheadbullets->next; p != NULL; p = p->next) {
		if (freep != NULL) {
			Destroy(pheadbullets, freep);
			freep = NULL;
		}
		if (Wallcollider(p, wndwidth, wndheight)) {

			freep = p;
		}
	}
	if (freep != NULL) {
		Destroy(pheadbullets, freep);
		freep = NULL;
	}

	for (Entity* p = penemybullets->next; p != NULL; p = p->next) {
		if (freep != NULL) {
			Destroy(penemybullets, freep);
			freep = NULL;
		}
		if (Wallcollider(p, wndwidth, wndheight))
			freep = p;
	}
	if (freep != NULL) {
		Destroy(penemybullets, freep);
		freep = NULL;
	}

	if (Wallcollider(player, wndwidth, wndheight))
		player->p = 1;

	if (player->life <= 0) 
		Changelevel(FAIL);
	else if (nscore >= Getpscore()) {
		switch (curlevel)
		{
		case LEVEL1:
			Changelevel(LEVEL2_OPEN);
			break;
		case LEVEL2:
			Changelevel(SUCCEED);
			break;
		}
	}
}