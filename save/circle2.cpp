#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/encode/SkPngEncoder.h"
#include "include/utils/SkNullCanvas.h"
#include "include/core/SkStream.h"

#include <fstream>
#include <filesystem>
#include <iostream>

int main()
{

    std::filesystem::path outputPath = "out";
    if (!std::filesystem::exists(outputPath))
    {
        std::filesystem::create_directories(outputPath);
    }

    SkImageInfo info = SkImageInfo::MakeN32Premul(256, 256);
    auto surface = SkSurfaces::Raster(info);
    if (!surface)
    {
        fprintf(stderr, "Failed to create SkSurface\n");
        return 1;
    }

    SkCanvas *canvas = surface->getCanvas();

    SkPaint paint;

    paint.setAntiAlias(true);
    canvas->drawCircle(128, 128, 90, paint);
    paint.setColor(SK_ColorWHITE);
    canvas->drawCircle(86, 86, 20, paint);
    canvas->drawCircle(160, 76, 20, paint);
    canvas->drawCircle(140, 150, 35, paint);

    auto image = surface->makeImageSnapshot();
    if (!image)
    {
        fprintf(stderr, "Failed to make image snapshot\n");
        return 1;
    }

    SkPixmap pixmap;
    if (!image->peekPixels(&pixmap))
    {
        printf("Failed to peek pixels.\n");
        return 1;
    }
    SkPngEncoder::Options options;
    options.fFilterFlags = SkPngEncoder::FilterFlag::kSub;

    SkDynamicMemoryWStream stream;
    bool success = SkPngEncoder::Encode(&stream, pixmap, options);
    if (!success)
    {
        fprintf(stderr, "Failed to encode PNG with SkPngEncoder\n");
        return 1;
    }
    sk_sp<SkData> pngData = stream.detachAsData();
    SkFILEWStream out("out/circle.png");
    stream.copyTo(&out);
    out.write(pngData->data(), pngData->size());

    return 0;
}