#include "Window.h"

#include <stdio.h>
#include <stdlib.h>
//#include <math.h>

#include "CMakePlatformInfo.h"

int main(int argc, char* argv[])
{
    printf("This project is build with CMake, this platform is %s.\n",LP_PLATFORM_INFO_NAME);
    int16_t origin_x = 300, origin_y = 150;
    int16_t size_width = 400, size_height = 300;
    if(argc>2 && argc<6)
    {
        origin_x = atof(argv[1]) > 0 ? atof(argv[1]) : origin_x;
        origin_y = atof(argv[2]) > 0 ? atof(argv[2]) : origin_y;
        size_width = atof(argv[3]) > 0 ? atof(argv[3]) : size_width;
        size_height = atof(argv[4]) > 0 ? atof(argv[4]) : size_height;
    }
    else// if(argc < 2)
    {
        printf("Window parameter by default.\n");
    }

    LpWindowUserParameter window_user_parameter;
    window_user_parameter.name = "Hello World";
    window_user_parameter.origin_x = origin_x;
    window_user_parameter.origin_y = origin_y;
    window_user_parameter.size_width = size_width;
    window_user_parameter.size_height = size_height;

    LpWindow* window = lpCreateWindow(&window_user_parameter);

    while(1)
    {
        lpProcessEvents(window);
    }

    lpDestoryWindow(window);

    return 0;
}