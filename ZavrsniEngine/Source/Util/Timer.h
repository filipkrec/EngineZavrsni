#pragma once

#include <chrono>

namespace engine {
	class Timer
	{
	private:
		bool _dontPause;
		typedef std::chrono::high_resolution_clock HighResolutionClock;
		typedef std::chrono::duration<float, std::milli> milisecondsType;
		std::chrono::time_point<HighResolutionClock> _start;

		static std::chrono::time_point<HighResolutionClock> _pauseStart;
		static float _pauseTime;
		static bool _paused;
	public:
		Timer();
		Timer(float startingTime);
		void reset();
		float elapsed();
		void setUnpausable();
		static void pause();
		static bool isPaused();
		static float getPauseTime();
		static void unpause();
	};
}