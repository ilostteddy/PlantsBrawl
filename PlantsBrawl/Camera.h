#pragma once

#include <cmath>
#include "Vector2.h"
#include "Time.h"

class Camera
{
public:
	Camera()
	{
		timer_shake.set_one_shot(true); // 设置为单次触发
		timer_shake.set_callback([this]() 
			{
			is_shaking = false; // 震动结束后重置状态
			reset_position();
			});
	};

	~Camera() = default;

	const Vector2& get_position() const
	{
		return position;
	}

	void reset_position()
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		timer_shake.on_update(delta); // 更新震动定时器状态

		if (is_shaking)
		{
			// 在抖动强度为半径的圆内进行随机运动
			position.x = (-50 + rand() % 100) / 50.0f * shake_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shake_strength;
		}
	}

	void shake(float strength, int duration)
	{
		is_shaking = true;  // 开始震动
		shake_strength = strength;  // 设置震动强度

		timer_shake.set_wait_time(duration);  // 设置震动持续时间
		timer_shake.restart();  // 重启定时器，开始计时
	}

private:
    Vector2 position; // 摄像机位置

	Time timer_shake; // 震动定时器
	bool is_shaking = false; // 是否正在震动
	float shake_strength = 0; // 震动强度
};