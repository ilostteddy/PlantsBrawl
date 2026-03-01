#pragma once

#include <iostream>
#include "Player.h"

class SunflowerPlayer : public Player
{
public:
	SunflowerPlayer() = default;
	~SunflowerPlayer() = default;

	void on_update(int delta_time) override
	{
		std::cout << "向日葵正在更新 " << delta_time << " ms" << std::endl;
	};

	void on_input(const ExMessage& msg) override
	{

	};

	void on_draw(const Camera& camera) override
	{

	};
};