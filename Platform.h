#pragma once

#if defined( _WIN32 )

#define LP_USE_PLATFORM_WIN32

#elif defined( __linux )

#define LP_USE_PLATFORM_LINUX

#else

#error Platform not yet supported

#endif
