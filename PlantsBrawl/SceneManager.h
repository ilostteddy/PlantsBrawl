#pragma once

#include "Scene.h"

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	// 设置当前的场景
	void set_current_state(Scene* scene) 
	{
		current_scene = scene;
		current_scene->on_enter();
	};

	// 切换到指定类型的场景
	void switch_to(SceneType type)
	{
		current_scene->on_exit();
		switch (type)
		{
		case SceneType::Menu:
			break;
		case SceneType::Game:
			break;
		default:
			break;
		}
		current_scene->on_enter();
	}


private:
	Scene* current_scene = nullptr;

};