#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <conio.h>

#pragma comment(lib, "winmm.lib")

#include "util.h"
#include "Atlas.h"
#include "Platform.h"
#include "Scene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "SelectorScene.h"
#include "Resources.h"  // 包含资源定义
#include "Vector2.h"
#include "Player.h"
#include "Bullet.h"

bool is_debug = false;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

Player* player_1 = nullptr;
Player* player_2 = nullptr;
IMAGE* img_player_1_avatar = nullptr;
IMAGE* img_player_2_avatar = nullptr;

SceneManager scene_manager;

Camera main_camera;

std::vector<Platform> platform_list;
std::vector<Bullet*> bullet_list;

int main() 
{	
	// 切换控制台到 UTF-8
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// 01 初始化部分
	ExMessage msg;
	const int FPS = 60;

	initgraph(1280, 720);

	load_game_resources(); // 加载游戏资源，例如图片、动画等

	settextstyle(24, 0, _T("Flx")); // 设置全局文本样式，使用加载的 Flx 字体
	setbkmode(TRANSPARENT); // 设置文本背景模式为透明

	BeginBatchDraw(); // 开始批量绘制模式，可以提高绘制效率，减少闪烁

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.set_current_scene(menu_scene);

	while (true)
	{
		DWORD frame_start_time = GetTickCount();   // 获取当前时间戳，单位为毫秒

		// 02 读取操作
		while (peekmessage(&msg))
		{
			// 在这里处理消息，例如按键、鼠标移动等
			scene_manager.on_input(msg); // 将消息传递给当前场景处理
		}

		// 03 处理数据
		static DWORD last_tick_time = GetTickCount();  // 关键：static修饰的变量
		DWORD current_tick_time = GetTickCount();  // 获取当前时间戳
		DWORD delta_tick = current_tick_time - last_tick_time; // 计算与上次更新的时间差

		scene_manager.on_update(delta_tick); // 更新场景逻辑

		last_tick_time = current_tick_time; // 更新上次更新的时间戳

		// 04 渲染绘制
		cleardevice();  // 清空画布
		scene_manager.on_draw(main_camera); // 绘制当前场景
		FlushBatchDraw(); // 刷新绘制内容到屏幕

		
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