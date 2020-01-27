#pragma once

#include <string>
#include "../../../External/freetype-gl/freetype-gl.h"
#include "../Math/Vector2.h"

namespace graphics {
	class Font
	{
	private:
		texture_atlas_t* _atlas;
		texture_font_t* _font;

		math::Vector2 _scale;
	public:
		Font(std::string filename, std::string name, unsigned int size);

		void setScale(float x, float y);

		inline texture_font_t* getFont() const { return _font; }
		inline const math::Vector2& getScale() const { return _scale; }
		inline unsigned int getId() const { return _atlas->id; }
	};
}