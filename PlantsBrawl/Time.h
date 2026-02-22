#pragma once

#include <functional>

class Time
{
public:
	Time() = default;
	~Time() = default;

public:
	void restart() 
	{
		pass_time = 0;
		shotted = false;
	}

	// 设置定时器等待时间
	void set_wait_time(int time) 
	{
		wait_time = time;
	}

	// 是否单次触发的状态
	void set_one_shot(bool flag) 
	{
		one_shot = flag;
	}

	// 设置回调函数
	void set_callback(const std::function<void()>& callback) 
	{
		this->callback = callback;
	}

	// 计时器暂停
	void pause() 
	{
		paused = true;
	}

	// 计时器继续
	void resume() 
	{
		paused = false;
	}

	// 计时器更新逻辑
	void on_update(int delta) 
	{
		if (paused)
			return;
		pass_time += delta;
		if (pass_time >= wait_time)
		{
			if((!one_shot || (one_shot && !shotted) && callback))
				callback();
			shotted = true;
			pass_time = 0; // 重置已过时间
		}
	}



private:
	int pass_time = 0;      // 已过时间（毫秒）
	int wait_time = 0;      // 等待时间（毫秒）
	bool paused = false;    // 是否暂停
	bool shotted = false;   // 是否已触发
	bool one_shot = false;  // 是否单次触发
	std::function<void()> callback; // 回调函数
};