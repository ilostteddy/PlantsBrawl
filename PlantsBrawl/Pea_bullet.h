#pragma once

#include <iostream>

#include "Bullet.h"
#include "Animation.h"
#include "Resources.h"
#include "util.h"

class Pea_bullet : public Bullet
{
public:
    Pea_bullet()
    {
        size.x = 64; // 设置子弹宽度
        size.y = 64; // 设置子弹高度
		damage = 10; // 设置子弹伤害

        animation_break.set_atlas(&atlas_pea_break); // 设置破碎动画图集
        animation_break.set_loop(false); // 破碎动画不循环播放
		animation_break.set_interval(100); // 设置破碎动画帧间隔为100毫秒

        animation_break.set_callback([&]() // animation的回调函数
            {
                can_remove = true;
            }
        );
    };

	~Pea_bullet() = default;

    void on_collide() override
    {
        // 触发外部注册的回调（加 mp）
        if (callback)
            callback();

		set_valid(false); // 碰撞后子弹失效

        switch (rand() % 3)
        {
        case 0:
            mciSendString(_T("play pea_break_1 from 0"), NULL, 0, NULL);
            break;
        case 1:
            mciSendString(_T("play pea_break_2 from 0"), NULL, 0, NULL);
            break;
        case 2:
            mciSendString(_T("play pea_break_3 from 0"), NULL, 0, NULL);
            break;
        }
    }

    void on_update(int delta) override
    {
		position += velocity * (float)delta; // 根据速度和时间增量更新位置

		if (!valid) // 如果子弹无效，更新破碎动画状态
        {
            animation_break.on_update(delta);
        }

        if (check_if_exceeds_screen()) // 超出屏幕边界后子弹可以被移除
        {
			can_remove = true; 
		}
    }

    void on_draw(const Camera& camera) override
    {
        if (valid)
        {
            putimage_alpha(camera, (int)position.x, (int)position.y, &img_pea); // 绘制豌豆子弹
			std::cout << "豌豆子弹绘制" << std::endl;
        }
        else
        {
            animation_break.on_draw(camera, (int)position.x, (int)position.y); // 绘制破碎动画
        }
			
		Bullet::on_draw(camera); // 调用基类的调试绘制    
    }

private:
	Animation animation_break; // 豌豆子弹的破碎动画

};