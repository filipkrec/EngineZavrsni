#include "Layer.h"
#include <algorithm>
namespace graphics {
	Layer::Layer(Shader shader, Renderer* renderer) :
		_shader(shader), _renderer(renderer)
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
	}
	Layer::~Layer() {
		_sprites.clear();
		std::vector<Sprite*>().swap(_sprites);

		std::vector<Sprite*>().swap(_renderingSprites);

		std::vector<Sprite*>().swap(_renderingSprites);
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

	void Layer::clear()
	{
		_sprites.clear();

		_labels.clear();

		_labelSprites.clear();

		_renderingSprites.clear();
	}

	void Layer::add(Label* label)
	{
		_labels.push_back(label);
	}

	void Layer::labelToSprite(Label* label)
	{
		const math::Vector2 position = label->getPosition();
		float posy = position.y;
		float posx = position.x;

		Font* currentFont = label->getFont();
		float fontSize = label->getSize();
		const math::Vector2& scale = currentFont->getScale() / fontSize;
		texture_font_t* FTFont = currentFont->getFont();
		std::string text = label->getText();
		Sprite* spriteLabel = new Sprite(*label);

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

				math::Vector2 symbolPos = math::Vector2((x0 + x1) / 2, (y0 + y1) / 2);

				float u0 = glyph->s0;
				float v0 = glyph->t0;
				float u1 = glyph->s1;
				float v1 = glyph->t1;

				Sprite* sprite = new Sprite();
				memcpy(sprite, spriteLabel, sizeof(Sprite));
				sprite->setScale(math::Vector2(fontSize, fontSize));
				sprite->setPosition(math::Vector2(symbolPos.x, symbolPos.y));
				sprite->setTextureCoordinates(math::Vector2(u0, v1), 0);
				sprite->setTextureCoordinates(math::Vector2(u0, v0), 1);
				sprite->setTextureCoordinates(math::Vector2(u1, v0), 2);
				sprite->setTextureCoordinates(math::Vector2(u1, v1), 3);

				_labelSprites.push_back(sprite);

				posx += glyph->advance_x / scale.x;
			}
		}
	}

	void Layer::render() {
		_shader.enable();
		_renderer->begin();
		//label se razbija na više spriteova s istom texturom ali razlicitim koordinatima
		if (_labels.empty() == false)
		{
			for (Label* label : _labels)
			{
				labelToSprite(label);
			}
		}
		std::vector<Sprite*>::iterator it;
		//spriteove labelova i ostali spriteovi se stavlaju u isti vektor, sortiraju i renderaju
		_sprites.erase(
			std::remove_if(_sprites.begin(), _sprites.end(),
				[](Sprite* x) {
					bool temp = x->toDestroySprite();
					if (temp)
					{
						delete x;
					}
					return temp;
				}),
			_sprites.end());
		_renderingSprites.insert(_renderingSprites.end(), _sprites.begin(), _sprites.end());
		_renderingSprites.insert(_renderingSprites.end(), _labelSprites.begin(), _labelSprites.end());
		std::sort(_renderingSprites.begin(), _renderingSprites.end(), sortSprite);
		for (Sprite* sprite : _renderingSprites) {
			_renderer->submit(sprite);
		}
		_renderer->end();
		_renderer->flush();
		//brišu se 'novonastali' spriteovi labele
		for (it = _labelSprites.begin(); it != _labelSprites.end();) {
			delete *it;
			it = _labelSprites.erase(it);
		}

		_labelSprites.clear();
		_renderingSprites.clear();
	}

	
}