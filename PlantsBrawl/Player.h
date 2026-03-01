#pragma once

#include <graphics.h>
#include "Camera.h"

class Player
{
public:
	Player() = default;
	~Player() = default;

	virtual void on_update(int delta) {}; // 场景更新时调用，参数为时间增量（单位：毫秒）

	virtual void on_input(const ExMessage& msg) {}; // 场景输入事件时调用，参数为输入事件消息

	virtual void on_draw(const Camera& camera) {}; // 场景绘制时调用，参数为当前摄像机对象
};
