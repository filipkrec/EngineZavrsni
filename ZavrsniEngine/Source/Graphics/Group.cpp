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
		const math::Vector2* position = label->getPosition();
		float posy = position->y;
		float posx = position->x;

		Font* currentFont = label->getFont();
		const math::Vector2& scale = currentFont->getScale();
		texture_font_t* FTFont = currentFont->getFont();
		std::string text = label->getText();

		for (int i = 0; i < text.size(); ++i)
		{
			char c = text.at(i);
			texture_glyph_t* glyph = texture_font_get_glyph(FTFont, c);
			if (glyph != NULL)
			{
				if (i > 0) {
					float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
					posx += kerning / scale.x;
				}


				float x0 = posx + glyph->offset_x / scale.x;
				float y0 = posy + glyph->offset_y / scale.y;
				float x1 = x0 + glyph->width / scale.x;
				float y1 = y0 - glyph->height / scale.y;

				float u0 = glyph->s0;
				float v0 = glyph->t0;
				float u1 = glyph->s1;
				float v1 = glyph->t1;

				Sprite* spriteTemp = (Sprite*)label;
				Sprite* sprite = new Sprite();
				memcpy(sprite, spriteTemp, sizeof(Sprite));
				sprite->setPosition(math::Vector2(x0, y0), 0);
				sprite->setPosition(math::Vector2(x0, y1), 1);
				sprite->setPosition(math::Vector2(x1, y1), 2);
				sprite->setPosition(math::Vector2(x1, y0), 3);
				sprite->setTextureCoordinates(math::Vector2(u0, v0), 0);
				sprite->setTextureCoordinates(math::Vector2(u0, v1), 1);
				sprite->setTextureCoordinates(math::Vector2(u1, v1), 2);
				sprite->setTextureCoordinates(math::Vector2(u1, v0), 3);

				add(sprite);

				posx += glyph->advance_x / scale.x;
			}
		}
	}

	Group::~Group()
	{
		for (int i = 0; i < _sprites.size(); ++i)
		{
			delete _sprites[i];
		}
	}
}