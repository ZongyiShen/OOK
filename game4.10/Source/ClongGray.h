namespace game_framework {
	//節奏醫生中案空白鍵會跑出來的物件
	class ClongGray
	{
	public:
		ClongGray();
		bool IsAlive();											// 是否活著
		bool IsShow();
		void LoadBitmap();										// 載入圖形
		//void OnMove();											// 移動
		void OnShow();											// 將圖形貼到畫面
		void SetXY(int nx, int ny);								// 設定座標
		void SetIsAlive(bool alive);							// 設定是否活著
		void SetIsShow(bool show);
		//void SetDelay(int d);								
	protected:
		CMovingBitmap bmp;			// 圖
		//CMovingBitmap bmp_center;	// 圓心的圖			
		int x, y;					// 圖的座標
		bool is_alive;				// 是否活著
		bool is_show;
	};
}