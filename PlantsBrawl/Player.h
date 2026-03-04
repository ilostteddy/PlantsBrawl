#pragma once

#include <graphics.h>
#include <iostream>

#include "Vector2.h"
#include "Camera.h"
#include "Player_id.h"
#include "Animation.h"

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
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left; // 根据方向选择默认动画
		}

		current_animation->on_update(delta); // 更新当前动画状态
	}; 

	virtual void on_input(const ExMessage& msg) // 场景输入事件时调用，参数为输入事件消息
	{
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
				}
				break;
			case Player_id::P2:
				switch (msg.vkcode)
				{
					// '←'键按下，P2向左移动
				case 0x25:
					if_leftkey_down = true;
					break;
					// '→'键按下，P2向右移动
				case 0x27:
					if_rightkey_down = true;
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
	}; 

	void set_id(Player_id id) // 设置玩家序号
	{
		this->id = id;
	}

	void set_position(float x, float y) // 设置角色位置
	{
		position.x = x;
		position.y = y;
	}

protected:
	Vector2 position;  // 角色位置

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
