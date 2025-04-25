#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <cstdlib>
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkGraphics.h"

typedef struct win
{
    Display *dpy;
    int scr;

    Window win;

    int width, height;
    KeyCode quit_code;

    GC gc;
    XImage *ximage;
    SkBitmap bitmap;
} win_t;

static void win_init(win_t *win);
static void win_deinit(win_t *win);
static void win_draw(win_t *win);
static void win_handle_events(win_t *win);
static void win_init_skia(win_t *win);

static win_t
dpy_init()
{
    SkGraphics::Init();

    win_t win;
    win.dpy = XOpenDisplay(0);

    if (win.dpy == NULL)
    {
        fprintf(stderr, "Failed to open display\n");
        exit(1);
    }

    return win;
}

static void win_init(win_t *win)
{
    Window root;

    win->width = 400;
    win->height = 400;

    root = DefaultRootWindow(win->dpy);
    win->scr = DefaultScreen(win->dpy);

    win->win = XCreateSimpleWindow(win->dpy, root, 100, 0,
                                   win->width, win->height, 0,
                                   BlackPixel(win->dpy, win->scr), BlackPixel(win->dpy, win->scr));

    win->gc = DefaultGC(win->dpy, win->scr);

    win->quit_code = XKeysymToKeycode(win->dpy, XStringToKeysym("Q"));

    XSelectInput(win->dpy, win->win,
                 KeyPressMask | StructureNotifyMask | ExposureMask);

    XMapWindow(win->dpy, win->win);

    XStoreName(win->dpy, win->win, "hello_world");

    XStoreName(win->dpy, win->win, "skia-test");
    XClassHint *classHint = XAllocClassHint();
    classHint->res_name = const_cast<char *>("test");
    classHint->res_class = const_cast<char *>("test");
    XSetClassHint(win->dpy, win->win, classHint);
    XFree(classHint);

    win->ximage = nullptr;
    win_init_skia(win);
}

static void win_init_skia(win_t *win)
{
    if (win->ximage)
    {
        XDestroyImage(win->ximage);
        win->ximage = nullptr;
    }

    Visual *visual = DefaultVisual(win->dpy, DefaultScreen(win->dpy));
    XWindowAttributes attrs;
    XGetWindowAttributes(win->dpy, win->win, &attrs);
    win->ximage = XCreateImage(
        win->dpy, attrs.visual,
        attrs.depth, ZPixmap, 0,
        (char *)malloc(win->width * win->height * 4),
        win->width, win->height, 32, 0);

    // 初始化 Skia
    SkImageInfo info = SkImageInfo::MakeN32Premul(win->width, win->height);
    win->bitmap.installPixels(info, win->ximage->data, win->width * 4);
}

static void
win_deinit(win_t *win)
{
    XDestroyImage(win->ximage);
    XDestroyWindow(win->dpy, win->win);
}

static void
win_handle_events(win_t *win)
{
    XEvent xev;

    while (1)
    {
        XNextEvent(win->dpy, &xev);
        switch (xev.type)
        {
        case KeyPress:
        {
            XKeyEvent *kev = &xev.xkey;

            if (kev->keycode == win->quit_code)
            {
                return;
            }
        }
        break;
        case ConfigureNotify:
        {
            XConfigureEvent *cev = &xev.xconfigure;

            win->width = cev->width;
            win->height = cev->height;
            win_init_skia(win);
        }
        break;
        case Expose:
        {
            XExposeEvent *eev = &xev.xexpose;
            // printf("event:>Expose\n");
            if (eev->count == 0)
            {

                win_draw(win);
            }
        }
        break;
        }
    }
}
