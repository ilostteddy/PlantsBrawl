#pragma once

#include "Bullet.h"
#include "Animation.h"

class Pea_bullet : public Bullet
{
public:
	Pea_bullet() = default;
	~Pea_bullet() = default;

    void on_collide() override
    {
        Bullet::on_collide();

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

    }

private:
	Animation animation_break; // 豌豆子弹的破碎动画

};