#pragma once
#include <vector>
#include "Audio.h"

namespace audio
{
	class AudioManager
	{
	private:
		friend class Audio;
		friend void setLoopOnFinish(ga_Handle* in_handle, void* in_context);

		static std::vector<Audio*> _Audios;
		static gau_Manager* _Manager;
		static ga_Mixer* _Mixer;
		static bool _Paused;
	public:

		static void init();
		static void add(Audio* audio);
		static void clean();
		static void update();
		static Audio* get(const std::string& name);
		static void togglePause();

	private:
		AudioManager() {};
	};
}