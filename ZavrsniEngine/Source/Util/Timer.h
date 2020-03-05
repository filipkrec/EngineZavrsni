#pragma once

#include <chrono>

namespace engine {
	class Timer
	{
	private:
		typedef std::chrono::high_resolution_clock HighResolutionClock;
		typedef std::chrono::duration<float, std::milli> milisecondsType;
		std::chrono::time_point<HighResolutionClock> _Start;
	public:
		Timer()
		{
			reset();
		}

		Timer(float startingTime)
		{
			int roundedUp = ceil(startingTime);
			_Start = HighResolutionClock::now();
			_Start -= std::chrono::seconds(roundedUp);
		}

		void reset()
		{
			_Start = HighResolutionClock::now();
		}

		float elapsed()
		{
			return std::chrono::duration_cast<milisecondsType>(HighResolutionClock::now() - _Start).count() / 1000.0f;
		}
	};
}