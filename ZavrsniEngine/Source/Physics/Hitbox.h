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
		math::Vector2 _location;
		bool _spriteless;
	public:
		Hitbox();
		~Hitbox();

		Hitbox(const Hitbox& other);
		Hitbox(const math::Vector2 location, const math::Vector2 collisionRange);
		Hitbox(graphics::Sprite* sprite);
		Hitbox(graphics::Sprite* sprite, Shape shape, float width, float height = 0);
		Hitbox(graphics::Sprite* sprite, Shape shape, math::Vector2 collisionRange);

		inline const math::Vector2& getSpritePosition() const 
		{	
			if (!_spriteless) 
				return _boundSprite->getPosition(); 
			else return _location; 
		}
		inline const math::Vector2& getSpriteSize() const { return _boundSprite->getSize(); }
		inline graphics::Sprite* getSprite() { return _boundSprite; }
		inline const math::Vector2& getCollisionRange() const { return _collisionRange; }

		bool isHit(const Hitbox& other) const;
		bool isHit(const math::Vector2& point) const;
		bool willBeHit(const Hitbox& other, const math::Vector2 nextMove) const; //ako ce OVAJ biti pogoden od OTHER pomaknutog za NEXTMOVE

		math::Vector4 getLineIntersection(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint) const;
		int computeCode(const math::Vector2& vectorOrigin) const;
	protected:
		friend class GameObject;
	};
}