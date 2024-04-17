#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g) : CGameState(g)
{
}

void CGameStateOver::OnMove()
{

}

void CGameStateOver::OnBeginState()
{
}

void CGameStateOver::OnInit()
{
	giraffe.LoadBitmap({ "resources/giraffe.bmp", "resources/giraffe_left.bmp" }, RGB(255, 255, 255));
	giraffe.SetTopLeft(430, 451);
	giraffe.SetAnimation(100, false);

	man.LoadBitmap({ "resources/man.bmp", "resources/man_right.bmp" }, RGB(255, 255, 255));
	man.SetTopLeft(490, 481);
	man.SetAnimation(100, false);

	bee.LoadBitmap({ "resources/bee_1.bmp", "resources/bee_2.bmp" }, RGB(255, 255, 255));
	bee.SetTopLeft(350, 451);
	bee.SetAnimation(100, false);
	load_background();
}

void CGameStateOver::OnShow()
{
	background.ShowBitmap();
	giraffe.ShowBitmap();
	man.ShowBitmap();
	bee.ShowBitmap();

}

void CGameStateOver::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}


void CGameStateOver::load_background() {
	background.LoadBitmapByString({ "resources/over_background.bmp" });
	background.SetTopLeft(0, 0);
}