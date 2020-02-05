#pragma once
#include <string>

#include <gorilla/ga.h>
#include <gorilla/gau.h>

namespace audio {
	class Audio
	{
		friend void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context);

		std::string _Name;
		std::string _Filename;

		gau_SampleSourceLoop** _Looping;
		gau_SampleSourceLoop* _loopSource;

		ga_Sound* _Sound;
		ga_Handle* _Handle;

		int _Position;
		float _Gain;
		void loop();

	public:
		Audio(const std::string& name, const std::string& filename);
		~Audio();

		void play();
		void pause();
		void resume();
		void stop();
		void setLoopOnFinish(const bool& willLoop);

		void setGain(float gain);

		inline const std::string& getName() const { return _Name; }
		inline const std::string& getFilename() const { return _Filename; }

	};
}