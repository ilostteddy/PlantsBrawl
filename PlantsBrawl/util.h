#pragma once

#include <graphics.h>
#include <windows.h>

#include "Atlas.h"
#include "Camera.h"
#include "Vector2.h"


// 绘制带有 alpha 通道的图像
#pragma comment(lib, "Msimg32.lib")
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

// 带摄像机的putimage_alpha重载版本
inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	const Vector2& pos_camera = camera.get_position();
	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - pos_camera.x), (int)(dst_y - pos_camera.y), 
		w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

// 裁剪渲染功能的putimage_alpha重载版本
inline void putimage_alpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y)
{
	int w = width > 0 ? width : img->getwidth();
	int h = height > 0 ? height : img->getheight();
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
		GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
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