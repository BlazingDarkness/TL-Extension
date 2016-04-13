#include "stdafx.h"
#include "ExtendedEngine.h"
#include "ExEngine.h"

namespace tle
{
	// Legacy style behaviour of certain TL functions
	bool g_bLegacyTL = false;

	IEngine* NewEngine()
	{
		TL_FN("New3DEngine");
		return new ExEngine();
		TL_ENDFN;
	}
}
