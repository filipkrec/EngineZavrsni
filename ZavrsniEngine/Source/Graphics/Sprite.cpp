#include "Sprite.h"

namespace graphics {

	Sprite::Sprite()
		:_size(math::Vector2(0, 0)), _color(0), _zindex(0), _texture(nullptr) 
		{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 0);
		_textureCoordinates[2] = math::Vector2(0, 0);
		_textureCoordinates[3] = math::Vector2(0, 0);

		_position[0] = math::Vector2(0, 0);
		_position[1] = math::Vector2(0, 0);
		_position[2] = math::Vector2(0, 0);
		_position[3] = math::Vector2(0, 0);
	}
	
	Sprite::~Sprite()
	{
	}

	Sprite::Sprite(const Sprite* sprite)
		: _size(sprite->getSize()),_color(sprite->getColor()),_texture(sprite->getTexture()),_zindex(sprite->getZindex())
	{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 0.9f);
		_textureCoordinates[2] = math::Vector2(0.9f, 0.9f);
		_textureCoordinates[3] = math::Vector2(0.9f, 0);
		
		float x = sprite->getPosition()->x;
		float y = sprite->getPosition()->y;
		_position[0] = math::Vector2(x, y);
		_position[1] = math::Vector2(x, y + _size.y);
		_position[2] = math::Vector2(x + _size.x, y + _size.y);
		_position[3] = math::Vector2(x + _size.x, y);
	}

	Sprite::Sprite(float x, float y, float width, float height, const unsigned int color, float zindex)
		: _size(math::Vector2(width, height)), _color(color), _texture(nullptr), _zindex(zindex)
	{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 0.9f);
		_textureCoordinates[2] = math::Vector2(0.9f, 0.9f);
		_textureCoordinates[3] = math::Vector2(0.9f, 0);

		_position[0] = math::Vector2(x, y);
		_position[1] = math::Vector2(x, y + height);
		_position[2] = math::Vector2(x + width, y + height);
		_position[3] = math::Vector2(x + width, y);
	}

	Sprite::Sprite(float x, float y, float width, float height, const Texture* texture, float zindex)
		: _size(math::Vector2(width, height)), _color(0xffffffff), _texture(texture), _zindex(zindex)
	{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1);
		_textureCoordinates[2] = math::Vector2(1, 1);
		_textureCoordinates[3] = math::Vector2(1, 0);

		_position[0] = math::Vector2(x, y);
		_position[1] = math::Vector2(x, y + height);
		_position[2] = math::Vector2(x + width, y + height);
		_position[3] = math::Vector2(x + width, y);
	}

	Sprite::Sprite(float x, float y, float width, float height, unsigned int color, const Texture* texture, float zindex)
		: _size(math::Vector2(width, height)), _color(color), _texture(texture), _zindex(zindex)
	{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1);
		_textureCoordinates[2] = math::Vector2(1, 1);
		_textureCoordinates[3] = math::Vector2(1, 0);

		_position[0] = math::Vector2(x, y);
		_position[1] = math::Vector2(x, y + height);
		_position[2] = math::Vector2(x + width, y + height);
		_position[3] = math::Vector2(x + width, y);
	}


	void Sprite::Rotate(float degrees)
	{
		using namespace math;
		Vector3 axis(0, 0, 1);
		
		for (int i = 0; i < 4; ++i)
		{
			_textureCoordinates[i] = _textureCoordinates[i] - Vector2(0.5,0.5); //postavljanje tocaka oko centra kako bi se mogli rotirati oko svoje osi
		}

		for (int i = 0; i < 4; ++i)
		{
			 Vector3 temp = Matrix4::rotation(degrees, axis) * Vector3(_textureCoordinates[i].x, _textureCoordinates[i].y, 0);
			 _textureCoordinates[i] = Vector2(temp.x, temp.y) + Vector2(0.5, 0.5); //rotacija oko osi + vracanje van centra unutar granica koordinata teksture
		}
	}

	void Sprite::RotatePosition(float degrees)
	{
		using namespace math;
		Vector3 axis(0, 0, 1);
		Vector2 temp = (Matrix4::rotation(degrees, axis) * _position[0]) - _position[0]; //trazenje pomaka s obzirom na sredinu 

		for (int i = 0; i < 4; ++i)
		{
			_position[i] += temp; //pomicanje svih vrhova za isti pomak
		}
	}

	void Sprite::setPosition(const math::Vector2& value,int index) 
	{
		if(index != 5)
		_position[index] = value;
		else
		{
			_position[0] = math::Vector2(value.x, value.y);
			_position[1] = math::Vector2(value.x, value.y + _size.x);
			_position[2] = math::Vector2(value.x + _size.x, value.y + _size.y);
			_position[3] = math::Vector2(value.x + _size.x, value.y);
		}
	}

	void Sprite::setTextureCoordinates(const math::Vector2& value, int index)
	{
		_textureCoordinates[index] = value;
	}


	void Sprite::setColor(unsigned int color)
	{
		_color = color;
	}

	void Sprite::applyTransformation(const math::Matrix4& transformation)
	{
		for (int i = 0; i < 4; ++i)
		{
			_position[i] = transformation * _position[i];
		}
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
}