#pragma once

#include <graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() {};   // 场景进入时调用
	virtual void on_update() {};  // 处理数据时
	virtual void on_draw() {};    // 渲染绘制时调用
	virtual void on_input(const ExMessage& msg) {}; // 处理输入数据时调用
	virtual void on_exit() {};    // 场景退出时调用
private:

};