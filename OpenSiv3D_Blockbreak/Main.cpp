# include <Siv3D.hpp> // Siv3D v0.6.14


class Ball;
class Block_Mngr;
//バーの管理クラス
class Bar {
public:
	/// @brief コンストラクタ
	/// @param[in] bar_ バーの初期位置
	Bar(Vec2 bar_) :bar(bar_) {}
	/// @brief バーとの反射
	/// @param[in,out] ball ボールの位置
	/// @param[in,out] ballv ボールの速度
	/// @param[in,out] rally ラリー回数
	/// @return 反射した場合true,それ以外はfalse
	bool refrect(Vec2& ball, Vec2& ballv, int& rally) {
		Vec2 ballp = ball + ballv;
		if (ballv.y > 0) {
			if (ballp.y < bar.y || ball.y >= bar.y)return false;
		}
		else {
			return false;
		}
		double bar0 = bar.x - 50;
		double bar1 = bar0 + 33.5;
		double bar2 = bar0 + 66.5;
		double bar3 = bar0 + 100;
		double ball_vec = ballv.x / ballv.y;	//ボールの傾き
		double ball_line = (bar.y - ball.y) * ball_vec + ball.x;	//バーとの線分
		Vec2 ballv0 = { 2.5, 2.5 };		//ボールの初速度
		//バーの左側1/3
		if (bar0 <= ball_line && ball_line <= bar1) {
			//ボールの速度の傾きが右向き
			if (ballv.x > 0) {
				ballv = -ballv;
				ballv0 *= -1;
			}
			//ボールが直線運動
			else if (ballv.x < 0) {
				ballv = { 0, -(ballv.y * sqrt(2)) };
				ballv0 = { 0, -(ballv0.y * sqrt(2)) };
			}
			//ボールの傾きが左側向き
			else if (ballv.x == 0) {
				ballv.y = -(ballv.y / sqrt(2));
				ballv.x = ballv.y;
				ballv0 *= -1;
			}
			//ボールの位置情報更新
			ball = { ball_line, bar.y };
			ballp -= ball;
			ball += ballp;
			rally = 0;
			ballv0 = ballv - ballv0;
			//ボールの向き変更
			ballv -= ballv0;
			return true;
		}
		//バーの中央部1/3
		else if (bar1 < ball_line && ball_line < bar2) {
			if (ballv.x > 0) {
				ballv0.y *= -1;
			}
			else if (ballv.x == 0) {
				ballv0 = { 0, -(ballv0.y * sqrt(2)) };
			}
			else if (ballv.x < 0) {
				ballv0 *= -1;
			}
			ballv.y *= -1;
			ball = { ball_line, bar.y };
			ballp -= ball;
			ball += ballp;
			rally = 0;
			ballv0 = ballv - ballv0;
			ballv -= ballv0;
			return true;
		}
		//バーの右側1/3
		else if (bar2 < ball_line && ball_line <= bar3) {
			if (ballv.x < 0) {
				ballv *= -1;
				ballv0.y *= -1;
			}
			else if (ballv.x > 0) {
				ballv = { 0, -(ballv.y * sqrt(2)) };
				ballv0 = { 0, -(ballv0.y * sqrt(2)) };
			}
			else if (ballv.x == 0) {
				ballv.y = -(ballv.y / sqrt(2));
				ballv.x = -(ballv.y);
				ballv0.y *= -1;
			}
			ball = { ball_line, bar.y };
			ballp -= ball;
			ball += ballp;
			rally = 0;
			ballv0 = ballv - ballv0;
			ballv -= ballv0;
			return true;
		}
		return false;
	}
	/// @brief バーの描画
	void Draw() {
		RectF(Arg::center(bar), 100, 10).draw();
	}
	/// @brief バーの操作
	void move() {
		if (KeyRight.pressed()) {
			bar.x += dy * Scene::DeltaTime();
			if (bar.x >= 800)bar.x = 800;
		}
		if (KeyLeft.pressed()) {
			bar.x -= dy * Scene::DeltaTime();
			if (bar.x <= 0)bar.x = 0;
		}
	}
	/// @brief バーの情報取得
	/// @return バーの位置情報
	const Vec2& Get_bar()const {
		return bar;
	}
	Vec2& Get_bar() {
		return bar;
	}
	void reset_bar(const Vec2& _bar) {
		bar = _bar;
	}
private:
	Vec2 bar;
	double dy = 300;
};
/// @brief 境界線のクラス
class Border {
public:
	/// @brief 境界線との反射
	/// @param[in,out] ball ボールの情報
	/// @param[in,out] ballv ボールの速度
	/// @param[in,out] rally ラリー回数
	/// @return 反射したときtrue,それ以外はfalse
	bool refrect(Vec2& ball, Vec2& ballv, int& rally) {
		double ball_vec1 = ballv.y / ballv.x;
		double ball_vec2 = ballv.x / ballv.y;
		Vec2 ballp = ball + ballv;
		//ボールが画面左側と接触
		if (ball.x <= 0 && ballp.x <= 0) {
			double ball_line = (0 - ball.x) * ball_vec1 + ball.y;
			if (0 <= ball_line && ball_line <= 600) {
				ball = { 0, ball_line };
				ballp -= ball;
				ballp.x *= -1;
				ball += ballp;
				ballv = { -ballv.x, ballv.y };
				rally++;
				return true;
			}
		}
		//画面右側
		if (ball.x >= Scene::Width() && ballp.x >= Scene::Width()) {
			double ball_line = (800 - ball.x) * ball_vec1 + ball.y;
			if (0 <= ball_line && ball_line <= 600) {
				ball = { 800, ball_line };
				ballp -= ball;
				ballp.x *= -1;
				ball += ballp;
				ballv = { -ballv.x, ballv.y };
				rally++;
				return true;
			}
		}
		//画面上部
		if (ball.y <= 0 && ballp.y <= 0) {
			double ball_line = (0 - ball.y) * ball_vec2 + ball.x;
			if (0 <= ball_line && ball_line <= 800) {
				ball = { ball_line, 0 };
				ballp -= ball;
				ballp.y *= -1;
				ball += ballp;
				ballv = { ballv.x, -ballv.y };
				rally++;
				return true;
			}
		}
		return false;
	}
};

class Ball {
public:
	/// @brief コンストラクタ
	/// @param _ball ボールの初期位置
	/// @param _ballV ボールの初速度
	Ball(Vec2 _ball, Vec2 _ballV) :ball(_ball), ballV0(_ballV) {
		ballV = { 0, 0 };
		rally = 0;
	}
	//ボールの描画
	void Draw() {
		cball.draw();
	}
	bool calc_ball(Bar& bar, Border& border, Block_Mngr& block, bool& ball_plus);
	/// @brief ボールの発車
	/// @param[in] bar バーの位置情報
	void move(const Bar& bar) {
		if (ballV == Vec2(0,0)) {
			if (KeySpace.down()) {
				ball = bar.Get_bar();
				ballV = ballV0;
			}
		}
	}
	//ボールの速度を初速度にする
	void move2() {
		ballV = ballV0;
	}
	/*int32 Get_life() {
		return life;
	}*/

	/// @brief ボールの速度を返す
	/// @return ボールの速度を返す
	const Vec2& Get_ballV()const {
		return ballV;
	}
	Vec2& Get_ballV() {
		return ballV;
	}

	/// @return ボールの位置を返す
	const Vec2& Get_ball()const {
		return ball;
	}
	Vec2& Get_ball() {
		return ball;
	}
	/// @brief ボールの情報をリセット
	void reset_ball() {
		ball = { -20, -20 };
		cball = { ball, 5 };
		ballV = { 0, 0 };
		rally = 0;
	}
private:
	Vec2 ball, ballV, ballV0;
	Circle cball;
	int rally;
	double balla = 1;
	Array<Circle> balls;
};
//ブロック一個の描画
class Block : public Rect{
public:
	Block (const Rect& _block, ColorF c):Rect(_block), color(c){}
	ColorF color;
};

/// @brief ブロックの描画管理クラス
class Block_Mngr {
public:
	/// @brief コンストラクタ
	/// @param[in] _block ブロック一個のサイズ
	Block_Mngr(Size _block) :block(_block) {
		for (int y = 0; y < 4; y++) {//ブロックを四列用意する
			for (int x = 0; x < (Scene::Width() / block.x); x++) {
				ColorF blockcolor;
				//列ごとに色付け、三列目中央部に赤色のブロックを設置
				switch (y)
				{
				case 0:
					blockcolor = ColorF(Palette::Blue);
					break;
				case 1:
					/*if (x == 10) {
						blockcolor = ColorF(Palette::Red);
					}*/
					//else {
						blockcolor = ColorF(Palette::Greenyellow);
					//}
					break;
				case 2:
					blockcolor = ColorF(Palette::Aqua);
					break;
				case 3:
					if (x == 10) {
						blockcolor = ColorF(Palette::Red);
					}
					else {
						blockcolor = ColorF(Palette::Orange);
					}
					break;
				}

				//配列として登録
				blocks << Block(Rect{ (x * block.x), (60 + y * block.y), block }, blockcolor);
			}
		}
		score = 0;
	}
	//ブロックの描画
	void Draw() {
		int i = 0;
		for (const auto& blockd : blocks) {
			blockd.stretched(-1).draw(blockd.color);
			i++;
		}
	}
	//スコアの描画
	void Score_draw() {
		font(U"Score: ",score).draw(700, 570);
	}
	/// @brief ブロックとの衝突処理
	/// @param[in,out] ball ボールの位置情報
	/// @param[in,out] ballv ボールの速度
	/// @param[in,out] flag ブロックが存在しているか否か
	/// @param[in,out] rally ラリー回数
	/// @param[in, out] ball_plus 赤色のブロックが消えた時ボールを増やす
	void hit(Circle& ball, Vec2& ballv, bool& flag, int& rally, bool& ball_plus) {
		
		for (auto it = blocks.begin(); it != blocks.end();) {
			if (it->intersects(ball)) {
				if (it->bottom().intersects(ball) || it->top().intersects(ball)) {
					ballv.y *= -1;
				}
				else {
					ballv.x *= -1;
					
				}
				if (it->color == ColorF(Palette::Red)) {
					ball_plus = true;
				}
				it = blocks.erase(it);
				rally++;
				if (ballv == Vec2(2.5, 2.5))score += 10;
				else score += 20;
			}

			else it++;
		}
		if (blocks.empty()) {
			flag = true;
		}
	}
	//ブロックの再配置及びリセット
	void reset_block(const Size& _block) {
		block = _block;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < (Scene::Width() / block.x); x++) {
				ColorF blockcolor;
				switch (y)
				{
				case 0:
					blockcolor = ColorF(Palette::Blue);
					break;
				case 1:
					/*if (x == 10) {
						blockcolor = ColorF(Palette::Red);
					}*/
//					else {
						blockcolor = ColorF(Palette::Greenyellow);
//					}
					break;
				case 2:
					blockcolor = ColorF(Palette::Aqua);
					break;
				case 3:
					if (x == 10) {
						blockcolor = ColorF(Palette::Red);
					}
					else {
						blockcolor = ColorF(Palette::Orange);
					}
					break;
				}
				blocks << Block(Rect{ (x * block.x), (60 + y * block.y), block }, blockcolor);
			}
		}
		score = 0;
	}
private:
	Size block;
	Array<Block>blocks;
	Font font{ 20 };
	int score;
};
//ゲームオーバー処理
class Over {
public:
	/// @brief ゲームオーバーの描画
	/// @param flag ブロックが全て消えたか否か
	void View(bool flag) {
		if (flag == true) {
			title(text1).draw(Arg::center(400, 100), Palette::Red);
		}
		else {
			title(text2).draw(Arg::center(400, 100), Palette::Red);
		}
		font(restart).draw(Arg::center(400, 300));
		font(quit).draw(Arg::center(400, 400));
	}
private:
	const Font title{ 100 };
	const Font font{ 60 };
	String restart = U"Restart Press'R'";
	String quit = U"Quit Press 'Q'";
	String text1 = U"Success!";
	String text2 = U"Failed!";
};

/// @brief ボールの挙動処理
/// @param[in,out] bar バーの情報
/// @param[in,out] border 画面端の情報
/// @param[in,out] block ブロックの情報
/// @param[in,out] ball_plus ボールが増えるタイミング
/// @return ブロックが残っているか否かを変数flagを通してtrue,falseを返す
bool Ball::calc_ball(Bar& bar, Border& border, Block_Mngr& block,bool& ball_plus ) {
	bool colision = false;
	bool flag = false;
	colision = bar.refrect(ball, ballV, rally);
	colision = border.refrect(ball, ballV, rally);
	block.hit(cball, ballV, flag, rally, ball_plus);
	if (colision == false)ball += ballV;
	if (ball.y >= 800) {
		rally = 0;
	}
	cball = { ball, 5 };
	if (rally % 5 == 0 && rally >= 5) {
		if (balla > 1 && ballV == ballV0)balla = 1;
		ballV = ballV0 * (1.0 + balla / 10);
		balla++;
	}
	return flag;
}
//複数のボール管理クラス
class BallMngr{
public:
	/// @brief コンストラクタ
	/// @param _ball ボールの位置
	/// @param _ballV ボールの初速度
	/// @param _life 残機
	BallMngr(Vec2 _ball, Vec2 _ballV, int _life):ball(_ball), ballV(_ballV),life(_life) {
		balls << Ball{_ball, _ballV};
	}
	/// @brief 描画
	void Draw() {
		for (auto& balld : balls) {
			balld.Draw();
		}
		font(U"Life: ", life).draw(10, 570);
	}
	/// @brief ボールの発射
	/// @param bar バーの情報
	void move(const Bar& bar) {
		balls[0].move(bar);
	}
	/// @brief 複数のボールの挙動制御
	/// @param bar バーの情報
	/// @param border 画面端の情報
	/// @param block ブロックの情報
	/// @return ブロックが残っているか否かを変数flagを通してtrue,falseを返す
	bool calc_ball(Bar& bar, Border& border, Block_Mngr& block) {
		bool flag = false, ball_plus = false;
		Vec2 ballvtemp;
		for (size_t j = 0; j < balls.size();) {
			flag = balls[j].calc_ball(bar, border, block, ball_plus);
			if (ball_plus == true) {
				ballvtemp = balls[j].Get_ballV();
				for (int i = 0; i < 2; i++) {
					if (i == 0) {
						//ボールの傾きに応じて増えたボールの傾きを決定する
						if (ballvtemp.x < 0) {
							if (ballvtemp.y < 0) {
								ballvtemp.x *= cos(Math::ToRadians(120));
								ballvtemp.y *= sin(Math::ToRadians(120));
							}
							else {
								ballvtemp.x *= cos(Math::ToRadians(210));
								ballvtemp.y *= sin(Math::ToRadians(210));
							}
						}
						else if(ballvtemp.x > 0){
							if (ballvtemp.y < 0) {
								ballvtemp.x *= cos(Math::ToRadians(30));
								ballvtemp.y *= sin(Math::ToRadians(30));
							}
							else {
								ballvtemp.x *= sin(Math::ToRadians(-30));
								ballvtemp.y *= cos(Math::ToRadians(-30));
							}
						}
						else {
							if (ballvtemp.y < 0) {
								ballvtemp = { 2.5,-2.5 };
							}
							else {
								ballvtemp = { 2.5, 2.5 };
							}
						}
						balls<<Ball{ balls[j].Get_ball(),ballvtemp};
						balls[i + 1].move2();
					}
					else {
						if (ballvtemp.x < 0) {
							if (ballvtemp.y < 0) {
								ballvtemp.x *= cos(Math::ToRadians(150));
								ballvtemp.y *= sin(Math::ToRadians(150));
							}
							else {
								ballvtemp.x *= cos(Math::ToRadians(240));
								ballvtemp.y *= sin(Math::ToRadians(240));
							}
						}
						else if (ballvtemp.x > 0) {
							if (ballvtemp.y < 0) {
								ballvtemp.x *= cos(Math::ToRadians(60));
								ballvtemp.y *= sin(Math::ToRadians(60));
							}
							else {
								ballvtemp.x *= sin(Math::ToRadians(-60));
								ballvtemp.y *= cos(Math::ToRadians(-60));
							}
						}
						else {
							if (ballvtemp.y < 0) {
								ballvtemp = { -2.5,-2.5 };
							}
							else {
								ballvtemp = { -2.5, 2.5 };
							}
						}
						balls << Ball{ balls[j].Get_ball(), ballvtemp };
						balls[i + 1].move2();
					}
				}
				ball_plus = false;
			}
			if (balls[j].Get_ball().y >= 600) {
				balls.erase(balls.begin() + j);
				if (balls.empty()) {
					if (life > 0) {
						balls << Ball{ bar.Get_bar(), Vec2{2.5, -2.5} };
						life--;
					}
					else {
						flag = true;
					}
				}
			}
			else j++;
		}
		return flag;
	}
	//残機数を得る
	int Get_life() {
		return life;
	}
	//増えたボールを消しリセットする
	void reset_ball(int _life) {
		balls.clear();
		balls << Ball{ ball, ballV };
		life = _life;
	}
private:
	Array<Ball> balls;
	Vec2 ball, ballV;
	int life;
	Font font{ 20 };
};

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(Color(0,0,0));

	int life = 3;
	Vec2 _bar = { 400, 580 };
	Size size_block = { 40, 20 };
	String Game_state = U"game";
	Bar bar(_bar);
	Border border;
	//Ball ball(_bar, Vec2(2.5, -2.5));
	BallMngr balls(_bar, Vec2(2.5, -2.5), life);
	Block_Mngr block(size_block);
	Over over;
	bool flag = false;
	

	while (System::Update())
	{
		//ゲーム処理
		if (Game_state == U"game") {
			bar.move();
			balls.move(bar);
			//ブロックがなくなったらゲームオーバー処理に移行
			if (balls.calc_ball(bar, border, block) == true) {
				Game_state = U"over";
			}
			block.Draw();
			bar.Draw();
			balls.Draw();
			block.Score_draw();
		}
		//ゲームオーバー処理
		else if(Game_state == U"over") {
			if (balls.Get_life() != 0)flag = true;//残機がなくなったかどうかをチェック
			over.View(flag);
			if (KeyR.down()) {
				bar.reset_bar(_bar);
				balls.reset_ball(life);
				block.reset_block(size_block);
				Game_state = U"game";
			}
			else if (KeyQ.down()) {
				System::Exit();
			}
		}
	
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
