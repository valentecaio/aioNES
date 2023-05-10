
## Compilation

To compile this `libretro` core use `cmake`. Your C++ compiler must support the C++14/1y standard, eg. `clang >= 3.4` or `gcc >= 4.9`.
 
``` shell
$ mkdir build
$ cd build
$ cmake ..
$ retroarch -v -L ./libaioNES_libretro.so
```
