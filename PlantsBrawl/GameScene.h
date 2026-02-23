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
	
		};

		void on_update(int delta) override 
		{

		};

		void on_draw(const Camera& camera) override
		{

		};

		void on_input(const ExMessage& msg) override 
		{

		};

		void on_exit() override 
		{

		};

private:
};