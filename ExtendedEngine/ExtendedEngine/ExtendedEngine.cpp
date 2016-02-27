#include "stdafx.h"
#include "ExtendedEngine.h"
#include "ExEngine.h"

namespace tle
{
	IEngine* NewEngine()
	{
		TL_FN("New3DEngine");
		return new ExEngine();
		TL_ENDFN;
	}
}
