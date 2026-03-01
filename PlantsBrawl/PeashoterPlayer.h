#pragma once

#include <iostream>
#include "Player.h"

class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer() = default;
	~PeashooterPlayer() = default;

	void on_update(int delta_time) override
	{
		std::cout << "豌豆射手正在更新 " << delta_time << " ms" << std::endl;
	};

	void on_input(const ExMessage& msg) override
	{

	};

	void on_draw(const Camera& camera) override 
	{

	};
};