#include "Label.h"

namespace graphics {

	Label::Label(const std::string& text, float x, float y, unsigned int color, float size, Font* font, float zindex)
		:Sprite(x,y,1,1,color, new Texture(font),zindex), _text(text), _font(font), _size(size)
	{
	}

	void Label::setText(const std::string& text)
	{
		_text = text;
	}

	Label::~Label()
	{
		DestroySprite();
	}
}