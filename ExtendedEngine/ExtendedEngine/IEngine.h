#pragma once
#include "TL-Engine.h"
#include "IAnimation.h"
#include "IUsings.h"

namespace tle
{
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