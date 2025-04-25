#pragma once

#ifndef X11_SKIA_WIN_H
#define X11_SKIA_WIN_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "include/core/SkBitmap.h"

// 类型定义
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
    Pixmap buffer;
} win_t;

// 函数声明
#ifdef __cplusplus
extern "C"
{
#endif

    void win_init(win_t *win);
    void win_deinit(win_t *win);
    void win_handle_events(win_t *win, void (*draw_fn)(win_t *));
    void win_init_skia(win_t *win);
    win_t dpy_init();

#ifdef __cplusplus
}
#endif

#endif // X11_SKIA_WIN_