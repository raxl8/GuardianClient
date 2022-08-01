#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "Core/Config.h"
#include "Core/Error.h"
#include "Core/Pointers.h"

#include "Core/Platform.h"

#ifdef GDN_WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#include <Windows.h>
#endif
