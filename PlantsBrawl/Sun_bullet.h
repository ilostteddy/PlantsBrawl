#pragma once

#include "Bullet.h"
#include "Animation.h"
#include "Resources.h"
#include "Camera.h"

extern Camera main_camera;

class Sun_bullet : public Bullet
{
public:
	Sun_bullet() 
	{
		size.x = 96; // 设置子弹宽度
		size.y = 96; // 设置子弹高度

		damage = 20; // 设置子弹伤害

		animation_idle.set_atlas(&atlas_sun); 
		animation_idle.set_interval(50); 
		
		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]()
			{
				can_remove = true;
			}
		);

		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		explosion_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2; // 爆炸动画相对于默认动画的水平偏移
		explosion_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2; // 爆炸动画相对于默认动画的垂直偏移
	};

	~Sun_bullet() = default;

	void on_collide() override
	{
		Bullet::on_collide();

		main_camera.shake(5, 250); // 碰撞时触发摄像机震动，强度为5，持续300毫秒

		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta) override
	{
		if (valid)
		{
			velocity.y += gravity * (float)delta; // 根据速度和时间增量更新位置
			position += velocity * (float)delta; // 根据速度和时间增量更新位置
		}
		
		if (!valid) // 如果子弹无效，更新爆炸动画状态
			animation_explode.on_update(delta);
		else
			animation_idle.on_update(delta);

		if (check_if_exceeds_screen()) // 超出屏幕边界后子弹可以被移除
			can_remove = true;
	}

	void on_draw(const Camera& camera) override
	{
		if (valid)
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);
		else
			animation_explode.on_draw(camera, 
				(int)(position.x + explosion_render_offset.x), 
				(int)(position.y + explosion_render_offset.y));
	};

private:
	Animation animation_idle; // 默认动画
	Animation animation_explode; // 碰撞爆炸销毁动画
	Vector2 explosion_render_offset; // 爆炸动画渲染偏移，用来对齐两个动画的中心

	const float gravity = 1e-3f; // 换算为0.001像素/毫秒^2的重力加速度
};