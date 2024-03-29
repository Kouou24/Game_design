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
	CAudio::Instance()->Load(1, "music/push_sound.mp3");
	CAudio::Instance()->Load(2, "music/win_sound.mp3");

	background.LoadBitmapByString({
		"resources/background_all.bmp",
		
		});
	background.SetTopLeft(0, 0);

	load_background();


}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (nChar == 0x52){
		reset_phase(phase);
	}
	if (nChar == VK_UP) {
		last_x = character.GetLeft(), last_y = character.GetTop();
		character.SetTopLeft(character.GetLeft(), character.GetTop() - MAP_SIZE);
	}
	if (nChar == VK_DOWN) {
		last_x = character.GetLeft(), last_y = character.GetTop();
		character.SetTopLeft(character.GetLeft(), character.GetTop() + MAP_SIZE);
	}
	if (nChar == VK_RIGHT) {
		last_x = character.GetLeft(), last_y = character.GetTop();
		character.SetTopLeft(character.GetLeft() + MAP_SIZE, character.GetTop());
	}
	if (nChar == VK_LEFT) {
		last_x = character.GetLeft(), last_y = character.GetTop();
		character.SetTopLeft(character.GetLeft() - MAP_SIZE, character.GetTop());
	}
	if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_UP) {
		CAudio::Instance()->Play(1, false);
		lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
		box.SetTopLeft(box.GetLeft(), box.GetTop() - MAP_SIZE);
	}
	if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_DOWN) {
		CAudio::Instance()->Play(1, false);
		lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
		box.SetTopLeft(box.GetLeft(), box.GetTop() + MAP_SIZE);
	}
	if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_RIGHT) {
		CAudio::Instance()->Play(1, false);
		lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
		box.SetTopLeft(box.GetLeft() + MAP_SIZE, box.GetTop());
	}
	if (CMovingBitmap::IsOverlap(character, box) && nChar == VK_LEFT) {
		CAudio::Instance()->Play(1, false);
		lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
		box.SetTopLeft(box.GetLeft() - MAP_SIZE, box.GetTop());
	}
	if (CMovingBitmap::IsOverlap(fin, box)) {
		CAudio::Instance()->Play(2, false);
		fin.SetFrameIndexOfBitmap(1);
		win_flag = true;
	}
	for (int i = 0; i<int(wall.size()); i++) {
		if (CMovingBitmap::IsOverlap(character, wall[i])) {
			character.SetTopLeft(last_x, last_y);
			last_x = character.GetLeft();
			last_y = character.GetTop();
		}
		if (CMovingBitmap::IsOverlap(box, wall[i])) {
			box.SetTopLeft(lastbox_x, lastbox_y);
			lastbox_x = box.GetLeft();
			lastbox_y = box.GetTop();
			character.SetTopLeft(last_x, last_y);
			last_x = character.GetLeft();
			last_y = character.GetTop();
		}

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
	phase_control();
	show_image_by_phase();
	
	show_text_by_phase();
}

void CGameStateRun::show_image_by_phase() {
	if (phase <= 6) {
		background.ShowBitmap();
		show_map();
		fin.ShowBitmap();
		character.ShowBitmap();
		box.ShowBitmap();

		if (phase == 3 && sub_phase == 1) {
		}
		if (phase == 4 && sub_phase == 1) {
		}
		if (phase == 5 && sub_phase == 1) {
		}
		if (phase == 6 && sub_phase == 1) {
		}
	}
}


void game_framework::CGameStateRun::load_background()
{
	ifstream ifs("map/Random.map");

	int i_max, j_max,c;
	for (int k = 0; k < 2; k++) {  //初始化 地圖數量=k
		ifs >> i_max;
		ifs >> j_max;
		map.push_back(vector<vector<int> >());
		background_map.push_back(vector<vector<CMovingBitmap> >());
		for (int i = 0; i < i_max; i++) {
			map[k].push_back(vector<int> ());
			background_map[k].push_back(vector<CMovingBitmap>());
			for (int j = 0; j < j_max; j++) {
				ifs >> c;
				map[k][i].push_back(c);
				background_map[k][i].push_back(CMovingBitmap());
			}
		}
	}
	ifs.close();

	reset_phase(0);
}

void CGameStateRun::show_map() {
	int k = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {

				background_map[k][i][j].ShowBitmap();
			
		}
	}
	for (int i = 0; i < int(wall.size()); i++) {
		wall[i].ShowBitmap();
	}
}

void CGameStateRun::show_text_by_phase() {
	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(0, 0, 0), 800);

	if (phase == 1 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 1 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 2 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 2 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 3 && sub_phase == 1) {
		CTextDraw::Print(pDC, 205, 128, "");
		CTextDraw::Print(pDC, 68, 162, "");
		CTextDraw::Print(pDC, 68, 196, "");
		CTextDraw::Print(pDC, 373, 537, "");
	} else if (phase == 4 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "");
		CTextDraw::Print(pDC, 89, 162, "");
		CTextDraw::Print(pDC, 110, 196, "");
		CTextDraw::Print(pDC, 373, 537, "");
	} else if (phase == 5 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "");
		CTextDraw::Print(pDC, 89, 162, "");
		CTextDraw::Print(pDC, 373, 537, "");
	} else if (phase == 6 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "");
		CTextDraw::Print(pDC, 89, 162, "");
		CTextDraw::Print(pDC, 373, 537, "");
	} 

	CDDraw::ReleaseBackCDC();
}


bool CGameStateRun::validate_phase_2() {
	return 0;
}

bool CGameStateRun::validate_phase_3() {
	return 0;
}

bool CGameStateRun::validate_phase_4() {
	return 0;
}

bool CGameStateRun::validate_phase_5() {

	return 0;
}

bool CGameStateRun::validate_phase_6() {
	return 0;
}



void CGameStateRun::phase_control() {

	if (phase == 1) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
			reset_phase(1);
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
}




void CGameStateRun::reset_phase(int phase_chose) {

	int k = phase_chose;
	for (size_t i = 0; i < map[k].size() ; i++) {
		for (size_t j = 0; j < map[k][i].size() ; j++) {
			if (map[k][i][j] == 1) {
				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*i + 150, MAP_SIZE*j + 150);
			}
			if (map[k][i][j] == 2) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*i + 150, MAP_SIZE*j + 150);

				wall.push_back(CMovingBitmap());
				wall[wall.size() - 1].LoadBitmapByString({ "resources/wall.bmp" });
				wall[wall.size() - 1].SetTopLeft(MAP_SIZE*i + 150, MAP_SIZE*j + 150);
			}
			if (map[k][i][j] == 3) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*i + 150, MAP_SIZE*j + 150);

				box.LoadBitmapByString({ "resources/box.bmp" }, RGB(255, 255, 255));
				box.SetTopLeft(150 + i * MAP_SIZE, 150 + j * MAP_SIZE);

			}
			if (map[k][i][j] == 4) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*i + 150, MAP_SIZE*j + 150);

				character.LoadBitmapByString({ "resources/man.bmp" }, RGB(255, 255, 255));
				character.SetTopLeft(150 + MAP_SIZE * i, 150 + MAP_SIZE * j);

			}
			if (map[k][i][j] == 5) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*i + 150, MAP_SIZE*j + 150);

				fin.LoadBitmapByString({ "resources/fin.bmp", "resources/fin_ignore.bmp" }, RGB(255, 255, 255));
				fin.SetTopLeft(150 + MAP_SIZE * i, 150 + MAP_SIZE * j);
				fin.SetFrameIndexOfBitmap(0);

			}
		}
	}
}

