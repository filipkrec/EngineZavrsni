#pragma once

#include <string>
#include <freetype-gl.h>
#include "../Math/Vector2.h"

namespace graphics {
	class Font
	{
	private:
		texture_atlas_t* _atlas;
		texture_font_t* _font;
		float _size;

		math::Vector2 _scale;
	public:
		Font(const std::string& filename,  unsigned int size);

		void setScale(float x, float y);
		void setSize(float size);

		inline texture_font_t* getFont() const { return _font; }
		inline const math::Vector2& getScale() const { return _scale; }
		inline const float getSize() const { return _size; }
		inline unsigned int getId() const { return _atlas->id; }
	};
}