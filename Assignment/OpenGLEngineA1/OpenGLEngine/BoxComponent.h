#pragma once

namespace Reality
{
	struct BoxComponent
	{
		BoxComponent(float _xScale = 1, float _yScale = 1, float _zScale = 1)
			:xScale(_xScale), yScale(_yScale), zScale(_zScale){}
		float xScale;
		float yScale;
		float zScale;
	};
}