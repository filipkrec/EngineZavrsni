#pragma once

#include <vector>

#include "Sprite.h"
#include "Label.h"

namespace graphics 
{
	class Group 
	{
		std::vector<Sprite*> _sprites;
		std::vector<Label*> _labels;
	public:
		~Group();
		void add(Sprite* sprite);
		void add(Label* label);
	protected:
		friend class Layer;
	};
	 
}