#include "stdafx.h"
#include "CSound.h"
#include "CSoundManager.h"

namespace tle
{
	//Creates a sound object
	CSound::CSound(SoundType type, sf::SoundBuffer* buffer, CSoundManager* manager)
	{
		mType = type;
		mVolume = 100.0f;
		mpSound = new sf::Sound(*buffer);
		mpManager = manager;

		mpSound->setVolume(mpManager->GetVolume(mType));
	}

	//Sets the type of sound
	void CSound::SetType(SoundType type)
	{
		mType = type;
		mpSound->setVolume(mpManager->GetVolume(mType) * mVolume * 0.01f);
	}

	//Sets the volume of the music
	void CSound::SetVolume(float volume)
	{
		if (volume > 100.0f) mVolume = 100.0f;
		else if (volume < 0.0f) mVolume = 0.0f;
		else mVolume = volume;
		mpSound->setVolume(mpManager->GetVolume(mType) * mVolume * 0.01f);
	}

	//Set whether or not the music should loop after reaching the end
	void CSound::SetLoop(bool loop)
	{
		mpSound->setLoop(loop);
	}

	//Start or resume playing the music
	void CSound::Play()
	{
		mpSound->play();
	}

	//Pause the music, Play() will resume
	void CSound::Pause()
	{
		mpSound->pause();
	}

	//Stops the music, Play() will restart
	void CSound::Stop()
	{
		mpSound->stop();
	}

	//Updates the volume with the new modifier value
	void CSound::UpdateVolumeModifier(float volumeModifier)
	{
		mpSound->setVolume(volumeModifier * mVolume * 0.01f);
	}

	//Gets the music type
	SoundType CSound::GetType()
	{
		return mType;
	}

	//Gets the music's unmodified volume
	float CSound::GetVolume()
	{
		return mpSound->getVolume();
	}

	//Gets whether the music will loop
	bool CSound::IsLooped()
	{
		return mpSound->getLoop();
	}

	//Returns whether the music is currently playing, paused, or stopped
	sf::SoundSource::Status CSound::GetStatus()
	{
		return mpSound->getStatus();
	}

	//Destroys the sound
	CSound::~CSound()
	{
		delete mpSound;
	}
}