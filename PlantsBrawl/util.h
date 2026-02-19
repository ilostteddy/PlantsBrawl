#pragma once

#include <graphics.h>
#include <windows.h>

#include "Atlas.h"


// 绘制带有 alpha 通道的图像
#pragma comment(lib, "Msimg32.lib")
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}



// 水平翻转图像
inline void flip_image(IMAGE* sre, IMAGE* dst)
{
	int width = sre->getwidth();
	int height = sre->getheight();
	Resize(dst, width, height);

	DWORD* src_buffer = GetImageBuffer(sre);
	DWORD* dst_buffer = GetImageBuffer(dst);	

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int idx_src = y * width + x;
			int idx_dst = y * width + (width - 1 - x);
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}