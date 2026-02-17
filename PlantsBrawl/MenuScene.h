#pragma once

#include <graphics.h>
#include <iostream>
#include "scene.h"

class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter() override {
		std::cout << "Entering Menu Scene" << std::endl;
    };

    void on_update() override {
        std::cout << "Menu Scene Updating....." << std::endl;
    };

    void on_draw() override {
		outtextxy(100, 100, _T("Welcome to Plants Brawl!"));
    };

    void on_input(const ExMessage& msg) override {
    
    };

    void on_exit() override {
        std::cout << "Exiting Menu Scene" << std::endl;
    };

private:
	// 这里可以添加菜单场景特有的成员变量，例如按钮、背景图片等
};