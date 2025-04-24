https://skia.org/docs/user/api/skcanvas_overview/

https://chromium.googlesource.com/skia.git/+/chrome/m42/site/user/sample/hello.md

https://skia.org/docs/user/build/

```bash
bin/gn gen out/Static --args='is_official_build=true  is_component_build=false skia_use_system_libpng=true skia_use_system_libwebp=true skia_use_system_freetype2=false target_cpu="x64"'

ninja -C out/Static

g++ ./main.cpp -std=c++17 -I /home/zsy/Documents/zsy/github/skia -L /home/zsy/Documents/zsy/github/skia/out/Static -lskia -o skia_demo -lpthread -ldl -lm -ljpeg -lpng -lz -lwebp -lwebpdemux

```

```
sudo apt install libjpeg-dev zlib1g-dev libpng-dev

```

```
bin/gn gen out/Static --args='is_official_build=true  is_component_build=false skia_use_system_libpng=true  skia_use_libpng_encode=true  skia_use_libpng_decode=true skia_use_system_libwebp=true skia_use_libwebp_encode=true skia_use_libwebp_decode=true skia_use_system_freetype2=false target_cpu="x64"'

ninja -C out/Static

g++ ./main.cpp -std=c++17 -I /home/zsy/Documents/zsy/github/skia -L /home/zsy/Documents/zsy/github/skia/out/Static -lskia -o skia_demo -lpthread -ldl -lm -lpng -lz
```
