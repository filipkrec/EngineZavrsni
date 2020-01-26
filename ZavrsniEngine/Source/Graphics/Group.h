#pragma once

#include <vector>

#include "Sprite.h"

namespace graphics 
{
	class Group 
	{
		std::vector<Sprite*> _sprites;
	public:
		~Group();
		void applyTransformation(const math::Matrix4& transformation);
		void rotate(float degrees);
		void add(Sprite* sprite);
	protected:
		friend class Layer;
	};
	 
}