#pragma once

#ifdef _WIN32
	#define GDN_WINDOWS

	#define GDN_PLATFORMTYPE(type) GDN_CONCAT(type, Windows)
#endif
