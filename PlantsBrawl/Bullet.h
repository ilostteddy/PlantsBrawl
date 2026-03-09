#pragma once

#include <functional>
#include <graphics.h>

#include "Player_id.h"
#include "Vector2.h"
#include "Camera.h"

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;


	virtual void on_update(int delta_time) {};
	virtual void on_draw(const Camera& camera) {};

	virtual void on_collide()                             // 碰撞处理函数，子弹与目标碰撞时调用
	{
		if (callback)                                     // 如果设置了回调函数
		{
			callback();                                   // 调用回调函数，执行碰撞后的逻辑（如扣血、播放动画等）
		}
		valid = false;                                    // 碰撞后子弹失效
		can_remove = true;                                // 碰撞后子弹可以被移除
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	void set_damage(int val)
	{
		damage = val;                      
	}

	int get_damage()
	{
		return damage;
	}

	void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_velocity(float x, float y)
	{
		velocity.x = x;
		velocity.y = y;
	}

	void set_target(Player_id target)
	{
		target_id = target;
	}

	Player_id get_target() const
	{
		return target_id;
	}

	void set_callback(std::function<void()> callback)     // 设置碰撞回调函数
	{
		this->callback = callback;
	}

	void set_valid(bool is_valid)                         // 设置子弹有效性
	{
		valid = is_valid;
	}

	bool get_valid() const                                // 获取子弹有效性
	{
		return valid;
	}

	bool check_can_remove() const                         // 检查子弹是否可以被移除
	{
		return can_remove;
	}



protected:
	Vector2 position;                                     // 子弹位置
	Vector2 size;                                         // 子弹尺寸
	Vector2 velocity;                                     // 子弹速度

	int damage = 10;                                      // 子弹伤害	

	bool valid = true;                                    // 子弹是否有效（未碰撞或未超出边界）
	bool can_remove = true;                               // 子弹是否可以被移除（碰撞后或超出边界）

	std::function<void()> callback;                       // 碰撞回调函数，子弹与目标碰撞时调用

	Player_id target_id = Player_id::P1;			      // 子弹的目标玩家ID，默认为P1

protected:
	bool check_if_exceeds_screen()
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};