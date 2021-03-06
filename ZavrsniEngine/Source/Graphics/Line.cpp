#include "Line.h"
namespace graphics {
	Line::Line(const math::Vector2 lineBegin, const math::Vector2 lineEnd)
		:Sprite(0xff00ffff,100),_originPoint(lineBegin),_destinationPoint(lineEnd),_duration(PROCESSING_INTERVAL / 10),_isNew(true)
	{

	}
	Line::Line(const math::Vector2 lineBegin, const math::Vector2 lineEnd, unsigned int color, unsigned int duration, unsigned int zindex)
		:Sprite(color,zindex), _originPoint(lineBegin), _destinationPoint(lineEnd), _duration(duration * PROCESSING_INTERVAL / 5), _isNew(true)
	{
	}

	Line::~Line()
	{
	}

	math::Vector4 Line::getLineElements() const
	{
		return(math::Vector4(_originPoint.x, _originPoint.y, _destinationPoint.x,_destinationPoint.y));
	}

	void Line::tick()
	{
		_duration -= 1;
		if (_duration == 0)
			DestroySprite();
	}

	bool Line::isNew()
	{
		if (_isNew)
		{
			_isNew = false;
			return true;
		}
		return false;
	}
}