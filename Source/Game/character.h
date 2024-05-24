#ifndef CHARACTER_H
#define CHARACTER_H


#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include "../Library/gameutil.h"
#include "config.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>

namespace game_framework{
	class Character {
	public:
		Character() {
			
		}
		~Character() {

		}
		void Setnew(int i, int j) {
			character.LoadBitmapByString({ "resources/man.bmp" }, RGB(255, 255, 255));
			character.SetTopLeft(120 + MAP_SIZE * j, 120 + MAP_SIZE * i);

		}
		
		void moving(UINT nChar) {
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
		}
		int last_location() {
			return last_x, last_y;
		}
		void move_to_last() {
			character.SetTopLeft(last_x, last_y);
			last_x = character.GetLeft();
			last_y = character.GetTop();
		}
		bool is_pushbox(CMovingBitmap box) {
			if (CMovingBitmap::IsOverlap(character, box)) return 1;
			return 0;
		}
		bool is_bomb(CMovingBitmap bomb) {
			if (CMovingBitmap::IsOverlap(character, bomb)) return 1;
			return 0;
		}
		bool is_wall(CMovingBitmap wall) {
			if (CMovingBitmap::IsOverlap(character, wall)) return 1;
			return 0;
		}
		void show() {
			character.ShowBitmap();
		}
	private:
		CMovingBitmap character;
		int last_x;
		int last_y;
	};
};

#endif CHARACTER_H