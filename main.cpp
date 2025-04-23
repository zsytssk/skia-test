#include "include/core/SkCanvas.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkImage.h"
#include "include/core/SkData.h"
#include "include/core/SkStream.h"

int main()
{
    SkImageInfo info = SkImageInfo::MakeN32Premul(256, 256);
    auto surface = SkSurfaces::Raster(info);
    SkCanvas *canvas = surface->getCanvas();

    SkPaint paint;
    paint.setColor(SK_ColorRED);
    canvas->drawCircle(128, 128, 100, paint);

    auto image = surface->makeImageSnapshot();
    auto data = image->refEncodedData();
    SkFILEWStream out("circle.png");
    out.write(data->data(), data->size());

    return 0;
}