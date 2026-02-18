#pragma once

#include <vector>
#include <graphics.h>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, int num)
	{
		img_list.clear();		// clear()清空 vector 中的所有元素
		img_list.resize(num);	// resize(num) 重新设置 vector 的大小为 num

		TCHAR path_file[256];
		for (int i = 0; i < num; ++i)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&img_list[i], path_file);
		}
	}

	void clear() // 清空图集中已经加载的图像
	{
		img_list.clear();
	}

	int get_size()
	{
		return (int)img_list.size();
	}

	IMAGE* get_image(int index)
	{
		if (index < 0 || index >= (int)img_list.size())
			return nullptr; // 返回 nullptr 表示索引无效
		return &img_list[index];
	}

	void add_image(const IMAGE& img) // 主要用来生成水平翻转的动画图集
	{
		img_list.push_back(img);
	}

private:
	// 定义IMAGE vector
	std::vector<IMAGE> img_list;
};