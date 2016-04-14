#include "stdafx.h"
#include "CMusic.h"
#include "CSoundManager.h"

namespace tle
{
	//Creates a music object
	CMusic::CMusic(SoundType type, sf::Music* music, CSoundManager* manager)
	{
		mType = type;
		mVolume = 100.0f;
		mpMusic = music;
		mpManager = manager;

		mpMusic->setVolume(mpManager->GetVolume(mType));
	}

	//Sets the type of sound
	void CMusic::SetType(SoundType type)
	{
		mType = type;
		mpMusic->setVolume(mpManager->GetVolume(mType) * mVolume * 0.01f);
	}

	//Sets the volume of the music
	void CMusic::SetVolume(float volume)
	{
		if (volume > 100.0f) mVolume = 100.0f;
		else if (volume < 0.0f) mVolume = 0.0f;
		else mVolume = volume;
		mpMusic->setVolume(mpManager->GetVolume(mType) * mVolume * 0.01f);
	}

	//Set whether or not the music should loop after reaching the end
	void CMusic::SetLoop(bool loop)
	{
		mpMusic->setLoop(loop);
	}

	//Start or resume playing the music
	void CMusic::Play()
	{
		mpMusic->play();
	}

	//Pause the music, Play() will resume
	void CMusic::Pause()
	{
		mpMusic->pause();
	}

	//Stops the music, Play() will restart
	void CMusic::Stop()
	{
		mpMusic->stop();
	}

	//Updates the volume with the new modifier value
	void CMusic::UpdateVolumeModifier(float volumeModifier)
	{
		mpMusic->setVolume(volumeModifier * mVolume * 0.01f);
	}

	//Gets the music type
	SoundType CMusic::GetType()
	{
		return mType;
	}

	//Gets the music's unmodified volume
	float CMusic::GetVolume()
	{
		return mVolume;
	}

	//Gets whether the music will loop
	bool CMusic::IsLooped()
	{
		return mpMusic->getLoop();
	}

	//Returns whether the music is currently playing, paused, or stopped
	sf::SoundSource::Status CMusic::GetStatus()
	{
		return mpMusic->getStatus();
	}

	//Destroys the music
	CMusic::~CMusic()
	{
		delete mpMusic;
	}
}