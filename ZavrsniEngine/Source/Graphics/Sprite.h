#pragma once
#include <vector>
#include "../Math/Math.h"
#include "Texture.h"

namespace graphics {
	class Sprite
	{
	protected:
		math::Vector2 _position; //center 
		math::Vector2 _size; //image width , height

		float _rotation;
		math::Vector2 _scale; //size * scale = display size
		math::Matrix4 _modelMatrix; //translation matrix * rotation matrix * scale matrix

		unsigned int _color;
		unsigned int  _zindex;
		const Texture* _texture;
		math::Vector2 _textureCoordinates[4];

		friend class Group;
	private:
	public:
		Sprite();
		~Sprite();
		Sprite(const Sprite& sprite);
		Sprite(float x, float y, float width, float height,  const unsigned int color, float zindex = 0.0f);
		Sprite(float x, float y, float width, float height, const Texture* texture, float zindex = 0.0f);
		Sprite(float x, float y, float width, float height, unsigned int color, const Texture* texture, float zindex);


		void setTextureCoordinates(const math::Vector2& value, int index);
		void setZindex(unsigned int zindex);
		void setColor(unsigned int color);

		void setPosition(const math::Vector2& value);
		void move(const math::Vector2& value);
		void setScale(const math::Vector2& scale);
		void rotate(float degrees);
		const Texture* swapTexture(const Texture* texture);

		void applyTransformations();

		inline const math::Vector2& getPosition() const { return _position; }
		inline const math::Matrix4& getModelMatrix() const { return _modelMatrix; }
		inline const float& getRotation() const { return _rotation; }
		inline const math::Vector2& getSize() const { return _size; }
		inline const unsigned int& getColor() const { return _color; }
		inline const unsigned int& getZindex() const { return _zindex; }
		inline const Texture* getTexture() const { return _texture; }
		inline const math::Vector2* getTextureCoordinates() const { return _textureCoordinates; }
	};
}