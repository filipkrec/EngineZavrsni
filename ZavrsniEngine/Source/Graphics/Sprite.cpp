#include "Sprite.h"

namespace graphics {

	Sprite::Sprite(float x, float y, float width, float height, const unsigned int color)
		: _size(math::Vector2(width, height)), _color(color),_texture(nullptr)
	{
		_position[0] = math::Vector3(x, y, 0);
		_position[1] = math::Vector3(x, y + height, 0);
		_position[2] = math::Vector3(x + width, y + height, 0);
		_position[3] = math::Vector3(x + width, y, 0);
	}

	Sprite::Sprite(float x, float y, float width, float height, const Texture* texture)
		: _size(math::Vector2(width, height)), _color(0xffffffff), _texture(texture)
	{
		_textureCoordinates[0] = math::Vector2(0, 0);
		_textureCoordinates[1] = math::Vector2(0, 1);
		_textureCoordinates[2] = math::Vector2(1, 1);
		_textureCoordinates[3] = math::Vector2(1, 0);

		_position[0] = math::Vector3(x, y, 0);
		_position[1] = math::Vector3(x, y + height, 0);
		_position[2] = math::Vector3(x + width, y + height, 0);
		_position[3] = math::Vector3(x + width, y, 0);
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
		Vector3 temp = (Matrix4::rotation(degrees, axis) * _position[0]) - _position[0]; //trazenje pomaka s obzirom na sredinu 

		for (int i = 0; i < 4; ++i)
		{
			_position[i] += temp; //pomicanje svih vrhova za isti pomak
		}
	}

}