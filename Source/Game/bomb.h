#ifndef BOMB_H
#define BOMB_H


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
	class Bomb
	{
	public:
		Bomb(){}
		~Bomb(){}
		void Setnew(int i, int j) {
			bomb.LoadBitmapByString({ "resources/bomb.bmp", "resources/bomb_ignore.bmp" }, RGB(255, 255, 255));
			bomb.SetTopLeft(MAP_SIZE*j + 120, MAP_SIZE*i + 120);
			bomb.SetFrameIndexOfBitmap(0);

		}
		CMovingBitmap pushout() {
			return bomb;
		}
		void show() {
			bomb.ShowBitmap();
		}
	private:
		CMovingBitmap bomb;
	};

}
#endif BOMB_H