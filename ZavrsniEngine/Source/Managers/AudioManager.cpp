#include "AudioManager.h"

namespace audio {

	gau_Manager* AudioManager::_manager = nullptr;
	ga_Mixer* AudioManager::_mixer = nullptr;
	std::vector<Audio*> AudioManager::_audios;
	bool AudioManager::_paused = false;

	void AudioManager::init()
	{
		gc_initialize(0);
		_manager = gau_manager_create();
		_mixer = gau_manager_mixer(_manager);
	}


	void AudioManager::add(Audio* audio)
	{
		_audios.push_back(audio);
	}

	Audio* AudioManager::get(const std::string& name)
	{
		for (Audio* audio : _audios)
		{
			if (audio->getName() == name)
				return audio;
		}
		return nullptr;
	}

	void AudioManager::clean()
	{
		for (int i = 0; i < _audios.size(); ++i)
			delete _audios[i];

		gau_manager_destroy(_manager);
		gc_shutdown();
	}

	void AudioManager::update()
	{
		if (!_paused) {
			gau_manager_update(AudioManager::_manager);
		}
		else
			printf("PAUSED");
	}

	void AudioManager::togglePause()
	{
		_paused ? false : true;
	}

}