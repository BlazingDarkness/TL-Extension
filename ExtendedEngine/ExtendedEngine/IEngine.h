#pragma once
#include "TL-Engine.h"
#include "CVector3.h"
#include "IAnimation.h"
#include <vector>

namespace tle
{
	using tlx::CVector3;

	class IEngine : public I3DEngine
	{
		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(const std::vector<string>& frameList,
							const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),
							const float		tickRate	= 0.1f,
							const bool		looped		= true
							) = 0;

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(string& name, string& extension, int amount,
							const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),
							const float		tickRate	= 0.1f,
							const bool		looped		= true
							) = 0;

		//Attempts to remove the animation
		virtual void RemoveAnimation(IAnimation* pAnimation) = 0;
	};
}