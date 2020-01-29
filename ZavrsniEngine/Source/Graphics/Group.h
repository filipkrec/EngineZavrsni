#pragma once

#include <vector>

#include "Sprite.h"
#include "Label.h"

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
		void add(Label* label);
	protected:
		friend class Layer;
	};
	 
}