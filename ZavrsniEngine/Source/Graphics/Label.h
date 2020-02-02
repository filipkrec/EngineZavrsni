#pragma once

#include "Sprite.h"
#include "Font.h"

namespace graphics {
	class Label : public Sprite
	{
		std::string _text;
		Font* _font;
	public:
		Label(const std::string text, float x, float y, unsigned int color, Font* font, float zindex = 0.0f);
		~Label();

		inline std::string getText() const { return _text; }
		inline Font* getFont() const { return _font; }
		inline Font* getTextureId() const { return getTextureId(); }
		void setText(std::string& text);
	};
}