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
	CAudio::Instance()->Play(3, true);
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	CAudio::Instance()->Load(1, "music/push_sound.mp3");
	CAudio::Instance()->Load(2, "music/win_sound.mp3");
	CAudio::Instance()->Load(3, "music/game.mp3");


	background.LoadBitmapByString({
		"resources/background_all.bmp",
		
		});
	background.SetTopLeft(0, 0);

	load.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/loading.bmp",
		}, RGB(255, 255, 255));
	load.SetTopLeft(0, 0);

	dead.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/dead.bmp",
		}, RGB(255, 255, 255));
	dead.SetTopLeft(0, 0);


	load_background();


}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (nChar == 0x52){
		while_load = false;
		load.SetFrameIndexOfBitmap(0);
		reset_phase(phase-1); //關卡減1
	}

	if (nChar == 0x51) {
		win_flag = true;
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
	
	if (bomb.size() > 0) {
		for (int i = 0; i < int(bomb.size()); i++) {
			if (CMovingBitmap::IsOverlap(character, bomb[i])) {
				dead.SetFrameIndexOfBitmap(1);
			}
		}
	}

	int gg = int(fin.size())  , k = 0;
	for (int j = 0; j < int(box.size()); j++) {

		if (CMovingBitmap::IsOverlap(character, box[j]) && nChar == VK_UP) {
			CAudio::Instance()->Play(1, false);
			lastbox_x[j] = box[j].GetLeft(), lastbox_y[j] = box[j].GetTop();
			box[j].SetTopLeft(box[j].GetLeft(), box[j].GetTop() - MAP_SIZE);
		}
		if (CMovingBitmap::IsOverlap(character, box[j]) && nChar == VK_DOWN) {
			CAudio::Instance()->Play(1, false);
			lastbox_x[j] = box[j].GetLeft(), lastbox_y[j] = box[j].GetTop();
			box[j].SetTopLeft(box[j].GetLeft(), box[j].GetTop() + MAP_SIZE);
		}
		if (CMovingBitmap::IsOverlap(character, box[j]) && nChar == VK_RIGHT) {
			CAudio::Instance()->Play(1, false);
			lastbox_x[j] = box[j].GetLeft(), lastbox_y[j] = box[j].GetTop();
			box[j].SetTopLeft(box[j].GetLeft() + MAP_SIZE, box[j].GetTop());
		}
		if (CMovingBitmap::IsOverlap(character, box[j]) && nChar == VK_LEFT) {
			CAudio::Instance()->Play(1, false);
			lastbox_x[j] = box[j].GetLeft(), lastbox_y[j] = box[j].GetTop();
			box[j].SetTopLeft(box[j].GetLeft() - MAP_SIZE, box[j].GetTop());
		}
		

		for (int i = 0; i<int(box.size()); i++) {

			
				if (i == j) continue;
				if (CMovingBitmap::IsOverlap(box[j], box[i])) {
					character.SetTopLeft(last_x, last_y);
					last_x = character.GetLeft();
					last_y = character.GetTop();

					box[j].SetTopLeft(lastbox_x[j], lastbox_y[j]);
					lastbox_x[j] = box[j].GetLeft();
					lastbox_y[j] = box[j].GetTop();

					box[i].SetTopLeft(lastbox_x[i], lastbox_y[i]);
					lastbox_x[i] = box[i].GetLeft();
					lastbox_y[i] = box[i].GetTop();
				}
			
		}


		if (bomb.size() > 0) {
			for (int i = 0; i < int(bomb.size()); i++) {
				if (CMovingBitmap::IsOverlap(box[j], bomb[i])) {

					character.SetTopLeft(last_x, last_y);
					last_x = character.GetLeft();
					last_y = character.GetTop();

					box[j].SetTopLeft(lastbox_x[j], lastbox_y[j]);
					lastbox_x[j] = box[j].GetLeft();
					lastbox_y[j] = box[j].GetTop();

				}
			}
		}

		for (int i = 0; i < int(fin.size()); i++) {

			if ((CMovingBitmap::IsOverlap(fin[i], box[j]))) {
				k++;
			}
			if (k == gg) {
				CAudio::Instance()->Play(2, false);
				load.SetFrameIndexOfBitmap(1);
				while_load = true;
				if (nChar == 0x4A) {
					win_flag = true;
					while_load = false;
					load.SetFrameIndexOfBitmap(0);
				}
				//fin[i].SetFrameIndexOfBitmap(1);
				//win_flag = true;
			}
			
		}
	}

	for (int i = 0; i<int(wall.size()); i++) {
		if (CMovingBitmap::IsOverlap(character, wall[i])) {
			character.SetTopLeft(last_x, last_y);
			last_x = character.GetLeft();
			last_y = character.GetTop();
		}
		for (int j = 0; j < int(box.size()); j++) {

			if (CMovingBitmap::IsOverlap(box[j], wall[i])) {
				box[j].SetTopLeft(lastbox_x[j], lastbox_y[j]);
				lastbox_x[j] = box[j].GetLeft();
				lastbox_y[j] = box[j].GetTop();

				character.SetTopLeft(last_x, last_y);
				last_x = character.GetLeft();
				last_y = character.GetTop();
			}
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
		for (int i = 0; i < int(fin.size()); i++) {
				fin[i].ShowBitmap();
			}
		if (bomb.size()>0) {
			for (int i = 0; i < int(bomb.size()) ; i++) {
				bomb[i].ShowBitmap();
			}
			
		}
		character.ShowBitmap();
		for (int i = 0; i < int(box.size()); i++) {
			box[i].ShowBitmap();
		}
		dead.ShowBitmap();
		load.ShowBitmap();
	}
}


void game_framework::CGameStateRun::load_background()
{
	ifstream ifs("map/Random.map");

	int i_max, j_max,c;
	for (int k = 0; k < 6; k++) {  //初始化 地圖數量=k
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


void CGameStateRun::show_text_by_phase() {
	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(0, 0, 0), 800);
	if (while_load) {
		CTextDraw::Print(pDC, 50, 20, "");
	}
	else if (phase == 1 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 1 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 2 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 2 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 3 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 3 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 4 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 4 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 5 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 5 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} else if (phase == 6 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 20, "關卡 : 6 / 20");
		CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
	} 

	CDDraw::ReleaseBackCDC();
}






void CGameStateRun::reset_phase(int phase_chose) {
	wall.clear();
	bomb.clear();
	box.clear();
	fin.clear();
	lastbox_y.clear();
	lastbox_x.clear();
	dead.SetFrameIndexOfBitmap(0);
	int k = phase_chose;
	for (size_t i = 0; i < map[k].size() ; i++) {
		for (size_t j = 0; j < map[k][i].size() ; j++) {
			if (map[k][i][j] == 1) {
				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);
			}
			if (map[k][i][j] == 2) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);

				wall.push_back(CMovingBitmap());
				wall[wall.size() - 1].LoadBitmapByString({ "resources/wall.bmp" });
				wall[wall.size() - 1].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);
			}
			if (map[k][i][j] == 3) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);
				
				box.push_back(CMovingBitmap());
				box[box.size() - 1].LoadBitmapByString({ "resources/box.bmp" }, RGB(255, 255, 255));
				box[box.size() - 1].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);
				lastbox_x.push_back(MAP_SIZE*j + 150);
				lastbox_y.push_back(MAP_SIZE*i + 150);
			}
			if (map[k][i][j] == 4) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);

				character.LoadBitmapByString({ "resources/man.bmp" }, RGB(255, 255, 255));
				character.SetTopLeft(150 + MAP_SIZE * j, 150 + MAP_SIZE * i);

			}
			if (map[k][i][j] == 5) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);


				fin.push_back(CMovingBitmap());
				fin[fin.size() - 1].LoadBitmapByString({ "resources/fin.bmp", "resources/fin_ignore.bmp" }, RGB(255, 255, 255));
				fin[fin.size() - 1].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);
				fin[fin.size() - 1].SetFrameIndexOfBitmap(0);


			}
			if (map[k][i][j] == 6) {

				background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
				background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);

				bomb.push_back(CMovingBitmap());
				bomb[bomb.size() - 1].LoadBitmapByString({ "resources/bomb.bmp", "resources/bomb_ignore.bmp" }, RGB(255, 255, 255));
				bomb[bomb.size() - 1].SetTopLeft(MAP_SIZE*j + 150, MAP_SIZE*i + 150);
				bomb[bomb.size() - 1].SetFrameIndexOfBitmap(0);


			}
		}
	}
}


void CGameStateRun::phase_control() {

	if (phase == 1) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			phase += 1;
			win_flag = 0;

			reset_phase(1);
		}
	}
	else if (phase == 2) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			win_flag = 0;
			phase += 1;

			reset_phase(2);
		}
	}
	else if (phase == 3) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			win_flag = 0;
			phase += 1;

			reset_phase(3);
		}
	}
	else if (phase == 4) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			win_flag = 0;
			phase += 1;

			reset_phase(4);
		}
	}
	else if (phase == 5) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			win_flag = 0;
			phase += 1;

			reset_phase(5);
		}
	}
	else if (phase == 6) {
		if (sub_phase == 1) {
			sub_phase += win_flag;
		}
		else if (sub_phase == 2) {
			sub_phase = 1;
			win_flag = 0;
			phase += 1;

			GotoGameState(GAME_STATE_OVER);
		}
	}
}


void CGameStateRun::show_map() {
	int k = phase - 1;
	for (size_t i = 0; i < map[k].size(); i++) {
		for (size_t j = 0; j < map[k][i].size(); j++) {

			background_map[k][i][j].ShowBitmap();

		}
	}
	for (int i = 0; i < int(wall.size()); i++) {
		wall[i].ShowBitmap();
	}
}