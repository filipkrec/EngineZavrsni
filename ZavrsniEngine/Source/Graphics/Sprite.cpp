#include "Sprite.h"

namespace graphics {

	Sprite::Sprite()
		:_position(math::Vector2(0, 0)), _offset(math::Vector2(0, 0)), _size(math::Vector2(0, 0)), _color(0), _zindex(0), _texture(nullptr), _rotation(0.0f), _scale(1.0f,1.0f), _modelMatrix(1.0f), _destroySprite(false)
	{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 0);
		_textureCoordinates[2] = math::Vector2(0, 0);
		_textureCoordinates[3] = math::Vector2(0, 0);
		applyTransformations();
	}
	
	Sprite::~Sprite()
	{
	}

	Sprite::Sprite(const math::Vector2 lineBegin, const math::Vector2 lineEnd)
		:_zindex(100), _destroySprite(false)
	{
		_isLine = true;
		_position = lineBegin;
		_size = lineEnd;
		_rotation = 500;
	}

	Sprite::Sprite(const Sprite& sprite)
		: _position(sprite.getPosition()), _offset(sprite.getOffset()), _size(sprite.getSize()),_color(sprite.getColor()),_texture(sprite.getTexture()),_zindex(sprite.getZindex()), _rotation(sprite._rotation), _scale(sprite._scale), _modelMatrix(sprite._modelMatrix), _destroySprite(false)
	{
		_isLine = false;
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1.0f);
		_textureCoordinates[2] = math::Vector2(1.0f, 1.0f);
		_textureCoordinates[3] = math::Vector2(1.0f, 0);
		applyTransformations();
	}

	Sprite::Sprite(float x, float y, float width, float height, const unsigned int color, float zindex)
		: _position(math::Vector2(x, y)),_size(math::Vector2(width, height)), _color(color), _texture(nullptr), _zindex(zindex), _rotation(0.0f), _scale(1.0f, 1.0f), _modelMatrix(1.0f), _destroySprite(false)
	{
		_isLine = false;
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1.0f);
		_textureCoordinates[2] = math::Vector2(1.0f, 1.0f);
		_textureCoordinates[3] = math::Vector2(1.0f, 0);
		applyTransformations();
	}

	Sprite::Sprite(float x, float y, float width, float height, const Texture* texture, float zindex, const math::Vector2& offset)
		: _position(math::Vector2(x, y)), _size(math::Vector2(width, height)),_offset(offset), _color(0xffffffff), _texture(texture), _zindex(zindex), _rotation(0.0f), _scale(1.0f, 1.0f), _modelMatrix(1.0f), _destroySprite(false)
	{
		_isLine = false;
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1.0f);
		_textureCoordinates[2] = math::Vector2(1.0f, 1.0f);
		_textureCoordinates[3] = math::Vector2(1.0f, 0);
		applyTransformations();
	}

	Sprite::Sprite(float x, float y, float width, float height, unsigned int color, const Texture* texture, float zindex)
		: _position(math::Vector2(x, y)), _size(math::Vector2(width, height)), _color(color), _texture(texture), _zindex(zindex), _rotation(0.0f), _scale(1.0f, 1.0f), _modelMatrix(1.0f), _destroySprite(false)
	{
		_isLine = false;
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1.0f);
		_textureCoordinates[2] = math::Vector2(1.0f, 1.0f);
		_textureCoordinates[3] = math::Vector2(1.0f, 0);
		applyTransformations();
	}

	void Sprite::setPosition(const math::Vector2& value) 
	{
		_position = value;
		applyTransformations();
	}

	void Sprite::setOffset(const math::Vector2& value)
	{
		_offset = value;
	}

	void Sprite::move(const math::Vector2& value)
	{
		_position += value;
		applyTransformations();
	}

	void Sprite::rotate(float degrees)
	{
		_rotation = fmod(_rotation + degrees, 360);
		applyTransformations();
	}

	void Sprite::setRotation(float degrees)
	{
		_rotation = fmod(degrees, 360);
		applyTransformations();
	}

	void Sprite::setScale(const math::Vector2& scale)
	{
		_scale = scale;
		applyTransformations();
	}

	void Sprite::setTextureCoordinates(const math::Vector2& value, int index)
	{
		_textureCoordinates[index] = value;
	}


	void Sprite::setColor(unsigned int color)
	{
		_color = color;
	}

	void Sprite::setZindex(unsigned int zindex)
	{
		_zindex = zindex;
	}

	void Sprite::applyTransformations()
	{
		_modelMatrix = math::Matrix4::translation(_position) * math::Matrix4::rotation(_rotation, math::Vector3(0, 0, 1)) * math::Matrix4::scale(_scale);
	}

	void Sprite::setModelMatrix(const math::Matrix4& modelMatrix)
	{
		_modelMatrix = modelMatrix;
	}

	const Texture* Sprite::swapTexture(const Texture* texture)
	{
		if (texture != _texture)
		{
			const Texture* textureOld = _texture;
			_texture = texture;
			return textureOld;
		}
		return _texture;
	}

	void Sprite::DestroySprite()
	{
		_destroySprite = true;
	}

	bool Sprite::toDestroySprite()
	{
		return _destroySprite;
	}


	void Sprite::DoNotDestroySprite()
	{
		_destroySprite = false;
	}
}