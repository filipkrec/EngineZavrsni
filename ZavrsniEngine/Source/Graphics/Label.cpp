#include "Label.h"

namespace graphics {

	Label::Label(std::string text, float x, float y, unsigned int color, Font* font, float zindex)
		:Sprite(x,y,1,1,color,zindex), _text(text), _font(font)
	{
	}

	void Label::setText(std::string& text)
	{
		_text = text;
	}
}