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

extern std::vector<Platform> platform_list; // 声明平台列表，供Player类使用

class Player
{
public:
	Player()
	{
		current_animation = &animation_idle_right; // 默认朝右站立
	};

	~Player() = default;

	virtual void on_update(int delta) // 场景更新时调用，参数为时间增量（单位：毫秒）
	{
		int direction = if_rightkey_down - if_leftkey_down;  // 计算移动方向，右键按下为1，左键按下为-1，同时按下或都未按下为0

		if (direction != 0)
		{
			is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left; // 根据方向选择跑步动画
			float distance = run_velocity * delta * direction; // 计算移动距离
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left; // 根据方向选择默认动画
		}

		current_animation->on_update(delta); // 更新当前动画状态

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
			std::cout << "未处理的输入消息类型：" << msg.message << std::endl;
			break;
		};
	}; 

	virtual void on_draw(const Camera& camera) // 场景绘制时调用，参数为当前摄像机对象
	{
		current_animation->on_draw(camera, (int)position.x, (int)position.y); // 绘制当前动画的当前帧，位置根据角色位置计算
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
		position.x += distance; // 根据移动距离更新角色位置
	}

	void on_jump()
	{
		if (velocity.y !=0)
			return; // 如果垂直速度不为0，说明角色正在空中，不能再次跳跃
		velocity.y += jump_velocity; // 设置垂直速度为负值，使角色向上跳跃
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

	Animation* current_animation = nullptr; // 当前动画指针

	Player_id id = Player_id::P1; // 玩家序号 ID

	bool if_leftkey_down = false;  // 是否按下左键
	bool if_rightkey_down = false; // 是否按下右键

	bool is_facing_right = true; // 是否面向右边，默认为true
};
