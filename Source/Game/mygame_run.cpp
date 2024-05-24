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
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>



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
	//CAudio::Instance()->Play(3, true);
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{

	CAudio::Instance()->Load(1, "music/push_sound.mp3");
	CAudio::Instance()->Load(2, "music/win_sound.mp3");
	CAudio::Instance()->Load(3, "music/game.mp3");
	CAudio::Instance()->Load(4, "music/die_sound.mp3");


	background.LoadBitmapByString({
		"resources/background_all.bmp",

		});
	background.SetTopLeft(0, 0);

	load.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/loading.bmp",
		}, RGB(255, 255, 255));
	load.SetTopLeft(-10, 0);

	dead.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/die_pic.bmp",
		}, RGB(255, 255, 255));
	dead.SetTopLeft(-300, 200);


	map_menu.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/menu.bmp",

		}, RGB(255, 255, 255));
	map_menu.SetTopLeft(-10, -10);


	menu_box.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/select_box.bmp",

		}, RGB(255, 255, 255));
	menu_box.SetTopLeft(80, 86);


	map_menu.SetFrameIndexOfBitmap(1);
	menu_box.SetFrameIndexOfBitmap(1);

	scare.LoadBitmapByString({
		"resources/fin_ignore.bmp",
		"resources/scare.bmp",

		}, RGB(255, 255, 255));
	scare.SetTopLeft(0, 0);
	scare.SetFrameIndexOfBitmap(0);
	load_background();


}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (nChar == 0x52) { // 重置關卡 R
		CAudio::Instance()->Stop(4);
		while_load = false;
		while_dead = false;
		load.SetFrameIndexOfBitmap(0);
		scare.SetFrameIndexOfBitmap(0);
		reset_phase(phase - 1); //關卡減1=本關卡的陣列位址
	}

	if (nChar == 0x51) {//金手指 Q
		win_flag = true;
	}
	if (nChar == 0x4D) {// 去地圖選關 M

		while_menu = true;
		map_menu.SetFrameIndexOfBitmap(1);
		menu_box.SetFrameIndexOfBitmap(1);
		menu_box.SetTopLeft(80, 86);

	}
	if (while_dead)
	{

	}
	else if (while_load)
	{
		
	}
	else if (while_menu) {//選單介面 出現時
		if (nChar == VK_RETURN) {

			while_menu = false;
			map_menu.SetFrameIndexOfBitmap(0);
			menu_box.SetFrameIndexOfBitmap(0);

			phase = ((menu_box.GetLeft() - 80) / 88) + ((menu_box.GetTop() - 86) / 118 * 5) + 1;
			reset_phase(phase - 1);
			menu_box.SetTopLeft(0, 0);
		}

		if (nChar == VK_UP && menu_box.GetTop() > 86) {
			menu_box.SetTopLeft(menu_box.GetLeft(), menu_box.GetTop() - 118);
		}
		if (nChar == VK_DOWN && menu_box.GetTop() < 440) {
			menu_box.SetTopLeft(menu_box.GetLeft(), menu_box.GetTop() + 118);
		}
		if (nChar == VK_RIGHT && menu_box.GetLeft() < 346) {
			menu_box.SetTopLeft(menu_box.GetLeft() + 88, menu_box.GetTop());
		}
		if (nChar == VK_LEFT && menu_box.GetLeft() > 82) {
			menu_box.SetTopLeft(menu_box.GetLeft() - 88, menu_box.GetTop());
			
		}
	}

	else {
		character.moving(nChar);
	}



	if (bomb.size() > 0) {
		for (int i = 0; i < int(bomb.size()); i++) {
			if (character.is_bomb( bomb[i].pushout() )) {
				CAudio::Instance()->Play(4, false);
				while_dead = true;
				dead.SetFrameIndexOfBitmap(1);
			}
		}
	}

	int gg = int(fin.size()), k = 0;
	for (int j = 0; j < int(box.size()); j++) {

		if (character.is_pushbox( box[j].pushout()) ) {
			CAudio::Instance()->Play(1, false);
			box[j].box_pushed(nChar);
		}
		


		for (int i = 0; i<int(box.size()); i++) {


			if (i == j) continue;
			if (CMovingBitmap::IsOverlap(box[j].pushout(), box[i].pushout() )) {
				character.move_to_last();

				box[j].move_to_last();

				
			}

		}


		if (bomb.size() > 0) {
			for (int i = 0; i < int(bomb.size()); i++) {
				if (CMovingBitmap::IsOverlap(box[j].pushout(), bomb[i].pushout() )) {

					character.move_to_last();

					box[j].move_to_last();

				}
			}
		}

		for (int i = 0; i < int(fin.size()); i++) {

			if ((CMovingBitmap::IsOverlap(fin[i], box[j].pushout() ))) {
				k++;
			}
			if(while_load){
				if (nChar == 0x4A) {
					CAudio::Instance()->Stop(2);
					win_flag = true;
					while_load = false;
					load.SetFrameIndexOfBitmap(0);	
				}
				
			}
			else if (k == gg) {
				CAudio::Instance()->Play(2, false);
				load.SetFrameIndexOfBitmap(1);
				while_load = true;
			}

		}
	}

	for (int i = 0; i<int(wall.size()); i++) {
		if (character.is_wall( wall[i])) {
			character.move_to_last();
		}
		for (int j = 0; j < int(box.size()); j++) {
			
			if (CMovingBitmap::IsOverlap(box[j].pushout(), wall[i])) {
				box[j].move_to_last();
				character.move_to_last();
			}
		}
	}
	

	
}



void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	while (while_load)
	{
		if (point.x >= 110 && point.x <= 252 && point.y >= 345 && point.y <= 464)
		{
			CAudio::Instance()->Stop(2);
			win_flag = true;
			while_load = false;
			load.SetFrameIndexOfBitmap(0);
		}
		if (point.x >= 332 && point.x <= 485 && point.y >= 344 && point.y <= 473)
		{
			while_load = false;
			load.SetFrameIndexOfBitmap(0);
			reset_phase(phase - 1);
		}
	}
	if (while_menu) {
		while_menu = false;
		map_menu.SetFrameIndexOfBitmap(0);
		menu_box.SetFrameIndexOfBitmap(0);

		phase = ((menu_box.GetLeft() - 80) / 88) + ((menu_box.GetTop() - 86) / 118 * 5) + 1;
		reset_phase(phase - 1);
		menu_box.SetTopLeft(0, 0);
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	x_print = point.x;
	y_print = point.y;
	if (while_menu) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				if ((x_print >= (80 + (i * 88))) && (x_print <= (80 + (i + 1) * 88)) && (y_print >= (86 + (j * 118))) && (y_print <= (86 + ((j + 1) * 118)))) {
					menu_box.SetTopLeft((i * 88) + 80, 86 + (j * 118));
				}
			}
		}
	}



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
	//show_text_point(x_print, y_print);
}
void CGameStateRun::show_text_point(int x_print, int y_print) {
	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 255, 255), 800);
	CTextDraw::Print(pDC, 450, 50, to_string(x_print));
	CTextDraw::Print(pDC, 500, 50, to_string(y_print));
	CDDraw::ReleaseBackCDC();

}

void CGameStateRun::show_image_by_phase() {

	if (phase <= MAP_COUNT) {
		background.ShowBitmap();
		show_map();
		for (int i = 0; i < int(fin.size()); i++) {
			fin[i].ShowBitmap();
		}
		if (bomb.size() > 0) {
			for (int i = 0; i < int(bomb.size()); i++) {
				bomb[i].show();
			}

		}
		character.show();
		for (int i = 0; i < int(box.size()); i++) {
			box[i].show();
		}
		dead.ShowBitmap();
		load.ShowBitmap();
	}
	scare.ShowBitmap();
	map_menu.ShowBitmap();
	menu_box.ShowBitmap();
}



void game_framework::CGameStateRun::load_background()
{


	int i_max, j_max, c;
	for (int k = 0; k < MAP_COUNT; k++) {  //初始化 地圖數量=k
		ifstream ifs("map/phase" + std::to_string(k + 1) + ".map");
		ifs >> i_max;
		ifs >> j_max;
		map.push_back(vector<vector<int> >());
		background_map.push_back(vector<vector<CMovingBitmap> >());
		for (int i = 0; i < i_max; i++) {
			map[k].push_back(vector<int>());
			background_map[k].push_back(vector<CMovingBitmap>());
			for (int j = 0; j < j_max; j++) {
				ifs >> c;
				map[k][i].push_back(c);
				background_map[k][i].push_back(CMovingBitmap());
			}
		}
		ifs.close();
	}


	reset_phase(0);
}


void CGameStateRun::show_text_by_phase() {
	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(0, 0, 0), 800);
	for (int p = 1; p <= MAP_COUNT; p++) {
		if (while_load) {
			CTextDraw::Print(pDC, 50, 20, "");
		}

		else if (while_menu) {
			CTextDraw::Print(pDC, 50, 20, "");
		}

		else if (phase == p && sub_phase == 1) {

			CTextDraw::Print(pDC, 50, 20, "關卡 : " + std::to_string(p) + "/ 20");
			CTextDraw::Print(pDC, 370, 20, "將箱子推到指定地點");
		}
	}
	if (while_load) {
		CTextDraw::Print(pDC, 50, 20, "");
	}
	else if (while_menu) {
		CTextDraw::Print(pDC, 50, 20, "");
	}
	else if (while_dead) {
		CTextDraw::Print(pDC, 420, 320, "按R重製關卡");
	}


	else if (phase == 1)
	{
		CTextDraw::Print(pDC, 150, 450, "hint : press R to reset");

	}
	CDDraw::ReleaseBackCDC();
}






void CGameStateRun::reset_phase(int phase_chose) {
	wall.clear();
	bomb.clear();
	box.clear();
	fin.clear();
	dead.SetFrameIndexOfBitmap(0);
	int k = phase_chose;
	for (size_t i = 0; i < map[k].size(); i++) {
		for (size_t j = 0; j < map[k][i].size(); j++) {

			background_map[k][i][j].LoadBitmapByString({ "resources/gress.bmp" });
			background_map[k][i][j].SetTopLeft(MAP_SIZE*j + 120, MAP_SIZE*i + 120);

			if (map[k][i][j] == 1) {
				
			}
			if (map[k][i][j] == 2) {

				wall.push_back(CMovingBitmap());
				wall[wall.size() - 1].LoadBitmapByString({ "resources/wall.bmp" });
				wall[wall.size() - 1].SetTopLeft(MAP_SIZE*j + 120, MAP_SIZE*i + 120);
			}
			if (map[k][i][j] == 3) {

				box.push_back(Box());
				box[box.size() - 1].Setnew(int(i),int(j));
			}
			if (map[k][i][j] == 4) {

				character.Setnew(int(i),int(j));

			}
			if (map[k][i][j] == 5) {

				fin.push_back(CMovingBitmap());
				fin[fin.size() - 1].LoadBitmapByString({ "resources/fin.bmp", "resources/fin_ignore.bmp" }, RGB(255, 255, 255));
				fin[fin.size() - 1].SetTopLeft(MAP_SIZE*j + 120, MAP_SIZE*i + 120);
				fin[fin.size() - 1].SetFrameIndexOfBitmap(0);


			}
			if (map[k][i][j] == 6) {

				bomb.push_back(Bomb());
				bomb[bomb.size() - 1].Setnew(int(i), int(j));


			}
			if (map[k][i][j] == 7) {

				box.push_back(Box());
				box[box.size() - 1].Setnew(int(i), int(j));

				fin.push_back(CMovingBitmap());
				fin[fin.size() - 1].LoadBitmapByString({ "resources/fin.bmp", "resources/fin_ignore.bmp" }, RGB(255, 255, 255));
				fin[fin.size() - 1].SetTopLeft(MAP_SIZE*j + 120, MAP_SIZE*i + 120);
				fin[fin.size() - 1].SetFrameIndexOfBitmap(0);


			}
		}
	}
}


void CGameStateRun::phase_control() {
	for (int g = 1; g <= MAP_COUNT; g++) {
		if (phase == g) {
			if (sub_phase == 1) {
				sub_phase += win_flag;
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
				win_flag = 0;
				reset_phase(g);

				if (phase == MAP_COUNT) {
					GotoGameState(GAME_STATE_OVER);
				}

			}
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




