#pragma once

#define WIN32_LEAN_AND_MEAN // Minimise windows includes

#include "IEngine.h"

namespace tle
{
	// Legacy style behaviour of certain TL functions
	bool g_bLegacyTL = false;

	IEngine* NewEngine();
}