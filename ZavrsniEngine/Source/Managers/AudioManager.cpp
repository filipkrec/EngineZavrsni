#include "AudioManager.h"
#include <iostream>

namespace audio {

	gau_Manager* AudioManager::_manager = nullptr;
	ga_Mixer* AudioManager::_mixer = nullptr;
	std::vector<Audio*> AudioManager::_audios;
	bool AudioManager::_paused = false;

	void AudioManager::init()
	{
		gc_initialize(0);
		_manager = gau_manager_create_custom(GA_DEVICE_TYPE_XAUDIO2, GAU_THREAD_POLICY_SINGLE, 4, 512);
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


	void AudioManager::clear()
	{
		for (int i = 0; i < _audios.size(); ++i)
		{
			if(i == 0)
			_audios[i]->stop();
			delete _audios[i];
		}

		_audios.clear();
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