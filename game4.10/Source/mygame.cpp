/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <cstdlib>
#include <time.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
bool times = true;
int start,END =0;
void delay()
	{
			time_t start_time, cur_time; // 变量声明
			time(&start_time);
			do {
			 time(&cur_time);
			} while ((cur_time - start_time) < 1);
	}
namespace game_framework {


	
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////
	CGameStateInit::CGameStateInit(CGame* g)
		: CGameState(g)
	{
	}
	CPractice::CPractice(){
		x = 30;
		y = 150;
	}
	int k,i = 0;
	void CPractice::OnMove() {
		
		int const interval[7] = { 90,150,210,270,330,390,450 };
		vector <int> v = { 0,0,0,6,7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6 };
		if (x <= 450) {
			x += v[k];
			
		}
		else {
			x = 30;
			y = 150;
		}
		//30 90 150 210 270 330 390 450
		if (x >= interval[i]) {
			if (i % 2 == 0) {
				y = 50;
			}
			else {
				y = 150;
			}
			i++;
		} 
		if (x >= 450) {
			i = 0;
		}
	}
	void CPractice::LoadBitmap() {
		pic.LoadBitmap(IDB_BITMAP4);

	}
	void CPractice::OnShow() {
		pic.SetTopLeft(x, y);
		pic.ShowBitmap();
	}
	int CPractice::getX() {
		return this->x;
	}
	void CBouncingBall::SetXY(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void CBouncingBall::SetFloor(int floor) {
		this->floor = floor;
	}
	void CBouncingBall::SetVelocity(int velocity) {
		this->velocity = velocity;
		this->initial_velocity = velocity;
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		for (int i = IDB_BITMAP9; i < IDB_BITMAP16; i++) {
			title1.AddBitmap(i);
		}
		title1.SetTopLeft(40, 80);
		
		x = 450;
		y = 70;
		logo.LoadBitmap(IDB_INITSELECTBOX);
		logo.SetTopLeft(x, y);
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\click.mp3");
		CAudio::Instance()->Load(AUDIO_CLICK, "sounds\\dingT1.mp3");
		
		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
		if (times) {
			CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\menu.mp3");
			times = false;
		}

		CAudio::Instance()->Play(AUDIO_LAKE, true);
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		const char KEY_ENTER = 13;
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		//const char KEY_ENTER = 0;
		if (nChar == KEY_ENTER) {
			if (y == 70) {
				CAudio::Instance()->Play(AUDIO_DING);
				CAudio::Instance()->Stop(AUDIO_LAKE);
				GotoGameState(GAME_STATE_RUN);// 切換至GAME_STATE_RUN
				END = clock();
			}
			else if (y >= 270) {
				PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
			}
		}
		else if (nChar == KEY_DOWN) {
			CAudio::Instance()->Play(AUDIO_DING);
			if (y < 270)
				y += 200;
			logo.SetTopLeft(450, y);
		}
		else if (nChar == KEY_UP) {
			CAudio::Instance()->Play(AUDIO_DING);
			if (y >= 170)
				y -= 200;
			logo.SetTopLeft(450, y);
		}
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	void CGameStateInit::OnMove() {
		title1.OnMove();
	}
	void CGameStateInit::OnShow()
	{
		//
		// 貼上logo
		//
		//logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
		//title.ShowBitmap();
		logo.ShowBitmap();
		title1.SetDelayCount(3);
		title1.OnShow();
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, * fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 200));

		pDC->TextOut(450, 105, "開始遊戲");

		//pDC->TextOut(300,205,"Ctrl-F 切換至全螢幕或視窗");
		//if (ENABLE_GAME_PAUSE)
			//pDC->TextOut(300,305,"Ctrl-Q 暫停遊戲");
		pDC->TextOut(450, 305, "離開遊戲");
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame* g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, * fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);					// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame* g)
		: CGameState(g)
	{
		
	}

	CGameStateRun::~CGameStateRun()
	{
		
	}

	void CGameStateRun::OnBeginState()
	{
		LoadKeyboardLayout("0x0409", KLF_ACTIVATE | KLF_SETFORPROCESS);
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
<<<<<<< HEAD
		const int HITS_LEFT = 44;
=======
		const int HITS_LEFT = 5;
>>>>>>> 4c4b9121bf0c353f197b58150a522ea56665bbcc
		int CLOCK = start;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;

		test1.SetXY(450, 0);
		test1.SetIsAlive(true);
		test1.SetIsShow(false);
		//hand2.SetXY(SIZE_X - hand1.Width() - 20, SIZE_Y - hand1.Height());
		
		
		hand.SetDelayCount(1);
		background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
		//help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
		clocktime.SetInteger(CLOCK);
		clocktime.SetTopLeft(0, 400);
		hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, false);			// 撥放 MIDI

		
	}
	ofstream ofs;
	
	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// 移動背景圖的座標
		//
		background1.OnMove();
		
		start = (clock()-END)-600;
		bar.OnMove();
		//clocktime.SetInteger(start);
		int const min = 20;
		int const max = 480;
		int const minx = 0;
		int const maxx = 300;
		/*if (start/40 == clap[c]/40) {
			c++;
			int x = rand() % (maxx - min + 1) + min;
			int y = rand() % (max + 1) + 0;
			
			background.SetTopLeft(x,y);
		}
		if (start / 100 > (clap[43] / 100 )+50) {
			c++;
		}*/
		
		
		/* 產生 [min , max] 的整數亂數 */
		
		
		

		c_practice.OnMove();
		/*
		if (c_practice.getX() == 450 && test1.IsAlive()) {
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}
		*/
		//bball.OnMove();
		//gamemap.OnMove();
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//
		isClick = false;
		c = 0;
		test1.LoadBitmap(177);
		rank.LoadBitmap(IDB_BITMAP30);
		background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
		background1.AddBitmap(IDB_BG1);
		background1.AddBitmap(IDB_BG2);
		background1.SetDelayCount(7);
		// 完成部分Loading動作，提高進度
		//
		ShowInitProgress(50);
		//Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 繼續載入其他資料
		//
		for (int i = IDB_BITMAP17; i < IDB_BITMAP27+1; i++) { //2021/04/23 broken leg;
			bar.AddBitmap(i, RGB(255, 255, 255));
		}
		bar.SetDelayCount(7);
		hand.AddBitmap(IDB_HAND1,RGB(255, 255, 255));
		hand.AddBitmap(IDB_HAND2, RGB(255, 255, 255));
		test.LoadBitmap(IDB_INITSELECTBOX);
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
		corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
		corner.ShowBitmap(background);							// 將corner貼到background
		//bball.LoadBitmap();										// 載入圖形
		hits_left.LoadBitmap();
		//clocktime.LoadBitmap();
		c_practice.LoadBitmap();
		//gamemap.LoadBitmap();
		//CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding1.mp3");	// 載入編號0的聲音ding.wav
		//CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\1-11.mp3");	// 載入編號2的聲音ntut.mid
		
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE) {
			hand.OnMove();
			
			//test1.SetIsShow(!test1.IsShow());
			//test1.OnShow();
			//test1.SetIsShow(true); 2021/04/08
		}
	}
	bool H = false;
	vector <int> fuck;
	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE) {
			//test1.SetIsShow(!test1.
<<<<<<< HEAD
			hits_left.Add(-1);
			/*
			if (start - 300 < clap[c] && clap[c] < start + 300) {
				hits_left.Add(1);
=======
			isClick = true;
			
			if (start - 250 < clap[c] && clap[c] < start + 250) {
				isGet = true;
>>>>>>> 4c4b9121bf0c353f197b58150a522ea56665bbcc
			}
			else {
				isGet = false;
			}
			*/
			test1.SetIsShow(true);
			
			hand.OnMove();
			hand.Reset();
			
			fuck.push_back(start);
			
			//hand2.SetIsShow(true);
			
			//test1.OnShow();
			//test1.SetIsAlive(!test1.IsAlive());
			/*if(c_practice.getX()>=4
			00)
				hits_left.Add(+1);
			else
				hits_left.Add(-1);*/
			//
			// 若剩餘碰撞次數為0，則跳到Game Over狀態
			//
<<<<<<< HEAD
			if (c == 44 || hits_left.GetInteger()<=0) {
				//CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				/*ofs.open("firsttempo2.txt");
				for (auto f : fuck) {
					ofs << f << endl;
				}
				ofs.close();*/
				GotoGameState(GAME_STATE_OVER);
				
			}
			c++;
=======

			
>>>>>>> 4c4b9121bf0c353f197b58150a522ea56665bbcc
			int x = rand() % (400 - 20 + 1) + 20;
			int y = rand() % (480 + 1) + 0;
			background.SetTopLeft(x, y);
		}
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//
		//gamemap.OnShow();
		
		background1.OnShow();			// 貼上背景圖
		background.ShowBitmap();
		help.ShowBitmap();					// 貼上說明圖
		hits_left.ShowBitmap();
		//clocktime.ShowBitmap();
		bar.SetTopLeft(0, 220);
		bar.OnShow();
		if (test1.IsShow()) {
			test1.OnShow();
			//test1.SetIsAlive(!test1.IsAlive());
		}
		//isGet = false;
		if (clap[c]+100 <= start) { //到了一個時間點才會做判斷 而不是按空白鍵做判斷
			if (!isClick) {
				hits_left.Add(-1);
			}
			else {
				if (isGet) {
					hits_left.Add(1);
				}
				else
				{
					hits_left.Add(-1);
				}
			}
			c++;
			isClick = false;
			if (c == 44 || hits_left.GetInteger() <= 0) {
				//CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				/*
				ofs.open("time8.txt");
				for (auto f : fuck) {
					ofs << f << endl;
				}
				ofs.close();
				*/
				//GotoGameState(GAME_STATE_OVER);
			}
		}
		if (clap[c - 1] + 5000 <= start)
			GotoGameState(GAME_STATE_OVER);
		test1.SetIsShow(false);
		hand.SetTopLeft(640 - 479, 480 - 75);
		hand.OnShow();
		//
		//  貼上左上及右下角落的圖
		//
		corner.SetTopLeft(0, 0);
		corner.ShowBitmap();
		corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		corner.ShowBitmap();
		//gamemap.OnShow();
		//c_practice.OnShow();
	}
}