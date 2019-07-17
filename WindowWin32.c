#include "Window.h"

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#if defined( LP_USE_PLATFORM_WIN32 )

#pragma comment(lib, "gdi32.lib")

// Microsoft Windows specific versions of window functions
LRESULT CALLBACK WindowsEventHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg ) {
	case WM_CLOSE:
		return 0;
	case WM_SIZE:
		break;
    case WM_MOVE:
        printf("move\n");
        break;
	default:
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LpWindow* lpCreateWindow(const LpWindowUserParameter* window_parameter)
{
    WNDCLASSEX win_class;// {};
	assert( window_parameter->size_width > 0 );
	assert( window_parameter->size_height > 0 );

    LpWindow* window = (LpWindow*)malloc(sizeof(LpWindow));

	window->platform_ralative_data._win32_instance				= GetModuleHandle( NULL );
	window->platform_ralative_data._win32_class_name			= window_parameter->name;

	// Initialize the window class structure:
	win_class.cbSize			= sizeof( WNDCLASSEX );
	win_class.style				= CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc		= WindowsEventHandler;
	win_class.cbClsExtra		= 0;
	win_class.cbWndExtra		= 0;
	win_class.hInstance			= window->platform_ralative_data._win32_instance; // hInstance
	win_class.hIcon				= LoadIcon( NULL, IDI_APPLICATION );
	win_class.hCursor			= LoadCursor( NULL, IDC_ARROW );
	win_class.hbrBackground		= (HBRUSH)GetStockObject( WHITE_BRUSH );
	win_class.lpszMenuName		= NULL;
	win_class.lpszClassName		= window->platform_ralative_data._win32_class_name;
	win_class.hIconSm			= LoadIcon( NULL, IDI_WINLOGO );
	// Register window class:
	if( !RegisterClassEx( &win_class ) ) {
		// It didn't work, so try to give a useful error:
		assert( 0 && "Cannot create a window in which to draw!\n" );
		fflush( stdout );
		//std::exit( -1 );
	}

	DWORD ex_style	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style		= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// Create window with the registered class:
	RECT wr = { 0, 0, window_parameter->size_width, window_parameter->size_height };
	AdjustWindowRectEx( &wr, style, FALSE, ex_style );
	window->platform_ralative_data._win32_window = CreateWindowEx( 0,
		window->platform_ralative_data._win32_class_name,		// class name
		window->platform_ralative_data._win32_class_name,			// app name
		style,							// window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
		wr.right - wr.left,				// width
		wr.bottom - wr.top,				// height
		NULL,							// handle to parent
		NULL,							// handle to menu
		window->platform_ralative_data._win32_instance,				// hInstance
		NULL );							// no extra parameters
	if( !window->platform_ralative_data._win32_window ) {
		// It didn't work, so try to give a useful error:
		assert( 1 && "Cannot create a window in which to draw!\n" );
		fflush( stdout );
		//std::exit( -1 );
	}
	//SetWindowLongPtr( window->platform_ralative_data._win32_window, GWLP_USERDATA, ( LONG_PTR )this );

	ShowWindow( window->platform_ralative_data._win32_window, SW_SHOW );
	SetForegroundWindow( window->platform_ralative_data._win32_window );
	SetFocus( window->platform_ralative_data._win32_window );

    return window;
}

void lpProcessEvents(LpWindow* window)
{
    MSG msg;
	if( PeekMessage( &msg, window->platform_ralative_data._win32_window, 0, 0, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void lpDestoryWindow(LpWindow* window)
{

}

#endif