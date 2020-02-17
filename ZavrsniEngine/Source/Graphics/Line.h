#pragma once
#include "Sprite.h"

namespace graphics {
	class Line : public Sprite
	{
	protected:
		math::Vector2 _originPoint; //center 
		math::Vector2 _destinationPoint; //image width , height
		unsigned int _duration;
		bool _isNew;
	private:
	public:
		Line(const math::Vector2 lineBegin, const math::Vector2 lineEnd);
		Line(const math::Vector2 lineBegin, const math::Vector2 lineEnd, unsigned int color, unsigned int duration, unsigned int zindex);
		~Line();

		math::Vector4 getLineElements() const override;
		inline const unsigned int getDuration() const { return _duration; }
		bool isNew();
		void tick();
		void destroyLine();
		void virtualDestructor() override;
	};
}