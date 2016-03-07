#pragma once
#include <vector>
#include "IAnimation.h"
#include "Sprite.h"

namespace tle
{
	//Hacky work around to avoid circular reference
	class ExEngine;

	using SpriteVector = std::vector<ISprite*>;

	class CAnimation : public IAnimation
	{
	private:
		CVector3 mPos;

		bool mPaused;
		bool mLooped;
		bool mHasEnded;

		float mAnimationRate;
		float mTimePassed;

		int mSize;
		int mIndex;
		SpriteVector mSprites;

		ExEngine* mEngine;

	public:
		CAnimation(ExEngine* engine, SpriteVector& sprites, const CVector3& pos, const float rate, const bool looped);

		virtual void Update(float delta);

		virtual void Run();
		virtual void Pause();
		virtual void Reset();

		virtual void SetX(float x);
		virtual void SetY(float y);
		virtual void SetZ(float z);

		virtual void SetPosition(const CVector3& pos);

		virtual float GetX();
		virtual float GetY();
		virtual float GetZ();

		virtual CVector3 GetPosition();

		virtual bool IsPaused();
		virtual bool IsLooped();
		virtual bool HasEnded();

		virtual void MoveX(float xMovement);
		virtual void MoveY(float yMovement);
		virtual void MoveZ(float zMovement);
		virtual void Move(const CVector3& movement);

		~CAnimation();
	};
}