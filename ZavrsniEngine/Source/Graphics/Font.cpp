#include "Font.h"
#include <freetype-gl.h>

namespace graphics {
	Font::Font(std::string filename, unsigned int size)
	{
		_atlas = texture_atlas_new(512, 512, 2);
		_font = texture_font_new_from_file(_atlas, size, filename.c_str());
		texture_atlas_upload(_atlas);
	}

	void Font::setScale(float x, float y)
	{
		_scale = math::Vector2(x, y);
	}
}
