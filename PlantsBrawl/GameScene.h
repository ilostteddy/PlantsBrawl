#pragma once

#include <iostream>
#include <graphics.h>
#include <vector>

#include "util.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Platform.h"
#include "PeashoterPlayer.h"
#include "SunflowerPlayer.h"
#include "Vector2.h"
#include "StatusBar.h"
#include "Resources.h"

extern std::vector<Platform> platform_list;
extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Player* player_1;
extern Player* player_2;

extern Camera main_camera;

extern SceneManager scene_manager;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() override 
	{	
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL); 

		is_game_over = false;
		is_slide_out_started = false;

		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(2500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]()
			{
				is_slide_out_started = true;
			});

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]()
			{
				scene_manager.switch_to(SceneManager::SceneType::Menu);
			});
		
		// 初始化玩家位置
		player_1->set_position(200, 50);
		player_2->set_position(975, 50);

		// 初始化头像和状态栏
		status_bar_1P.set_avatar(img_player_1_avatar);
		status_bar_1P.set_position(235, 625);

		status_bar_2P.set_avatar(img_player_2_avatar);
		status_bar_2P.set_position(675, 625);

		// 计算天空和山脉图片的居中位置
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4);

		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.render_position.x = 122;
		large_platform.render_position.y = 455;
		large_platform.shape.left = (float)large_platform.render_position.x + 30;
		large_platform.shape.right = (float)large_platform.render_position.x + img_platform_large.getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_position.y + 60;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_position.x = 175;
		small_platform_1.render_position.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_position.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_position.x = 855;
		small_platform_2.render_position.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_position.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_position.x = 515;
		small_platform_3.render_position.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_position.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_position.y + img_platform_small.getheight() / 2;

	};

	void on_update(int delta) override 
	{
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);

		// 使用迭代器和lambda表达式，结合`std::remove_if`算法来移除无效的子弹对象
		// C++ 标准的 `erase-remove_if` 惯用法
		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{
				bool deletable = bullet->check_can_remove();
				if (deletable)
					delete bullet; // 删除子弹对象，释放内存
				return deletable;
			}),
			bullet_list.end());

		for (Bullet* bullet : bullet_list)
		{
			bullet->on_update(delta);
		}

		// 获取玩家位置给予玩家掉落检测
		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();

		// 如果玩家掉落出屏幕底部，直接扣光HP
		if (position_player_1.y >= getheight())
			player_1->set_hp(0); 
		if (position_player_2.y >= getheight())
			player_2->set_hp(0);

		// 测试弹出消息并退出
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			if (!is_game_over)
			{
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}

			is_game_over = true;
		}

		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_1P.set_mp(player_1->get_mp());

		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		// 如果游戏结束，执行结算动效的更新逻辑
		if (is_game_over)
		{
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);

			if (!is_slide_out_started) // 根据当前是否滑出的标志，执行不同的逻辑
			{
				timer_winner_slide_in.on_update(delta);

				// 如果当前滑入动画还未完成，则继续更新位置；如果已经完成，则保持在目标位置不动
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
			}
			else
				// 如果已经开始滑出动画，则继续更新定时器，直到定时器触发切换场景
				timer_winner_slide_out.on_update(delta);
		}

	};

	void on_draw(const Camera& camera) override
	{
		// 山脉和天空背景绘制
		putimage_alpha(pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(pos_img_sky.x, pos_img_sky.y, &img_hills);

		for (Platform& platform : platform_list)
		{
			platform.on_draw(camera);
		}

		if (is_debug)
		{
			settextcolor(RED);
			outtextxy(15, 15, _T("已开启调试模式，按 Q 键关闭"));
		}


		player_1->on_draw(camera);
		player_2->on_draw(camera);

		if (is_game_over)
		{
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y, player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}


		for (Bullet* bullet : bullet_list)
		{
			bullet->on_draw(camera);
		}

		status_bar_1P.on_draw();
		status_bar_2P.on_draw();
	};

	void on_input(const ExMessage& msg) override 
	{
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			// Q键切换DEBUG模式
			if (msg.vkcode == 'Q')
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	};

	void on_exit() override
	{
		// 1. 删除玩家对象（堆内存）
		delete player_1;
		player_1 = nullptr;
		delete player_2;
		player_2 = nullptr;

		// 2. 关闭调试
		is_debug = false;

		// 3. 清空子弹容器（只清指针，不清对象）
		// 正常游戏应该先循环 delete 所有 bullet 再 clear
		bullet_list.clear();

		// 4. 重置相机
		main_camera.reset_position();
	};

private:
	POINT pos_img_sky = { 0, 0 }; // 天空图片位置
	POINT pos_img_hills = { 0, 0 }; // 山脉图片位置

	StatusBar status_bar_1P;
	StatusBar status_bar_2P;

	bool is_game_over = false; // 游戏结束标志

	const float speed_winner_bar = 3.0f;
	const float speed_winner_text = 1.5f;

	POINT pos_img_winner_bar = { 0 };    // 结算动效背景位置
	POINT pos_img_winner_text = { 0 };  // 结算动效文本位置
	int pos_x_img_winner_bar_dst = 0;   // 结算动效背景移动的目标位置
	int pos_x_img_winner_text_dst = 0;  // 结算动效文本移动的目标位置

	Time timer_winner_slide_in; // 结算动效滑入定时器
	Time timer_winner_slide_out;  // 结算动效停留定时器
	bool is_slide_out_started = false; // 是否开始滑出动画
};