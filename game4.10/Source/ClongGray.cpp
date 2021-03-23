#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include "CBall.h"
#include "ClongGray.h"
namespace game_framework {
	ClongGray::ClongGray() {
		is_alive = true;
		x = y = 0;
	}
	bool ClongGray::IsAlive() {
		return is_alive;
	}
	bool ClongGray::IsShow() {
		return is_show;
	}
	void ClongGray::LoadBitmap() {
		bmp.LoadBitmap(IDB_LONGGRAY);
	}
	void ClongGray::SetIsAlive(bool alive)
	{
		is_alive = alive;
	}
	void ClongGray::SetIsShow(bool show) {
		is_show = show;
	}
	void ClongGray::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void ClongGray::OnShow()
	{
		if (is_alive) {
			bmp.SetTopLeft(x, y);
			bmp.ShowBitmap();
		}
	}
}