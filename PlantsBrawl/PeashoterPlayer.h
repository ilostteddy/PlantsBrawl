#pragma once

#include <iostream>
#include <vector>	

#include "Player.h"
#include "Resources.h"
#include "Animation.h"
#include "util.h"
#include "Pea_bullet.h"
#include "Vector2.h"
#include "Time.h"

extern Camera main_camera;

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
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);

		// 初始化动画帧间隔
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);

		size.x = 96; // 设置角色宽度
		size.y = 96; // 设置角色高度

		// 初始化攻击状态定时器
		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]()
			{
				is_attacking_ex = false; // 特殊攻击结束后重置状态
			});

		// 特殊攻击状态下发射子弹的定时器
		timer_spwan_pea_ex.set_wait_time(100);
		timer_spwan_pea_ex.set_callback	([&]()
			{
				spawn_pea_bullet(speed_pea_ex); // 生成特殊攻击豌豆子弹
			});

		// 普攻CD
		attack_cd = 100;
	};

	~PeashooterPlayer() = default;

	void on_update(int delta_time) override
	{
		Player::on_update(delta_time); // 调用基类的更新方法，处理基本的动画切换逻辑

		if (is_attacking_ex)
		{
			main_camera.shake(5, 100); // 特殊攻击时摄像机震动，参数为震动强度和持续时间
			timer_attack_ex.on_update(delta_time); // 更新特殊攻击状态定时器
			timer_spwan_pea_ex.on_update(delta_time); // 更新特殊攻击子弹发射定时器
		}
	};

	void on_input(const ExMessage& msg) override
	{
		Player::on_input(msg); // 调用基类的输入处理方法，处理基本的移动输入逻辑
	};

	void on_attack() override
	{
		//Player::on_attack(); // 调用基类的攻击方法，处理攻击冷却逻辑
		spawn_pea_bullet(speed_pea); // 生成普通豌豆子弹

		// 随机播放普通豌豆射击音效
		switch (rand() % 2)
		{
		case 0:
			mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
			break;
		}
	};

	void on_attack_ex() override
	{
		is_attacking_ex = true; // 进入特殊攻击状态
		timer_attack_ex.restart(); // 重置特殊攻击状态定时器

		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset(); // 重置特殊攻击动画
		
		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
	};


private:
	const float speed_pea = 0.75f;
	const float speed_pea_ex = 1.0f;
	const int attack_ex_duration = 2500; // 特殊攻击持续时间，单位为毫秒

	Time timer_attack_ex;	// 特殊攻击状态定时器
	Time timer_spwan_pea_ex;	//豌豆子弹发射定时器

private:
	void spawn_pea_bullet(float speed)
	{
		Bullet* bullet = new Pea_bullet(); // 创建一个新的豌豆子弹对象

		Vector2 bullet_position;	// 临时储存子弹位置的变量
		Vector2 bullet_velocity;	// 临时储存子弹速度的变量

		// 子弹发射位置
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = is_facing_right 
			? position.x + size.x - bullet_size.x / 2		// 朝右发射，子弹位置在角色右侧
			: position.x - bullet_size.x / 2;				// 朝左发射，子弹位置在角色左侧
		bullet_position.y = position.y;
		
		// 子弹发射速度
		bullet_velocity.x = is_facing_right
			? speed
			: -speed;
		bullet_velocity.y = 0;

		bullet->set_position(bullet_position.x, bullet_position.y); // 设置子弹位置
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y); // 设置子弹速度

		// 设置子弹碰撞目标
		bullet->set_target((id == Player_id::P1) ? Player_id::P2 : Player_id::P1); // 如果当前玩家是P1，子弹目标是P2；反之亦然

		// 设置子弹碰撞回调函数
		bullet->set_callback([&]()
			{
				mp += 25;
			});

		bullet_list.push_back(bullet); // 将子弹添加到全局子弹列表中，供游戏循环更新和绘制
	}
};