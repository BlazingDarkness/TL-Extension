#pragma once
#include <SFML\Audio.hpp>
#include <unordered_map>
#include <memory>
#include "CSound.h"
#include "CMusic.h"

namespace tle
{
	//Manages sounds and provides volume controls
	class CSoundManager
	{
	private:
		using CSound_ptr = std::unique_ptr<CSound>;
		using CMusic_ptr = std::unique_ptr<CMusic>;
		using SoundBuffer_ptr = std::unique_ptr<sf::SoundBuffer>;
		using TBufferPair = std::pair<std::string, std::unique_ptr<sf::SoundBuffer>>;
		using TSoundMap = std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>>; //Thank stroustrup for typedefs
		using TSoundList = std::list<CSound_ptr>;
		using TMusicList = std::list<CMusic_ptr>;

		TSoundMap mSoundBuffers;
		TSoundList mSounds;
		TMusicList mMusics;

		float mSFXVolume = 100.0f;
		float mAmbientVolume = 100.0f;
		float mMusicVolume = 100.0f;

	public:
		//Create a sound manager
		CSoundManager();

		/*******************
				Sound
		********************/

		//Loads the sound file if not already loaded and stores it
		//Then returns a pointer to the sound buffer
		sf::SoundBuffer* LoadSound(const std::string& soundFile);

		//Creates a sound object from the given file
		//Returns 0 if the sound could not be loaded
		ISound* CreateSound(const std::string& soundFile);

		//Removes the sound
		void RemoveSound(ISound* pSound);

		//Removes all sounds
		void ClearSounds();

		/*******************
				Music
		********************/

		//Creates a music object from the given file
		//Returns 0 if the file could not be loaded
		IMusic* CreateMusic(const std::string& musicFile);

		//Removes the music
		void RemoveMusic(IMusic* pMusic);

		//Removes all music
		void ClearMusic();

		/*******************
				Volume
		********************/

		//Sets the volume for the specific volume modifier
		void SetVolume(float volume, SoundType type);

		//Gets the volume for the specific volume modifier
		float GetVolume(SoundType type);

		/*******************
			Destruction
		********************/

		//Removes all sounds and music
		void Clear();

		//Destroys and sound or music objects and unloads sound buffers
		~CSoundManager();
	};
}