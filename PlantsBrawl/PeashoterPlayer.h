#pragma once

#include <iostream>

#include "Player.h"
#include "Resources.h"
#include "Animation.h"
#include "util.h"

class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer()
	{
		// 初始化豌豆射手的动画
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);

		// 初始化动画帧间隔
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		size.x = 96; // 设置角色宽度
		size.y = 96; // 设置角色高度
	};

	~PeashooterPlayer() = default;

	void on_update(int delta_time) override
	{
		Player::on_update(delta_time); // 调用基类的更新方法，处理基本的动画切换逻辑
	};

	void on_input(const ExMessage& msg) override
	{
		Player::on_input(msg); // 调用基类的输入处理方法，处理基本的移动输入逻辑
	};

};