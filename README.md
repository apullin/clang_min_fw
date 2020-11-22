# `clang_min_fw`

This repository contains a minimum working example of a CMake config to build C source into
a working firmware image for an ARM Cortex-M0 microcontroller.

The output ELF has been tested on an STM32L082CZ microcontroller via a debugger and appears
to load and run.

## Building

This _should_ build out-of-the-box with the version of clang that Apple provides in
the XCode command-line tools.  
(which I think is pretty interesting, to use the exact same compiler that, say, CLion
uses by default)

Standard CMake build:
```
mkdir build
cd build
cmake ../
make 
```

## Notes

This build is linked against `libc_nano` and `libnosys`. As far as I have, found, there is not an
equivalent in clang for the `--specs=nosys.specs` inputs.

Additionally, I am not entirely sure of the implications of building and running with
`libnosys`, other than avoiding the provision of empty system-lib function stubs,
and `libc_nano`, other than using a smaller, lower-featured implementation of newlib.

I tested the use of `float` types in the trivial `func` implementation, and I observed
spurious values via a debugger; TODO. 

## Current issues

The explicit linking with crt0, crti, crtn, crtbegin, and crtend objects from 
arm-gcc suggests that I do not have something configured correctly. 
Similarly, the convoluted path to pick up the right libraries indicates wrong or missing
flags, options, etc.

During testing, I found that without a lot of caution around the paths and the various
versions of `libcc.a` and the `crt*.o` files, I would end up with Thumb code linked
into my ELF, resulting in a hardfault.