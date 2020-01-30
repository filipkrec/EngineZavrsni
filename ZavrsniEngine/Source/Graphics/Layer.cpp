#include "Layer.h"
#include <algorithm>
namespace graphics {
	Layer::Layer(math::Matrix4 projectionMatrix, Shader shader, Renderer* renderer) :
		_shader(shader), _renderer(renderer), _projectionMatrix(projectionMatrix)
	{
		_shader.enable();
		int textureIds[] =
		{
			 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31
		};
		_shader.setUniform1iv("textures", 32, textureIds); //textureId's popunjeni za sampler sa maximalnim brojam sampleova (32 za openGL)
		_shader.setUniformMat4("projection_matrix", _projectionMatrix);
	}
	Layer::~Layer() {
		for (int i = 0; i < _sprites.size(); ++i)
		{
			delete _sprites[i];
		}
	}

	bool sortSprite(Sprite* first, Sprite* second)
	{
		return (first->getZindex() < second->getZindex());
	}

	void Layer::add(Sprite* renderable) {
		_sprites.push_back(renderable);
	}

	void Layer::add(const Group& group)
	{
		for (Sprite* sprite : group._sprites)
		{
			_sprites.push_back(sprite);
		}
		for (Label* label : group._labels)
		{
			_labels.push_back(label);
		}
	}

	void Layer::add(Label* label)
	{
		_labels.push_back(label);
	}

	void Layer::labelToSprite(Label* label)
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

				_renderingSprites.push_back(sprite);

				posx += glyph->advance_x / scale.x;
			}
		}
	}

	void Layer::render() {
		_shader.enable();
		_renderer->begin();
		if (_labels.empty() == false)
		{
			for (Label* label : _labels)
			{
				labelToSprite(label);
			}
		}
		_renderingSprites.insert(_renderingSprites.end(), _sprites.begin(), _sprites.end());
		std::sort(_renderingSprites.begin(), _renderingSprites.end(), sortSprite);
		for (const Sprite* sprite : _renderingSprites) {
			_renderer->submit(sprite);
		}
		_renderer->end();
		_renderer->flush();
		_renderingSprites.clear();
	}

	
}