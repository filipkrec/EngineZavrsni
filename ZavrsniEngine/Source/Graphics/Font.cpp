#include "Font.h"
#include <freetype-gl.h>

namespace graphics {
	Font::Font(const std::string& filename, unsigned int size)
	{
		_atlas = texture_atlas_new(512, 512, 2);
		_font = texture_font_new_from_file(_atlas, 50, filename.c_str());
		setScale(800.0f / 32.0f, 600.0f / 18.0f);
		setSize(1.0f);
		texture_atlas_upload(_atlas);
	}

	void Font::setScale(float x, float y)
	{
		_scale = math::Vector2(x, y);
	}

	void Font::setSize(float size)
	{
		_size = size;
	}
}
