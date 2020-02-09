#include "Group.h"

namespace graphics
{
	void Group::add(Sprite* sprite)
	{
		_sprites.push_back(sprite);
	}

	void Group::add(Label* label)
	{
		_labels.push_back(label);
	}

	Group::~Group()
	{
		for (int i = 0; i < _sprites.size(); ++i)
		{
			delete _sprites[i];
		}
	}
}