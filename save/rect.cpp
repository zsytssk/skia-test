#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/encode/SkPngEncoder.h"
#include "include/utils/SkNullCanvas.h"
#include "include/core/SkStream.h"

#include <fstream>

int main()
{
    // Step 1: Create a bitmap
    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::MakeN32Premul(100, 100));
    bitmap.eraseColor(SK_ColorRED);

    // Step 2: Get pixmap
    SkPixmap pixmap;
    if (!bitmap.peekPixels(&pixmap))
    {
        printf("Failed to peek pixels.\n");
        return 1;
    }

    // Step 3: Encode to PNG using a SkDynamicMemoryWStream
    SkPngEncoder::Options options;
    options.fFilterFlags = SkPngEncoder::FilterFlag::kSub;

    SkDynamicMemoryWStream stream;
    bool success = SkPngEncoder::Encode(&stream, pixmap, options);
    if (!success)
    {
        printf("Encoding failed.\n");
        return 1;
    }

    // Step 4: Convert stream to SkData and save
    sk_sp<SkData> pngData = stream.detachAsData();
    std::ofstream out("out/rect.png", std::ios::binary);
    out.write((const char *)pngData->data(), pngData->size());
    out.close();

    printf("Saved to rect.png\n");
    return 0;
}
