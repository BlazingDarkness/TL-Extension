#include "stdafx.h"
#include "CSoundManager.h"

namespace tle
{
	//Create a sound manager
	CSoundManager::CSoundManager()
	{
		//Nothing
	}

	/*******************
			Sound
	********************/

	//Loads the sound file if not already loaded and stores it
	//Then returns a pointer to the sound buffer or zero if failed to load
	sf::SoundBuffer* CSoundManager::LoadSound(const std::string& soundFile)
	{
		//Check if the sound file has already been loaded
		auto it = mSoundBuffers.find(soundFile);
		if (it != mSoundBuffers.end())
		{
			//Return a pointer to the existing sound buffer
			return it->second.get();
		}

		//Attempt to load the sound file
		sf::SoundBuffer* buffer = new sf::SoundBuffer();
		if (buffer->loadFromFile(soundFile))
		{
			mSoundBuffers.insert(TBufferPair{soundFile, SoundBuffer_ptr(buffer)});
			return buffer;
		}
		else
		{
			//Could not load the buffer
			delete buffer;
			return 0;
		}
	}

	//Creates a sound object from the given file
	//Returns 0 if the sound could not be loaded
	ISound* CSoundManager::CreateSound(const std::string& soundFile)
	{
		sf::SoundBuffer* buffer = LoadSound(soundFile);

		//If the sound could not be loaded then return a pointer to zero
		if (!buffer) return 0;
		
		CSound* sound = new CSound(SoundType::SFX, buffer, this);

		mSounds.push_back(CSound_ptr(sound));

		return sound;
	}

	//Removes the sound
	void CSoundManager::RemoveSound(ISound* pSound)
	{
		for (auto it = mSounds.begin(); it != mSounds.end(); ++it)
		{
			if ((*it).get() == pSound)
			{
				mSounds.erase(it);
				return; //No point going through the rest of the list
			}
		}
	}

	//Removes all sounds
	void CSoundManager::ClearSounds()
	{
		mSounds.clear();
		mSoundBuffers.clear();
	}

	/*******************
			Music
	********************/

	//Creates a music object from the given file
	//Returns 0 if the file could not be loaded
	IMusic* CSoundManager::CreateMusic(const std::string& musicFile)
	{
		sf::Music* sfmusic = new sf::Music();
		if (sfmusic->openFromFile(musicFile))
		{
			CMusic* music = new CMusic(SoundType::Music, sfmusic, this);
			mMusics.push_back(CMusic_ptr(music));
			return music;
		}
		else
		{
			delete sfmusic;
			return 0;
		}
	}

	//Removes the music
	void CSoundManager::RemoveMusic(IMusic* pMusic)
	{
		for (auto it = mMusics.begin(); it != mMusics.end(); ++it)
		{
			if ((*it).get() == pMusic)
			{
				mMusics.erase(it);
				return; //No point going through the rest of the list
			}
		}
	}

	//Removes all music
	void CSoundManager::ClearMusic()
	{
		mMusics.clear();
	}

	/*******************
			Volume
	********************/

	//Sets the volume for the specific volume modifier
	void CSoundManager::SetVolume(float volume, SoundType type)
	{
		if (volume > 100.0f) volume = 100.0f;
		if (volume < 0.0f) volume = 0.0f;

		switch (type)
		{
		case SoundType::SFX:
			mSFXVolume = volume;
			break;
		case SoundType::Ambient:
			mAmbientVolume = volume;
			break;
		case SoundType::Music:
			mMusicVolume = volume;
			break;
		case SoundType::Interface:
			mInterfaceVolume = volume;
			break;
		case SoundType::Global:
			sf::Listener::setGlobalVolume(volume);
			break;
		default:
			//Unknown music type
			break;
		}

		for (auto& sound : mSounds)
		{
			if (sound->GetType() == type) sound->UpdateVolumeModifier(volume);
		}

		for (auto& music : mMusics)
		{
			if (music->GetType() == type) music->UpdateVolumeModifier(volume);
		}
	}

	//Gets the volume for the specific volume modifier
	float CSoundManager::GetVolume(SoundType type)
	{
		switch (type)
		{
		case SoundType::SFX:
			return mSFXVolume;
		case SoundType::Ambient:
			return mAmbientVolume;
		case SoundType::Music:
			return mMusicVolume;
		case SoundType::Interface:
			return mInterfaceVolume;
		case SoundType::Global:
			return sf::Listener::getGlobalVolume();
		default:
			//Unknown music type
			return 0.0f;
		}
	}

	/*******************
		Destruction
	********************/

	//Removes all sounds and music
	void CSoundManager::Clear()
	{
		ClearSounds();
		ClearMusic();
	}

	//Destroys and sound or music objects and unloads sound buffers
	CSoundManager::~CSoundManager()
	{
		Clear();
	}
}