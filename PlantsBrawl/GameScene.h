#pragma once

#include <iostream>
#include <graphics.h>

#include "Scene.h"
#include "SceneManager.h"

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	    GameScene() = default;
		~GameScene() = default;

		void on_enter() override 
		{
			std::cout << "Entering Game Scene" << std::endl;
		};

		void on_update(int delta) override 
		{
			std::cout << "Game Scene Updating....." << std::endl;
		};

		void on_draw() override 
		{
			outtextxy(100, 100, _T("This is the Game Scene!"));
		};

		void on_input(const ExMessage& msg) override 
		{
			if (msg.message == WM_KEYDOWN)
			{
				scene_manager.switch_to(SceneManager::SceneType::Menu);
			}
		};

		void on_exit() override 
		{
			std::cout << "Exiting Game Scene" << std::endl;
		};

private:
};