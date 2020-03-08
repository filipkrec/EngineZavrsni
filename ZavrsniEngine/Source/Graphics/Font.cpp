#include "Font.h"
#include <freetype-gl.h>

namespace graphics {
	Font::Font(const std::string& filename)
	{
		_atlas = texture_atlas_new(512, 512, 2);
		_font = texture_font_new_from_file(_atlas, 50, filename.c_str());
		float aspectRatioX = 16.0f;
		float aspectRatioY = 9.0f;
		float screenSizeX = 800.0f;
		float screenSizeY = 600.0f;
		setScale(screenSizeX / (aspectRatioX * 2), screenSizeY / (aspectRatioY * 2));
		texture_atlas_upload(_atlas);
	}

	void Font::setScale(float x, float y)
	{
		_scale = math::Vector2(x, y);
	}
}
