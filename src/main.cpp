#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/encode/SkPngEncoder.h"
#include "include/utils/SkNullCanvas.h"
#include "include/core/SkStream.h"
#include "include/core/SkGraphics.h"

#include "win.cpp"

int main(int argc, char *argv[])
{
    win_t win = dpy_init();
    win_init(&win);
    win_draw(&win);

    win_handle_events(&win);

    win_deinit(&win);
    XCloseDisplay(win.dpy);
    return 0;
}

static void
win_draw(win_t *win)
{
    Visual *visual = DefaultVisual(win->dpy, DefaultScreen(win->dpy));
    XWindowAttributes attrs;
    XGetWindowAttributes(win->dpy, win->win, &attrs);
    XImage *ximage = XCreateImage(
        win->dpy, attrs.visual,
        attrs.depth, ZPixmap, 0,
        (char *)malloc(win->width * win->height * 4),
        win->width, win->height, 32, 0);

    // 初始化 Skia
    SkGraphics::Init();
    SkImageInfo info = SkImageInfo::MakeN32Premul(win->width, win->height);
    SkBitmap bitmap;
    bitmap.installPixels(info, ximage->data, win->width * 4);

    // 使用 Skia 绘制
    SkCanvas canvas(bitmap);
    canvas.clear(SK_ColorWHITE);

    SkPaint paint;

    paint.setAntiAlias(true);
    canvas.drawCircle(128, 128, 90, paint);
    paint.setColor(SK_ColorWHITE);
    canvas.drawCircle(86, 86, 20, paint);
    canvas.drawCircle(160, 76, 20, paint);
    canvas.drawCircle(140, 150, 35, paint);

    // 显示到窗口
    GC gc = DefaultGC(win->dpy, win->scr);
    XPutImage(
        win->dpy, win->win, gc, ximage,
        0, 0, 0, 0,
        win->width, win->height);
}
