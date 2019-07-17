#include "Platform.h"

#include "API.h"

#include <stddef.h> //for NULL
#include <inttypes.h>

typedef struct LpPlatformRelativeData
{
#if defined( LP_USE_PLATFORM_WIN32 )

    HINSTANCE							_win32_instance;//					= NULL;
	HWND								_win32_window;//					= NULL;
	const char*							_win32_class_name;
	uint16_t						    _win32_class_id_counter;

#elif defined ( LP_USE_PLATFORM_LINUX )

    xcb_connection_t				*	_xcb_connection;//					= NULL;
	xcb_screen_t					*	_xcb_screen;//						= NULL;
	xcb_window_t						_xcb_window;//						= 0;
	xcb_intern_atom_reply_t			*	_xcb_atom_window_reply;//			= NULL;

#endif
}LpPlatformRelativeData;

typedef struct LpWindowUserParameter
{
    int16_t origin_x, origin_y;
    int16_t size_width, size_height;
    const char* name;
}LpWindowUserParameter;

typedef struct lpWindow
{
    LpPlatformRelativeData platform_ralative_data;
    LpWindowUserParameter window_user_parameter;
}LpWindow;

LpWindow* lpCreateWindow(const LpWindowUserParameter* window_parameter);

void lpProcessEvents(LpWindow* window);

void lpDestoryWindow(LpWindow* window);