#pragma once

#include <iostream>

#include "Resources.h"
#include "Player.h"
#include "Vector2.h"
#include "Sun_bullet.h"
#include "Sun_bullet_ex.h"

extern Player* player_1;
extern Player* player_2;

class SunflowerPlayer : public Player
{
public:
	SunflowerPlayer(bool facing_right = true) : Player(facing_right)
	{
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		animation_sun_text.set_atlas(&atlas_sun_text);
		animation_die_left.set_atlas(&atlas_sunflower_die_left);
		animation_die_right.set_atlas(&atlas_sunflower_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75); 
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);
		animation_sun_text.set_interval(100);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false); // 死亡动画不循环
		animation_die_right.set_loop(false);
		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_sun_text.set_loop(false);

		animation_attack_ex_left.set_callback([&]()
			{
				is_attacking_ex = false; // 大招攻击动画播放完成后重置状态
				is_sun_text_visible = false;
			});

		animation_attack_ex_right.set_callback([&]()
			{
				is_attacking_ex = false; // 大招攻击动画播放完成后重置状态
				is_sun_text_visible = false;
			});

		size.x = 96; // 设置角色宽度
		size.y = 96; // 设置角色高度

		attack_cd = 250; // 设置普通攻击冷却时间
	};

	~SunflowerPlayer() = default;

	void on_update(int delta_time) override
	{
		Player::on_update(delta_time); // 调用基类的更新方法，处理基本的动画切换逻辑

		if (is_sun_text_visible)
		{
			animation_sun_text.on_update(delta_time);
		}
	};

	void on_input(const ExMessage& msg) override
	{
		Player::on_input(msg); // 调用基类的输入处理方法，处理基本的移动输入逻辑
	};

	void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);

		if (is_sun_text_visible)
		{
			Vector2 text_position;
			IMAGE* frame = animation_sun_text.get_frame();
			text_position.x = position.x - (size.x - frame->getwidth()) / 2;
			text_position.y = position.y - frame->getheight();
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);
		}
	}

	void on_attack() override
	{
		Bullet* bullet = new Sun_bullet();

		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y + (size.y - bullet_size.y) / 2;

		bullet->set_position(bullet_position.x, bullet_position.y);
		// 根据角色朝向设置子弹水平速度
		bullet->set_velocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, 
							velocity_sun.y);

		bullet->set_target(id == Player_id::P1 ? Player_id::P2 : Player_id::P1);

		bullet->set_callback([&]() { mp += 35; });

		bullet_list.push_back(bullet);

		std::cout << "太阳普通子弹生成发射" << std::endl;
	}

	void on_attack_ex() override
	{
		is_attacking_ex = true;
		is_sun_text_visible = true;

		animation_sun_text.reset(); // 重置日字动画状态，使其从第一帧开始播放
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		Bullet* bullet = new Sun_bullet_ex();
		Player* target_player = (id == Player_id::P1) ? player_2 : player_1;

		// 通过计算使子弹水平居中于目标玩家，并从屏幕上方开始下落的初始位置和速度
		Vector2 bullet_position;
		Vector2 bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		const Vector2& target_size = target_player->get_size();
		const Vector2& target_position = target_player->get_position();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		bullet_position.y = -size.y; // 从屏幕上方开始下落
		bullet_velocity.x = 0;
		bullet_velocity.y = speed_sun_ex;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_callback([&]() { mp += 50; });
		
		bullet_list.push_back(bullet);

		mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
	}

private:
	Animation animation_sun_text; // 头顶日字动画集
	bool is_sun_text_visible = false; // 头顶日字是否可见

	const float speed_sun_ex = 0.15f; // 大招下落速度
	const Vector2 velocity_sun = { 0.25f, -0.5f }; // 普攻抛射速度
};