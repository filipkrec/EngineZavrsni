#pragma once
#include "../Math/Math.h"
#include "../Graphics/Sprite.h"
#define INSIDE = 0
#define	LEFT = 1
#define RIGHT = 2
#define BOTTOM = 4
#define TOP = 8


namespace objects {
	enum Shape {
		SQUARE = 1,
		CIRCLE = 2,
		CUSTOM = 3
	};

	class Hitbox {
	protected:
		Shape _shape;
		math::Vector2 _collisionRange;
		graphics::Sprite* _boundSprite;
	public:
		Hitbox();
		~Hitbox();

		Hitbox(const Hitbox& other);
		Hitbox(graphics::Sprite* sprite);
		Hitbox(graphics::Sprite* sprite, Shape shape, float width, float height = 0);
		Hitbox(graphics::Sprite* sprite, Shape shape, math::Vector2 collisionRange);

		inline const math::Vector2& getSpritePosition() const { return _boundSprite->getPosition(); }
		inline const math::Vector2& getSpriteSize() const { return _boundSprite->getSize(); }
		inline graphics::Sprite* getSprite() { return _boundSprite; }
		inline const math::Vector2& getCollisionRange() const { return _collisionRange; }

		bool isHit(const Hitbox& other) const;
		bool isHit(const math::Vector2& point) const;
		float distanceMinimum(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint) const;
		bool willBeHit(const Hitbox& other, const math::Vector2 nextMove) const;

		math::Vector4 getLineIntersection(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint);
		int computeCode(const math::Vector2& vectorOrigin);
	protected:
		friend class GameObject;
	};
}