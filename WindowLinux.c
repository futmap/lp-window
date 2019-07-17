#include "Window.h"

#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

#if defined( LP_USE_PLATFORM_LINUX )

LpWindow* lpCreateWindow(const LpWindowUserParameter* window_parameter)
{
    // int screen_num;
    // xcb_connection_t *display_connection = xcb_connect(NULL, &screen_num);
    // const xcb_setup_t *setup = xcb_get_setup(connection);
    // xcb_screen_iterator_t display_iter = xcb_setup_roots_iterator(setup);
    // for (int i = 0; i < screen_num; ++i)
    // {
    //     xcb_screen_next(&display_iter);
    // }
    // xcb_screen_t *display_screen = display_iter.data;

    xcb_connection_t* connection = xcb_connect(NULL, NULL);
    const xcb_setup_t* setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t* screen = iter.data;

    xcb_window_t window = xcb_generate_id(connection);

    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2] = {screen->white_pixel,
                          XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
                              XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                              XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
                              XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE};

    xcb_create_window(connection,                                                       /* Connection */
                      XCB_COPY_FROM_PARENT,                                             /* Depth */
                      window,                                                           /* Window ID */
                      screen->root,                                                     /* Parent Window */
                      window_parameter->origin_x, window_parameter->origin_y,           /* X, Y */
                      window_parameter->size_width, window_parameter->size_height,      /* Width, Height */
                      10,                                                               /* Border Width */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,                                    /* Class */
                      screen->root_visual,                                              /* Visual */
                      mask, values                                                          /* Masks, Value List */);
    
    LpWindow* mywindow = (LpWindow*)malloc(sizeof(LpWindow) * 1);

    mywindow->window_user_parameter.name = window_parameter->name;
    mywindow->window_user_parameter.origin_x = window_parameter->origin_x;
    mywindow->window_user_parameter.origin_y = window_parameter->origin_y;
    mywindow->window_user_parameter.size_width = window_parameter->size_width;
    mywindow->window_user_parameter.size_height = window_parameter->size_height;

    mywindow->platform_ralative_data._xcb_connection = connection;
    mywindow->platform_ralative_data._xcb_screen = screen;
    mywindow->platform_ralative_data._xcb_window = window;

    xcb_map_window(connection, window);

    xcb_flush(connection);

    return mywindow;
    //pause();
}

/* print names of modifiers present in mask */
void print_modifiers(uint32_t mask)
{
    const char *MODIFIERS[] = {
        "Shift", "Lock", "Ctrl", "Alt",
        "Mod2", "Mod3", "Mod4", "Mod5",
        "Button1", "Button2", "Button3", "Button4", "Button5"};

    printf("Modifier mask: ");
    for (const char **modifier = MODIFIERS; mask; mask >>= 1, ++modifier)
    {
        if (mask & 1)
        {
            printf("%s", *modifier);
        }
    }
    printf("\n");
}

void lpProcessEvents(LpWindow* window)
{
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(window->platform_ralative_data._xcb_connection)))
    {
        switch (event->response_type & ~0x80)
        {
        case XCB_EXPOSE:
        {
            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;

            printf("Window %" PRIu32 " exposed. Region to be redrawn at location (%" PRIu16 ",%" PRIu16 "), with dimension (%" PRIu16 ",%" PRIu16 ")\n",
                   expose->window, expose->x, expose->y, expose->width, expose->height);
            break;
        }
        case XCB_BUTTON_PRESS:
        {
            xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
            print_modifiers(bp->state);

            switch (bp->detail)
            {
            case 4:
                printf("Wheel Button up in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                       bp->event, bp->event_x, bp->event_y);
                break;
            case 5:
                printf("Wheel Button down in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                       bp->event, bp->event_x, bp->event_y);
                break;
            default:
                printf("Button %" PRIu8 " pressed in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                       bp->detail, bp->event, bp->event_x, bp->event_y);
                break;
            }
            break;
        }
        case XCB_BUTTON_RELEASE:
        {
            xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
            print_modifiers(br->state);

            printf("Button %" PRIu8 " released in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                   br->detail, br->event, br->event_x, br->event_y);
            break;
        }
        case XCB_MOTION_NOTIFY:
        {
            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;

            printf("Mouse moved in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                   motion->event, motion->event_x, motion->event_y);
            break;
        }
        case XCB_ENTER_NOTIFY:
        {
            xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *)event;

            printf("Mouse entered window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                   enter->event, enter->event_x, enter->event_y);
            break;
        }
        case XCB_LEAVE_NOTIFY:
        {
            xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *)event;

            printf("Mouse left window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                   leave->event, leave->event_x, leave->event_y);
            break;
        }
        case XCB_KEY_PRESS:
        {
            xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
            print_modifiers(kp->state);

            printf("Key pressed in window %" PRIu32 "\n",
                   kp->event);
            break;
        }
        case XCB_KEY_RELEASE:
        {
            xcb_key_release_event_t *kr = (xcb_key_release_event_t *)event;
            print_modifiers(kr->state);

            printf("Key released in window %" PRIu32 "\n",
                   kr->event);
            break;
        }
        default:
            /* Unknown event type, ignore it */
            printf("Unknown event: %" PRIu8 "\n",
                   event->response_type);
            break;
        }

        free(event);
    }
}

void lpDestoryWindow(LpWindow* window)
{

}

#endif