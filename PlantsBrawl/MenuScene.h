#pragma once

#include <graphics.h>
#include <iostream>

#include "scene.h"
#include "SceneManager.h"
#include "Atlas.h"
#include "Animation.h"
#include "Camera.h"
#include "Time.h"

extern SceneManager scene_manager;
extern Atlas atlas_peashooter_run_right;
Time time_test;

class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter() override 
    {
		std::cout << "Entering Menu Scene" << std::endl;
		time_test.set_wait_time(3000); // 设置等待时间为3000毫秒（3秒）
		time_test.set_one_shot(false); // 设置为多次触发
        time_test.set_callback(
            []() {
                std::cout << "Time callback triggered" << std::endl;
            }
        );

        anim_peashooter_run.set_atlas(&atlas_peashooter_run_right);
        anim_peashooter_run.set_loop(true);
		anim_peashooter_run.set_interval(100); // 设置帧间隔为100毫秒
        anim_peashooter_run.set_callback(
            []() {
                scene_manager.switch_to(SceneManager::SceneType::Game);
            }
        );
    };

    void on_update(int delta) override 
    {
        camera.on_update(delta); // 更新摄像机位置
		time_test.on_update(delta); // 更新定时器状态

        anim_peashooter_run.on_update(delta);
		
    };

    void on_draw() override 
    {
		outtextxy(100, 100, _T("Welcome to Plants Brawl!"));

		const Vector2& cam_pos = camera.get_position(); // 获取摄像机位置

		anim_peashooter_run.on_draw(200 - cam_pos.x, 200 - cam_pos.y); // 根据摄像机位置调整动画绘制位置
    };

    void on_input(const ExMessage& msg) override  
    {
        if (msg.message == WM_KEYDOWN)
        {
			// 当按下任意键时，摄像机开始抖动
			camera.shake(5.0f, 500); // 设置震动强度为5.0，持续时间为500毫秒
        }
    };

    void on_exit() override {
        std::cout << "Exiting Menu Scene" << std::endl;
    };

private:
	// 这里可以添加菜单场景特有的成员变量，例如按钮、背景图片等
	Animation anim_peashooter_run; // 豌豆射手奔跑动画
	Camera camera; // 摄像机对象，可以用于实现菜单场景的视角控制，例如背景滚动等
};