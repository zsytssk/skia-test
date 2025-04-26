#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkPath.h"

#include "win_skia.h"

static void win_draw(win_t *win);
int main(int argc, char *argv[])
{
    win_t win = dpy_init();
    win_init(&win);
    // win_draw(&win);

    win_handle_events(&win, win_draw);

    win_deinit(&win);
    return 0;
}

static void win_draw(win_t *win)
{

    XLockDisplay(win->dpy);
    // 使用 Skia 绘制
    SkCanvas canvas(win->bitmap);

    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPath path;
    path.moveTo(R, 0.0f);

    const int n = 5;
    for (int i = 1; i < n; i++)
    {
        SkScalar thela = 3 * i * TAU / n;
        path.lineTo(R * cos(thela), R * sin(thela));
    }
    path.close();
    SkPaint p;

    p.setColor(SK_ColorRED);
    p.setAntiAlias(true);
    canvas.clear(SK_ColorWHITE);
    canvas.translate(0.5f * scale, 0.5f * scale);
    canvas.drawPath(path, p);

    XPutImage(
        win->dpy, win->buffer, win->gc, win->ximage,
        0, 0, 0, 0,
        win->width, win->height);

    // 一次性从Pixmap复制到窗口
    XCopyArea(
        win->dpy, win->buffer, win->win, win->gc,
        0, 0, win->width, win->height,
        0, 0);

    XFlush(win->dpy);

    XUnlockDisplay(win->dpy); // 解锁显示
}
