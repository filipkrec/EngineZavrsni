//#include "Audio.h"
#include "AudioManager.h"

namespace audio {
	void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
	{
		gc_int32* flag = (gc_int32*)(in_context);
		*flag = 1;
		Audio* currentAudio = (Audio*)in_handle->loop;
		ga_handle_destroy(in_handle);
	}

	Audio::Audio(const std::string& name, const std::string& filename)
		: _Name(name), _Filename(filename)
	{
		_Sound = gau_load_sound_file(filename.c_str(), "wav");
		_Looping = NULL;
		_loopSource = 0;
	}

	Audio::~Audio()
	{
		ga_sound_release(_Sound);
	}


	void Audio::play()
	{
		gc_int32 quit = 0;
		_Handle = gau_create_handle_sound(AudioManager::_Mixer, _Sound, &setFlagAndDestroyOnFinish, &quit, _Looping);
		ga_handle_play(_Handle);
	}

	void Audio::pause()
	{
		if (ga_handle_playing(_Handle)) {
			ga_handle_stop(_Handle);
			_Position = ga_handle_tell(_Handle, GA_TELL_PARAM_CURRENT);
		}
	}

	void Audio::resume()
	{
		if (ga_handle_stopped(_Handle))
		{
			ga_handle_play(_Handle);
			ga_handle_seek(_Handle, _Position);
		}
	}

	void Audio::stop()
	{
		if (ga_handle_playing(_Handle))
		{
			ga_handle_stop(_Handle);
			_Position = 0;
		}
	}

	void Audio::setGain(float gain)
	{
		_Gain = gain;
		if (ga_handle_playing(_Handle))
			ga_handle_setParamf(_Handle, GA_HANDLE_PARAM_GAIN, gain);
	}



	void Audio::setLoopOnFinish(const bool& willLoop)
	{
		if (willLoop == true)
			_Looping = &_loopSource;
		else {
			if (_loopSource != 0)
				gau_sample_source_loop_clear(_loopSource);
		}
	}
}