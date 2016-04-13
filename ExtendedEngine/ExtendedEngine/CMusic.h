#pragma once
#include <SFML\Audio\Music.hpp>
#include "IMusic.h"

namespace tle
{
	class CSoundManager;

	//sf::Music wrapper that hides unwanted functionality and provides additional volume control
	class CMusic : public IMusic
	{
	private:
		SoundType mType;			//The type of sound
		CSoundManager* mpManager;	//The manager object
		sf::Music* mpMusic;			//The internal sound object

		float mVolume;				//The individual volume control

	public:
		//Creates a music object
		CMusic(SoundType type, sf::Music* music, CSoundManager* manager);

		//Sets the type of sound
		virtual void SetType(SoundType type);

		//Sets the volume of the music
		virtual void SetVolume(float volume);

		//Set whether or not the music should loop after reaching the end
		virtual void SetLoop(bool loop);

		//Start or resume playing the music
		virtual void Play();

		//Pause the music, Play() will resume
		virtual void Pause();

		//Stops the music, Play() will restart
		virtual void Stop();

		//Updates the volume with the new modifier value
		virtual void UpdateVolumeModifier(float volumeModifier);

		//Gets the music type
		virtual SoundType GetType();

		//Gets the music's unmodified volume
		virtual float GetVolume();

		//Gets whether the music will loop
		virtual bool IsLooped();

		//Returns whether the music is currently playing, paused, or stopped
		virtual sf::SoundSource::Status GetStatus();

		//Destroys the music
		~CMusic();
	};
}