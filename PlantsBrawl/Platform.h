#pragma once

#include "Camera.h"
#include "util.h"

extern bool is_debug;

class Platform
{
public:
	struct CollisionShape
	{
		float y;
		float left;
		float right;
	};
public:
	CollisionShape shape;
	IMAGE* img = nullptr;
	POINT render_position = { 0, 0 };

public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, render_position.x, render_position.y, img);

		if (is_debug)
		{
			setlinecolor(RED);
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	};

private:

};