#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "Core/Config.h"
#include "Core/Error.h"
#include "Core/Instance.h"
#include "Core/Platform.h"
#include "Core/Pointers.h"
#include "Core/Utils.h"
#include "Core/Version.h"

#ifdef GDN_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#include <Windows.h>
#endif
