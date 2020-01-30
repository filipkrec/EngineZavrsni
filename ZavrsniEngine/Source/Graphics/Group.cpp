#include "Group.h"

namespace graphics
{
	void Group::applyTransformation(const math::Matrix4& transformation)
	{
		for (Sprite* sprite : _sprites)
		{
			for (int i = 0; i < 4; ++i)
			{
				sprite->_position[i] = transformation * sprite->_position[i];
			}
		}

		for (Label* label : _labels)
		{
			for (int i = 0; i < 4; ++i)
			{
				label->_position[i] = transformation * label->_position[i];
			}
		}
	}

	void Group::rotate(float degrees)
	{
		for (Sprite* sprite : _sprites)
		{
			sprite->Rotate(degrees);
		}
	}


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