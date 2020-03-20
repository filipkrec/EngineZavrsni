#include "Timer.h"

namespace engine {
	
	std::chrono::time_point<Timer::HighResolutionClock> Timer::_pauseStart = Timer::HighResolutionClock::now();
	float Timer::_pauseTime = false;
	bool Timer::_paused = false;

	Timer::Timer()
	{
		_dontPause = false;
		reset();
	}

	Timer::Timer(float startingTime)
	{
		_dontPause = false;
		int roundedUp = ceil(startingTime);
		_start = HighResolutionClock::now();
		_start -= std::chrono::seconds(roundedUp);
	}

	void Timer::reset()
	{
		_start = HighResolutionClock::now();
		_pauseTime = 0;
	}

	float Timer::elapsed()
	{
		if (!_paused || _dontPause)
		{
			float _tempPauseTime = _dontPause ? 0 : _pauseTime;
			return std::chrono::duration_cast<milisecondsType>(HighResolutionClock::now() - _start).count() / 1000.0f - _pauseTime;
		}
	}

	void Timer::setUnpausable()
	{
		_dontPause = true;
	}

	void Timer::pause()
	{
		_pauseStart = HighResolutionClock::now();
		_paused = true;
	}

	bool Timer::isPaused()
	{
		return _paused;
	}

	float Timer::getPauseTime()
	{
		return std::chrono::duration_cast<milisecondsType>(HighResolutionClock::now() - _pauseStart).count() / 1000.0f;
	}

	void Timer::unpause()
	{
		_paused = false;
		_pauseTime = std::chrono::duration_cast<milisecondsType>(HighResolutionClock::now() - _pauseStart).count() / 1000.0f;
	}
}