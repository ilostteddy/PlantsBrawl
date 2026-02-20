#pragma once

#include <cmath>
#include "Vector2.h"

class Camera
{
public:
	Camera() = default;
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
		Vector2 speed = { -0.35, 0 }; // 摄像机移动速度，单位为像素/毫秒
		position += speed * delta; // 根据时间增量更新摄像机位置
	}


private:
    Vector2 position; // 摄像机位置
};