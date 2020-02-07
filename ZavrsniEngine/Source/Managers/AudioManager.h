#pragma once
#include <vector>
#include "../Audio/Audio.h"

namespace audio
{
	class AudioManager
	{
	private:
		friend class Audio;

		static std::vector<Audio*> _audios;
		static gau_Manager* _manager;
		static ga_Mixer* _mixer;
		static bool _paused;
	public:

		static void init();
		static void add(Audio* audio);
		static void clean();
		static void update();
		static Audio* get(const std::string& name);
		static void togglePause();
	};
}