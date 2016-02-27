#pragma once
#include "TL-Engine.h"
#include "CVector3.h"
#include "IAnimation.h"
#include <vector>

namespace tle
{
	using tlx::CVector3;

	class IEngine : I3DEngine
	{
		virtual IAnimation* CreateAnimation(const std::vector<string>& frameList, const CVector3& position, const float tickRate, const bool looped) = 0;
		virtual IAnimation* CreateAnimation(string& name, string& extension, int amount, const CVector3& position, const float tickRate, const bool looped) = 0;
		virtual void RemoveAnimation(IAnimation* pAnimation) = 0;
	};
}