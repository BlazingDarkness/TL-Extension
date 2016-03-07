#include "stdafx.h"
#include "CAnimation.h"
#include "ExEngine.h"

namespace tle
{
	CAnimation::CAnimation(ExEngine* engine, SpriteVector& sprites, const CVector3& pos, const float rate, const bool looped)
	{
		mPos = pos;

		mPaused = false;
		mLooped = looped;
		mHasEnded = false;

		mAnimationRate = rate;
		mTimePassed = 0.0f;

		mSize = static_cast<int>(mSprites.size());
		mIndex = 0;
		mSprites = sprites;

		mEngine = engine;

		//Set the first sprite on screen
		if (mIndex < mSize) //Check if there is sprites within the vector
		{
			mSprites[mIndex]->SetPosition(mPos.x, mPos.y);
			mIndex++;
		}

		//Move all other sprites off screen
		for (; mIndex < mSize; mIndex++)
		{
			mSprites[mIndex]->SetPosition(static_cast<float>(mEngine->GetWidth()),
				static_cast<float>(mEngine->GetHeight()));
		}
		mIndex = 0;
	}

	//Move the animation frames along by the set amount of time
	void CAnimation::Update(float delta)
	{
		//Don't Animate if it is not looped and has already reached the end
		if (!mLooped && mIndex == mSize) return;
		//Don't Animate if there is not sprites in the vector, so I can assume for the rest of the code that there is at least one sprite
		//It's also a small optimization due to less checks needed to make it robust
		if (!mSize) return;
		if (mPaused) return;

		//Increment timer
		mTimePassed += delta;
		//For each tick of the animation that has passed since the last call
		while (mTimePassed > mAnimationRate)
		{
			//Remove the previous sprite
			mSprites[mIndex]->SetPosition(static_cast<float>(mEngine->GetWidth()),
				static_cast<float>(mEngine->GetHeight()));

			//Increment sprite pointer
			mIndex++;
			if (mIndex == mSize)
			{
				if (mLooped) //Loop back to the start
				{
					mIndex = 0;
				}
				else //Revert back to the last available sprite
				{
					mIndex--;
					mHasEnded = true;
				}
			}

			//Place next sprite on screen
			mSprites[mIndex]->SetPosition(mPos.x, mPos.y);

			//Decrement timer
			mTimePassed -= mAnimationRate;
		}
	}

	//Starts the animation
	void CAnimation::Run()
	{
		mPaused = false;
	}

	//Pauses the animation
	void CAnimation::Pause()
	{
		mPaused = true;
	}

	//Reset the animation to the start
	void CAnimation::Reset()
	{
		mTimePassed = 0.0f;
		mHasEnded = false;

		//Check if there are sprites
		if (mSize)
		{
			//Move the previous sprite off screen
			mSprites[mIndex]->SetPosition(static_cast<float>(mEngine->GetWidth()),
				static_cast<float>(mEngine->GetHeight()));

			//Move back to the start
			mIndex = 0;

			//Place the current sprite on screen
			mSprites[mIndex]->SetPosition(mPos.x, mPos.y);
		}
	}

	//Set the x position of the animation
	void CAnimation::SetX(float x)
	{
		mPos.x = x;
		//Update the current sprite if one exists
		if (mIndex < mSize)
		{
			mSprites[mIndex]->SetX(mPos.x);
		}
	}

	//Set the y position of the animation
	void CAnimation::SetY(float y)
	{
		mPos.y = y;
		//Update the current sprite if one exists
		if (mIndex < mSize)
		{
			mSprites[mIndex]->SetY(mPos.y);
		}
	}

	//Set the z position of the animation
	void CAnimation::SetZ(float z)
	{
		mPos.z = z;
		//Update all sprites in the vector
		for (auto it = mSprites.begin(); it != mSprites.end(); it++)
		{
			(*it)->SetZ(mPos.z);
		}
	}

	//Set the x, y and z position of the animation
	void CAnimation::SetPosition(const CVector3& pos)
	{
		mPos = pos;

		//Update the current sprite if one exist
		if (mIndex < mSize)
		{
			mSprites[mIndex]->SetPosition(mPos.x, mPos.y);
		}

		//Update all sprites in the vector
		for (auto it = mSprites.begin(); it != mSprites.end(); it++)
		{
			(*it)->SetZ(mPos.z);
		}
	}

	//Return the x position of the animation
	float CAnimation::GetX()
	{
		return mPos.x;
	}

	//Return the y position of the animation
	float CAnimation::GetY()
	{
		return mPos.y;
	}

	//Return the z position of the animation
	float CAnimation::GetZ()
	{
		return mPos.z;
	}

	//Returns the position of the animation
	CVector3 CAnimation::GetPosition()
	{
		return mPos;
	}

	//Returns whether the animation has been paused
	bool CAnimation::IsPaused()
	{
		return mPaused;
	}

	//Returns whether the animation has reached the end if the animation is not looped
	bool CAnimation::HasEnded()
	{
		return mHasEnded;
	}

	//Return whether the animation is looped or not
	bool CAnimation::IsLooped()
	{
		return mLooped;
	}

	void CAnimation::MoveX(float xMovement)
	{
		mPos.x += xMovement;
		//Update the current sprite if one exists
		if (mIndex < mSize)
		{
			mSprites[mIndex]->SetX(mPos.x);
		}
	}

	void CAnimation::MoveY(float yMovement)
	{
		mPos.y += yMovement;
		//Update the current sprite if one exists
		if (mIndex < mSize)
		{
			mSprites[mIndex]->SetY(mPos.y);
		}
	}

	void CAnimation::MoveZ(float zMovement)
	{
		SetZ(mPos.z + zMovement);
	}

	void CAnimation::Move(const CVector3& movement)
	{
		mPos = movement;

		//Update the current sprite if one exist
		if (mIndex < mSize)
		{
			mSprites[mIndex]->SetPosition(mPos.x, mPos.y);
		}

		//Update all sprites in the vector
		for (auto it = mSprites.begin(); it != mSprites.end(); it++)
		{
			(*it)->SetZ(mPos.z);
		}
	}

	CAnimation::~CAnimation()
	{
		//Remove all sprites from the scene
		for (auto it = mSprites.begin(); it != mSprites.end(); it++)
		{
			mEngine->RemoveSprite(*it);
		}
	}
}