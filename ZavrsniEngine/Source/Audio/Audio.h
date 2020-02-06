#pragma once
#include <string>

#include <gorilla/ga.h>
#include <gorilla/gau.h>

namespace audio {
	class Audio
	{
		friend void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context);

		std::string _name;
		std::string _filename;

		gau_SampleSourceLoop** _looping;
		gau_SampleSourceLoop* _loopSource;

		ga_Sound* _sound;
		ga_Handle* _handle;

		int _position;
		float _gain;
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

		inline const std::string& getName() const { return _name; }
		inline const std::string& getFilename() const { return _filename; }

	};
}