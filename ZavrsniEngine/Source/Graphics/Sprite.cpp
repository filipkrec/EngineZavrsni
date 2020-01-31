#include "Sprite.h"

namespace graphics {

	Sprite::Sprite() {}

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

	void Sprite::setPosition(const math::Vector2& value,const int& index) 
	{
		_position[index] = value;
	}

	void Sprite::setTextureCoordinates(const math::Vector2& value, const int& index)
	{
		_textureCoordinates[index] = value;
	}

}