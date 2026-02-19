#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <conio.h>

#pragma comment(lib, "winmm.lib")

#include "util.h"
#include "Atlas.h"
#include "Scene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "SelectorScene.h"
#include "Resources.h"  // 包含资源定义

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

SceneManager scene_manager;


int main() 
{	
	// 01 初始化部分
	ExMessage msg;
	const int FPS = 60;

	load_game_resources(); // 加载游戏资源，例如图片、动画等

	initgraph(800, 600, EW_SHOWCONSOLE);
	BeginBatchDraw();

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.set_current_scene(menu_scene);

	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		// 02 读取操作
		while (peekmessage(&msg))
		{
			// 在这里处理消息，例如按键、鼠标移动等
			scene_manager.on_input(msg); // 将消息传递给当前场景处理
		}

		// 03 处理数据
		scene_manager.on_update(); // 更新场景逻辑

		// 04 渲染绘制
		cleardevice();

		FlushBatchDraw();
		scene_manager.on_draw(); // 绘制当前场景


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