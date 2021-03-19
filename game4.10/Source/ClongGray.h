namespace game_framework {
	class ClongGray
	{
	public:
		ClongGray();
		bool IsAlive();											// 是否活著
		bool IsShow();
		void LoadBitmap();										// 載入圖形
		//void OnMove();											// 移動
		void OnShow();											// 將圖形貼到畫面
		void SetXY(int nx, int ny);								// 設定圓心的座標
		void SetIsAlive(bool alive);							// 設定是否活著
		void SetIsShow(bool show);
		//void SetDelay(int d);									// 設定旋轉的速度
	protected:
		CMovingBitmap bmp;			// 圖
		//CMovingBitmap bmp_center;	// 圓心的圖			
		int x, y;					// 圓心的座標
		bool is_alive;				// 是否活著
		bool is_show;
	};
}