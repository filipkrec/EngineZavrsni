#pragma once

#include "Sprite.h"
#include "Font.h"

namespace graphics {
	class Label : public Sprite
	{
		std::string _text;
		Font* _font;
		float _size;
	public:
		Label(const std::string& text, float x, float y, unsigned int color, float size, Font* font, float zindex = 0.0f);
		~Label() override;

		inline const std::string& getText() const { return _text; }
		inline Font* getFont() const { return _font; }
		inline float getSize() const { return _size; }
		void setText(const std::string& text);
	};
}