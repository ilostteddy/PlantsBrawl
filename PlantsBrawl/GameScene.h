#pragma once

#include "Scene.h"
#include <iostream>
#include <graphics.h>	

class GameScene : public Scene
{
public:
	    GameScene() = default;
		~GameScene() = default;

		void on_enter() override {
			std::cout << "Entering Game Scene" << std::endl;
		};

		void on_update() override {
			std::cout << "Game Scene Updating....." << std::endl;
		};

		void on_draw() override {
			outtextxy(100, 100, _T("This is the Game Scene!"));
		};

		void on_input(const ExMessage& msg) override {
		
		};

		void on_exit() override {
			std::cout << "Exiting Game Scene" << std::endl;
		};

private:


};