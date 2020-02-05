#include "AudioManager.h"

namespace audio {

	gau_Manager* AudioManager::_Manager = nullptr;
	ga_Mixer* AudioManager::_Mixer = nullptr;
	std::vector<Audio*> AudioManager::_Audios;
	bool AudioManager::_Paused = false;

	void AudioManager::init()
	{
		gc_initialize(0);
		_Manager = gau_manager_create();
		_Mixer = gau_manager_mixer(_Manager);
	}


	void AudioManager::add(Audio* audio)
	{
		_Audios.push_back(audio);
	}

	Audio* AudioManager::get(const std::string& name)
	{
		for (Audio* audio : _Audios)
		{
			if (audio->getName() == name)
				return audio;
		}
		return nullptr;
	}

	void AudioManager::clean()
	{
		for (int i = 0; i < _Audios.size(); ++i)
			delete _Audios[i];

		gau_manager_destroy(_Manager);
		gc_shutdown();
	}

	void AudioManager::update()
	{
		if (!_Paused) {
			gau_manager_update(AudioManager::_Manager);
		}
		else
			printf("PAUSED");
	}

	void AudioManager::togglePause()
	{
		_Paused ? false : true;
	}

}