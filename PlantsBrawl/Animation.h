#pragma once

#include <functional>
#include "Atlas.h"
#include "util.h"

class Animation
{
public:
	Animation() = default;
	~Animation() = default;
	
	void reset() // 重置动画状态
	{
		timer = 0;
		idx_frame = 0;
	}

	void set_atlas(Atlas* new_atlas) // 重置动画状态,设置动画图集
	{
		reset();
		atlas = new_atlas;
	}

	void set_loop(bool loop) // 设置是否循环
	{
		is_loop = loop;
	}

	void set_interval(int new_interval) // 设置帧间隔
	{
		interval = new_interval;
	}

	int get_idx_frame() const // 获取当前帧索引
	{
		return idx_frame;
	}

	IMAGE* get_frame()  // 获取当前帧图像
	{
		return atlas->get_image(idx_frame);
	}

	bool check_finished() const // 检查动画是否播放完成
	{
		if (is_loop) return false; // 循环动画永远不会完成
		return (idx_frame == atlas->get_size()-1); // 检查当前的帧索引是否已经到达图集的尾部
	}


	void on_update(int delta_time) // 更新动画状态
	{
		timer += delta_time; // 增加计时器
		if (timer >= interval) // 如果计时器超过帧间隔
		{
			timer = 0; // 重置计时器
			idx_frame++; // 切换到下一帧
			if (idx_frame >= atlas->get_size()) // 如果帧索引超过图集大小
			{
				if (is_loop)
					idx_frame = 0; // 循环回第一帧
				else
					idx_frame = atlas->get_size() - 1; // 停留在最后一帧
				if (callback && !is_loop) // 如果设置了回调函数且不是循环动画
					callback(); // 调用回调函数
			}
		}
	}

	void on_draw(int x, int y) // 绘制当前帧
	{
		putimage_alpha(x, y, atlas->get_image(idx_frame));
	}


	void set_callback(std::function<void()> callback) // 设置动画播放完成回调函数
	{
		this->callback = callback;
	}



private:
	int timer = 0; // 计时器
	int interval = 0; // 帧间隔
	int idx_frame = 0; // 帧索引
	bool is_loop = true; // 是否循环
	Atlas* atlas = nullptr; // 图集指针

	std::function<void()> callback; // 动画播放完成回调
};
