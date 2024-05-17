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
	class bomb
	{
	public:
		bomb(){}
		~bomb(){}
		void Setnew(int i, int j) {
			Bomb.LoadBitmapByString({ "resources/man.bmp" }, RGB(255, 255, 255));
			Bomb.SetTopLeft(120 + MAP_SIZE * j, 120 + MAP_SIZE * i);

		}
		CMovingBitmap pushout() {
			return Bomb;
		}
		void show() {
			Bomb.ShowBitmap();
		}
	private:
		CMovingBitmap Bomb;
	};

}
#endif BOMB_H