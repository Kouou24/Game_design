#ifndef BOX_H
#define BOX_H


#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include "../Library/gameutil.h"
#include "config.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>


namespace game_framework {
	class Box
	{
	public:
		Box() {}
		~Box() {}
		void Setnew(int i, int j) {
			box.LoadBitmapByString({ "resources/box.bmp" }, RGB(255, 255, 255));
			box.SetTopLeft(MAP_SIZE*j + 120, MAP_SIZE*i + 120);
			lastbox_x = MAP_SIZE*j + 120;
			lastbox_y = MAP_SIZE*i + 120;

		}
		void box_pushed(UINT nChar) {
			if (nChar == VK_UP) {
				lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
				box.SetTopLeft(box.GetLeft(), box.GetTop() - MAP_SIZE);
			}
			if (nChar == VK_DOWN) {
				lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
				box.SetTopLeft(box.GetLeft(), box.GetTop() + MAP_SIZE);
			}
			if (nChar == VK_RIGHT) {
				lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
				box.SetTopLeft(box.GetLeft() + MAP_SIZE, box.GetTop());
			}
			if (nChar == VK_LEFT) {
				lastbox_x = box.GetLeft(), lastbox_y = box.GetTop();
				box.SetTopLeft(box.GetLeft() - MAP_SIZE, box.GetTop());
			}
		}
		CMovingBitmap pushout() {
			return box;
		}

		void move_to_last() {
			box.SetTopLeft(lastbox_x, lastbox_y);
			lastbox_x = box.GetLeft();
			lastbox_y = box.GetTop();
		}
		void movell() {//好像是沒必要的東西 但不確定 先留著
			lastbox_x = box.GetLeft();
			lastbox_y = box.GetTop();
			box.SetTopLeft(lastbox_x, lastbox_y);
		}

		void show() {
			box.ShowBitmap();
		}
	private:
		CMovingBitmap box;
		int lastbox_x;
		int lastbox_y;
	};

}
#endif BOX_H