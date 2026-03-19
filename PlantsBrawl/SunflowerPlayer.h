#pragma once

#include <iostream>

#include "Resources.h"
#include "Player.h"
#include "Vector2.h"

class SunflowerPlayer : public Player
{
public:
	SunflowerPlayer() 
	{
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		animation_sun_text.set_atlas(&atlas_sun_text);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75); 
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);
		animation_sun_text.set_interval(100);

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
		Bullet* bullet = new Bullet();

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
	}

	void on_attack_ex() override
	{
		is_attacking_ex = true;
		is_sun_text_visible = true;
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();
		is_sun_text_visible = true; // 显示头顶日字动画
	}

private:
	Animation animation_sun_text; // 头顶日字动画集
	bool is_sun_text_visible = false; // 头顶日字是否可见

	const float speed_sun_ex = 0.15f; // 大招下落速度
	const Vector2 velocity_sun = { 0.25f, -0.5f }; // 普攻抛射速度
};