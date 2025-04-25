// main.cpp
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

int main()
{
    // 初始化 X11
    Display *display = XOpenDisplay(nullptr);
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen),
        0, 0, 800, 600, 1,
        BlackPixel(display, screen),
        WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    XStoreName(display, window, "skia-test");
    XClassHint *classHint = XAllocClassHint();
    classHint->res_name = const_cast<char *>("test");
    classHint->res_class = const_cast<char *>("test");
    XSetClassHint(display, window, classHint);
    XFree(classHint);

    // 创建兼容的 XImage
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);
    XImage *ximage = XCreateImage(
        display, attrs.visual,
        attrs.depth, ZPixmap, 0,
        (char *)malloc(800 * 600 * 4),
        800, 600, 32, 0);

    // 初始化 Skia
    SkGraphics::Init();
    SkImageInfo info = SkImageInfo::MakeN32Premul(800, 600);
    SkBitmap bitmap;
    bitmap.installPixels(info, ximage->data, 800 * 4);

    // 主循环
    XEvent e;
    while (true)
    {
        XNextEvent(display, &e);
        if (e.type == Expose)
        {
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
            GC gc = DefaultGC(display, screen);
            XPutImage(
                display, window, gc, ximage,
                0, 0, 0, 0,
                800, 600);
        }
        if (e.type == KeyPress)
            break;
    }

    // 清理资源
    XDestroyImage(ximage);
    XCloseDisplay(display);
    return 0;
}