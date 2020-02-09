#pragma once
#include "../Math/Math.h"
#include "../Graphics/Sprite.h"

namespace physics {
	enum Shape {
		SQUARE = 1,
		CIRCLE = 2,
		CUSTOM = 3
	};

	class Hitbox {
		Shape _shape;
		math::Vector2 _collisionRange;
		graphics::Sprite _boundSprite;
	public:
		Hitbox();
		~Hitbox();
		Hitbox(const graphics::Sprite& sprite, Shape shape, float width, float height = 0);

		inline const math::Vector2& getSpritePosition() const { return _boundSprite.getPosition(); }
		inline const math::Vector2& getSpriteSize() const { return _boundSprite.getSize(); }
		inline graphics::Sprite& getSprite() { return _boundSprite; }
		inline const math::Vector2& getCollisionRange() const { return _collisionRange; }

		bool isHit(const Hitbox& other) const;
		bool isHit(const math::Vector2& point) const;
	protected:
		friend class GameObject;
	};
}