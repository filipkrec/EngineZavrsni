//#include "Audio.h"
#include "../Managers/AudioManager.h"

namespace audio {
	void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
	{
		gc_int32* flag = (gc_int32*)(in_context);
		*flag = 1;
		Audio* currentAudio = (Audio*)in_handle->loop;
		ga_handle_destroy(in_handle);
	}

	Audio::Audio(const std::string& name, const std::string& filename)
		: _name(name), _filename(filename)
	{
		_sound = gau_load_sound_file(filename.c_str(), "wav");
		_looping = NULL;
		_loopSource = 0;
	}

	Audio::~Audio()
	{
		ga_sound_release(_sound);
	}


	void Audio::play()
	{
		gc_int32 quit = 0;
		_handle = gau_create_handle_sound(AudioManager::_mixer, _sound, &setFlagAndDestroyOnFinish, &quit, _looping);
		ga_handle_play(_handle);
	}

	void Audio::pause()
	{
		if (ga_handle_playing(_handle)) {
			ga_handle_stop(_handle);
			_position = ga_handle_tell(_handle, GA_TELL_PARAM_CURRENT);
		}
	}

	void Audio::resume()
	{
		if (ga_handle_stopped(_handle))
		{
			ga_handle_play(_handle);
			ga_handle_seek(_handle, _position);
		}
	}

	void Audio::stop()
	{
		if (ga_handle_playing(_handle))
		{
			ga_handle_stop(_handle);
			_position = 0;
		}
	}

	void Audio::setGain(float gain)
	{
		_gain = gain;
		if (ga_handle_playing(_handle))
			ga_handle_setParamf(_handle, GA_HANDLE_PARAM_GAIN, gain);
	}



	void Audio::setLoopOnFinish(const bool& willLoop)
	{
		if (willLoop == true)
			_looping = &_loopSource;
		else {
			if (_loopSource != 0)
				gau_sample_source_loop_clear(_loopSource);
		}
	}
}