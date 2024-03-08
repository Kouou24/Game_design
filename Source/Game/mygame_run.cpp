#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>
#include "config.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	ifstream ifs("map/Random.map");

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			ifs >> map[i][j];
			if (map[i][j] == 1) {
				background_map[i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[i][j].SetTopLeft(MAP_SIZE*i+60, MAP_SIZE*j+60);
			}
			if (map[i][j] == 2) {
				background_map[i][j].LoadBitmapByString({ "resources/wall.bmp" });
				background_map[i][j].SetTopLeft(MAP_SIZE*i+60, MAP_SIZE*j+60);
			}
		}
	}
	ifs.close();

	character.LoadBitmapByString({ "resources/man.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(150, 265);

	box.LoadBitmapByString({ "resources/box.bmp" }, RGB(255, 255, 255));
	box.SetTopLeft(180, 265);

	fin.LoadBitmapByString({ "resources/fin.bmp", "resources/fin_ignore.bmp" }, RGB(255, 255, 255));
	fin.SetTopLeft(260, 265);

	chest_and_key.LoadBitmapByString({ "resources/chest.bmp", "resources/chest_ignore.bmp" }, RGB(255, 255, 255));
	chest_and_key.SetTopLeft(150, 430);

	bee.LoadBitmapByString({ "resources/bee_1.bmp", "resources/bee_2.bmp" });
	bee.SetTopLeft(462, 265);
	bee.SetAnimation(1000, false);

	ball.LoadBitmapByString({ "resources/ball-3.bmp", "resources/ball-3.bmp", "resources/ball-2.bmp", "resources/ball-1.bmp", "resources/ball-ok.bmp" });
	ball.SetTopLeft(150, 430);
	ball.SetAnimation(1000, true);
	ball.ToggleAnimation();


	for (int i = 0; i < 3; i++) {
		door[i].LoadBitmapByString({ "resources/door_close.bmp", "resources/door_open.bmp" }, RGB(255, 255, 255));
		door[i].SetTopLeft(462 - 100 * i, 265);
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (phase == 1) {
		if (sub_phase == 1) {
			sub_phase += validate_phase_1();
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
		}
	}
	else if (phase == 2) {
		if (sub_phase == 1) {
			sub_phase += validate_phase_2();
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
		}
	}
	else if (phase == 3) {
		if (sub_phase == 1) {
			sub_phase += validate_phase_3();
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
		}
	}
	else if (phase == 4) {
		if (sub_phase == 1) {
			sub_phase += validate_phase_4();
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
		}
	}
	else if (phase == 5) {
		if (sub_phase == 1) {
			sub_phase += validate_phase_5();
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
		}
	}
	else if (phase == 6) {
		if (sub_phase == 1) {
			sub_phase += validate_phase_6();
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
			GotoGameState(GAME_STATE_OVER);
		}
	}
		if (nChar == VK_UP) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() - 30);
		}
		if (nChar == VK_DOWN) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + 30);

		}
		if (nChar == VK_RIGHT) {
			character.SetTopLeft(character.GetLeft() + 30, character.GetTop());

		}
		if (nChar == VK_LEFT) {
			character.SetTopLeft(character.GetLeft() - 30, character.GetTop());

		}
		if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_UP) {
			box.SetTopLeft(box.GetLeft(), box.GetTop() - 30);
		}
		if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_DOWN) {
			box.SetTopLeft(box.GetLeft(), box.GetTop() + 30);
		}
		if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_RIGHT) {
			box.SetTopLeft(box.GetLeft() + 30, box.GetTop());
		}
		if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_LEFT) {
			box.SetTopLeft(box.GetLeft() - 30, box.GetTop());
		}
		if (CMovingBitmap::IsOverlap(fin, box)) {
			fin.SetFrameIndexOfBitmap(1);
			win_flag = true;
		}

		

	}



void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	show_map();
	show_image_by_phase();
	show_text_by_phase();
}

void CGameStateRun::show_map() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (map[i][j] == 1) {
				background_map[i][j].ShowBitmap();
			}
		}
	}

}
void CGameStateRun::show_image_by_phase() {
	if (phase <= 6) {
		character.ShowBitmap();
		box.ShowBitmap();
		fin.ShowBitmap();
		if (phase == 3 && sub_phase == 1) {
			chest_and_key.ShowBitmap();
		}
		if (phase == 4 && sub_phase == 1) {
			bee.ShowBitmap();
		}
		if (phase == 5 && sub_phase == 1) {
			for (int i = 0; i < 3; i++) {
				door[i].ShowBitmap();
			}
		}
		if (phase == 6 && sub_phase == 1) {
			ball.ShowBitmap();
		}
	}
}

void CGameStateRun::show_text_by_phase() {
	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(0, 0, 0), 800);

	if (phase == 1 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 1 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 2 && sub_phase == 1) {
		CTextDraw::Print(pDC, 26, 128, "下一個階段，讓長頸鹿能夠透過上下左右移動到這個位置！");
		CTextDraw::Print(pDC, 373, 537, "按下 Enter 鍵來驗證");
	} else if (phase == 3 && sub_phase == 1) {
		CTextDraw::Print(pDC, 205, 128, "幫你準備了一個寶箱");
		CTextDraw::Print(pDC, 68, 162, "設計程式讓長頸鹿摸到寶箱後，將寶箱消失！");
		CTextDraw::Print(pDC, 68, 196, "記得寶箱要去背，使用 RGB(255, 255, 255)");
		CTextDraw::Print(pDC, 373, 537, "按下 Enter 鍵來驗證");
	} else if (phase == 4 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "幫你準備了一個蜜蜂好朋友");
		CTextDraw::Print(pDC, 89, 162, "已經幫它做了兩幀的動畫，讓它可以上下移動");
		CTextDraw::Print(pDC, 110, 196, "寫個程式來讓你的蜜蜂好朋友擁有動畫！");
		CTextDraw::Print(pDC, 373, 537, "按下 Enter 鍵來驗證");
	} else if (phase == 5 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "幫你準備了三扇門");
		CTextDraw::Print(pDC, 89, 162, "設計程式讓長頸鹿摸到門之後，門會打開");
		CTextDraw::Print(pDC, 373, 537, "按下 Enter 鍵來驗證");
	} else if (phase == 6 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "幫你準備了一顆會倒數的球");
		CTextDraw::Print(pDC, 89, 162, "設計程式讓球倒數，然後顯示 OK 後停止動畫");
		CTextDraw::Print(pDC, 373, 537, "按下 Enter 鍵來驗證");
	} else if (sub_phase == 2) {
		CTextDraw::Print(pDC, 268, 128, "完成！");
	}

	CDDraw::ReleaseBackCDC();
}

bool CGameStateRun::validate_phase_1() {
	return win_flag;
}

bool CGameStateRun::validate_phase_2() {
	return character.GetTop() > 204 && character.GetTop() < 325 && character.GetLeft() > 339 && character.GetLeft() < 459;
}

bool CGameStateRun::validate_phase_3() {
	return (
		character.GetTop() + character.GetHeight() >= chest_and_key.GetTop()
		&& character.GetLeft() + character.GetWidth() >= chest_and_key.GetLeft()
		&& chest_and_key.GetFrameIndexOfBitmap() == 1
		&& chest_and_key.GetFilterColor() == RGB(255, 255, 255)
	);
}

bool CGameStateRun::validate_phase_4() {
	return bee.IsAnimation() && bee.GetFrameSizeOfBitmap() == 2;
}

bool CGameStateRun::validate_phase_5() {
	bool check_all_door_is_open = true;
	for (int i = 0; i < 3; i++) {
		check_all_door_is_open &= (door[i].GetFrameIndexOfBitmap() == 1);
	}
	return check_all_door_is_open;
}

bool CGameStateRun::validate_phase_6() {
	return ball.IsAnimationDone() && !ball.IsAnimation() && ball.GetFrameIndexOfBitmap() == ball.GetFrameSizeOfBitmap() - 1;
}