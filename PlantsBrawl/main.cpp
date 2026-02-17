#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <conio.h>

#include "Scene.h"
#include "MenuScene.h"

int main() 
{	
	// 01 初始化部分
	ExMessage msg;
	const int FPS = 60;

	initgraph(800, 600, EW_SHOWCONSOLE);
	BeginBatchDraw();

	Scene* scene = new MenuScene(); // 创建一个菜单场景实例

	scene->on_enter(); // 调用场景的进入函数

	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		// 02 读取操作
		while (peekmessage(&msg))
		{
			// 在这里处理消息，例如按键、鼠标移动等
			scene->on_input(msg); // 将消息传递给当前场景处理
		}

		// 03 处理数据
		scene->on_update(); // 更新场景逻辑

		// 04 渲染绘制
		cleardevice();

		FlushBatchDraw();
		scene->on_draw(); // 绘制当前场景


		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
		{
			Sleep(1000 / FPS - frame_delta_time);
		}
	}

	EndBatchDraw();
	closegraph(); // 关闭图形环境，释放资源
	return 0;
}