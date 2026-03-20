#pragma once

#include <graphics.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Vector2.h"
#include "Camera.h"
#include "Player_id.h"
#include "Animation.h"
#include "Platform.h"
#include "Time.h"
#include "Bullet.h"
#include "Pea_bullet.h"
#include "Sun_bullet.h"
#include "util.h"

extern std::vector<Bullet*> bullet_list; // 声明子弹列表，供Player类使用
extern std::vector<Platform> platform_list; // 声明平台列表，供Player类使用

extern bool is_debug;

class Player
{
public:
	Player()
	{
		current_animation = &animation_idle_right; // 默认朝右站立

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
			{
				can_attack = true; // 攻击冷却结束后允许攻击
			});

		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]()
			{
				is_invulnerable = false; // 无敌状态结束后取消无敌
			});

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame; // 无敌状态闪烁切换显示状态
			});
	};

	~Player() = default;

	virtual void on_update(int delta) // 场景更新时调用，参数为时间增量（单位：毫秒）
	{
		int direction = if_rightkey_down - if_leftkey_down;  // 计算移动方向，右键按下为1，左键按下为-1，同时按下或都未按下为0

		if (direction != 0)
		{
			if (!is_attacking_ex)  // 如果不是特殊攻击状态，才根据方向切换动画
				is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left; // 根据方向选择跑步动画
			float distance = run_velocity * delta * direction; // 计算移动距离
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left; // 根据方向选择默认动画
		}

		// 在当前动画更新时，根据当前的特殊攻击状态，来决定是否播放对应放心的动画
		if (is_attacking_ex)
		{
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
		}

		current_animation->on_update(delta); // 更新当前动画状态

		timer_attack_cd.on_update(delta); // 更新攻击冷却计时器状态
		timer_invulnerable.on_update(delta); // 更新无敌状态计时器状态
		timer_invulnerable_blink.on_update(delta); // 更新无敌状态闪烁计时器状态

		if (is_invulnerable)
			sketch_image(current_animation->get_frame(), &img_sketch); // 如果处于无敌状态，生成当前帧的剪影图像

		move_and_collide(delta); // 更新位置并处理碰撞

	};

	virtual void on_input(const ExMessage& msg) // 场景输入事件时调用，参数为输入事件消息
	{
		// 处理ESC键，退出程序
		if (msg.vkcode == VK_ESCAPE) {
			std::cout << "按下ESC键，程序即将退出..." << std::endl;
			// 方案1：Windows平台标准退出（发送退出消息，优雅关闭）
			closegraph(); // 关闭绘图窗口
			exit(0);
			return; // 退出当前函数，避免执行后续逻辑
		}

		// 第一层，根据消息类型（按键按下、按键抬起等）进行分支
		switch (msg.message)
		{
		case WM_KEYDOWN:
			// 第二层，根据玩家序号进行分支，允许不同玩家有不同的按键设置
			switch (id)
			{
			case Player_id::P1:
				// 第三层，当按键按下且玩家序号为P1时，根据按键代码进行分支，执行对应的操作
				switch (msg.vkcode)
				{
					// 'A'键按下，P1向左移动
				case 0x41:
					if_leftkey_down = true;
					break;
					// 'D'键按下，P1向右移动
				case 0x44:
					if_rightkey_down = true;
					break;
					// 'W'跳跃
				case 0x57:
					on_jump(); 
					break;
				case 0x46:
					if (can_attack)
					{
						std::cout << "P1普通攻击！" << std::endl;
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case 0x47:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
				break;
			case Player_id::P2:
				switch (msg.vkcode)
				{
					// '←'键按下，P2向左移动
				case VK_LEFT:
					if_leftkey_down = true;
					break;
					// '→'键按下，P2向右移动
				case VK_RIGHT:
					if_rightkey_down = true;
					break;
				case VK_UP:
					on_jump();
					break;
				case VK_OEM_PERIOD:
					if (can_attack)
					{
						std::cout << "P2普通攻击！" << std::endl;
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case VK_OEM_2:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case Player_id::P1:
				switch (msg.vkcode)
				{
					// 'A'键抬起，P1停止向左移动
				case 0x41:
					if_leftkey_down = false;
					break;
					// 'D'键抬起，P1停止向右移动
				case 0x44:
					if_rightkey_down = false;
					break;
				}
				break;
			case Player_id::P2:
				switch (msg.vkcode)
				{
					// '←'键抬起，P2停止向左移动
				case 0x25:
					if_leftkey_down = false;
					break;
					// '→'键抬起，P2停止向右移动
				case 0x27:
					if_rightkey_down = false;
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		};
	}; 

	virtual void on_draw(const Camera& camera) // 场景绘制时调用，参数为当前摄像机对象
	{
		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		else
			current_animation->on_draw(camera, (int)position.x, (int)position.y); // 绘制当前动画的当前帧，位置根据角色位置计算
	
		if (is_debug)
		{
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
	}

	void set_id(Player_id id) // 设置玩家序号
	{
		this->id = id;
	}

	void set_position(float x, float y) // 设置角色位置
	{
		position.x = x;
		position.y = y;
	}

	void on_run(float distance)
	{
		if (is_attacking_ex)
			return;
		position.x += distance; // 根据移动距离更新角色位置
	}

	void on_jump()
	{
		if (velocity.y !=0 || is_attacking_ex)
			return; // 如果垂直速度不为0，说明角色正在空中，不能再次跳跃
		velocity.y += jump_velocity; // 设置垂直速度为负值，使角色向上跳跃
	}

	virtual void on_attack()
	{

	}

	virtual void on_attack_ex()
	{

	}

	virtual const Vector2 get_position() const
	{
		return position;
	}

	virtual const Vector2 get_size() const
	{
		return size;
	}

	void make_invulnerable() // 使玩家进入无敌状态
	{
		is_invulnerable = true; // 设置无敌状态标志
		timer_invulnerable.restart(); // 重置无敌状态计时器
	}

protected:
	void move_and_collide(int delta)
	{
		velocity.y += gravity * delta; // 应用重力加速度更新垂直速度
		position += velocity * (float)delta; // 根据速度和时间增量更新位置

		if (velocity.y > 0) // 如果垂直速度大于0，表示角色正在下落
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisionShape& shape = platform.shape; // 获取平台的碰撞形状
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left)); // 检查角色与平台在水平方向上是否重叠
				bool is_collide_y = (shape.y >= position.y) && (shape.y <= position.y + size.y); // 检查角色与平台在垂直方向上是否重叠

				if (is_collide_x && is_collide_y) // 如果角色与平台在水平和垂直方向上都发生碰撞，计算角色上一帧脚底的位置是否在平台上方
				{
					float delta_pos_y = velocity.y * delta; // 计算得到这一帧过程中玩家角色在竖直方向上的移动距离
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y; // 计算上一帧玩家脚底的位置
					if (last_tick_foot_pos_y <= shape.y) // 判断在发生碰撞的前一帧，玩家是否整个穿过了平台
					{
						position.y = shape.y - size.y; // 将玩家角色的位置调整到平台上
						velocity.y = 0; // 碰撞后垂直速度归零

						break; // 处理完一个平台的碰撞后就退出循环，避免重复处理多个平台造成位置错误
					}
				}
			}
		}
		if (!is_invulnerable)
		{
			for (Bullet* bullet : bullet_list)
			{
				// 如果子弹无效或目标不是当前玩家，跳过碰撞检测
				if (!bullet->get_valid() || bullet->get_target() != id)
					continue;

				// 检查子弹是否与玩家发生碰撞
				if (bullet->check_collision(position, size))
				{
					make_invulnerable(); // 进入无敌状态，避免连续被同一子弹多次扣血
					bullet->on_collide();
					bullet->set_valid(false); // 碰撞后子弹失效
					hp -= bullet->get_damage(); // 扣除玩家生命值
				}
			}
		}
	}

protected:
	Vector2 position;  // 角色位置
	Vector2 size;      // 角色尺寸
	Vector2 velocity;  // 角色速度

	const float gravity = 1.6e-3f; // 重力加速度，单位为像素/毫秒^2 （修复：去掉空格，合法的浮点字面量）
	const float run_velocity = 0.5f; // 跑步速度，单位为像素/毫秒
	const float jump_velocity = -0.8f; // 跳跃初速度，单位为像素/毫秒

	Animation animation_idle_left;   // 向左默认动画
	Animation animation_idle_right;  // 向右默认动画
	Animation animation_run_left;    // 向左跑步动画
	Animation animation_run_right;   // 向右跑步动画
	Animation animation_attack_ex_left; // 大招向左攻击动画
	Animation animation_attack_ex_right; // 大招向右攻击动画

	Animation* current_animation = nullptr; // 当前动画指针

	Player_id id = Player_id::P1; // 玩家序号 ID
	 
	bool if_leftkey_down = false;  // 是否按下左键
	bool if_rightkey_down = false; // 是否按下右键

	bool is_facing_right = true; // 是否面向右边，默认为true

	int attack_cd = 500; // 普通攻击冷却时间，单位为毫秒
	bool can_attack = true; // 是否可以释放普通攻击
	Time timer_attack_cd; // 普通攻击冷却计时器

	bool is_attacking_ex = false; // 是否正在释放特殊攻击

	IMAGE img_sketch;
	bool is_invulnerable = false; // 是否处于无敌状态
	bool is_showing_sketch_frame = false; // 当前帧是否应该显示剪影
	Time timer_invulnerable; // 无敌状态计时器
	Time timer_invulnerable_blink; // 无敌状态闪烁计时器

	int hp = 100; // 角色生命值
	int mp = 0; // 角色魔法值
};
