#pragma once

#include <graphics.h>
#include <iostream>

#include "scene.h"
#include "SceneManager.h"
#include "Atlas.h"
#include "Animation.h"
#include "Camera.h"
#include "Time.h"
#include "Resources.h"

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter() override 
    {
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL); // 循环播放菜单音乐
    };

    void on_update(int delta) override 
    {

		
    };

    void on_draw(const Camera& camera) override
    {
        putimage_alpha(0, 0, &img_selector_background);
    };

    void on_input(const ExMessage& msg) override  
    {
        if (msg.message == WM_KEYUP)
        {
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL); // 播放确认音效
			scene_manager.switch_to(SceneManager::SceneType::Selector); // 切换到选角界面
        }
    };

    void on_exit() override {

    };

private:

	Animation anim_peashooter_run; // 豌豆射手奔跑动画
	Camera camera; // 摄像机对象，可以用于实现菜单场景的视角控制，例如背景滚动等
};