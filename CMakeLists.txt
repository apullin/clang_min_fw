cmake_minimum_required(VERSION 3.17)
project(clang_min_fw C)
set(CMAKE_C_LINK_FLAGS "")

set(CMAKE_C_STANDARD 99)

enable_language( ASM )

# Apple clang, provided by xcode (I think)
#set(CMAKE_C_COMPILER clang )
#set(CMAKE_ASM_COMPILER clang )

set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

# CMake on OSX will apparently add "-isysroot" without prompting; these remove that addition
set( CMAKE_OSX_SYSROOT "/" )
set( CMAKE_OSX_DEPLOYMENT_TARGET "" )

set( ARM_GCC "arm-none-eabi-gcc" )
set( ARCH_CFLAGS_args -mthumb -mcpu=cortex-m0plus -mfloat-abi=soft -mfpu=auto )
set( ARCH_CFLAGS     "-mthumb -mcpu=cortex-m0plus -mfloat-abi=soft -mfpu=auto" )

message("process : ${ARM_GCC} ${ARCH_CFLAGS} -print-sysroot")

# Get the arm-gcc sysroot
execute_process( COMMAND ${ARM_GCC} ${ARCH_CFLAGS_args} -print-sysroot
        OUTPUT_VARIABLE ARM_CORTEXM_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
message( "ARM_CORTEXM_SYSROOT = ${ARM_CORTEXM_SYSROOT}" )

# Get the arm-gcc target/arch-specific library path
execute_process( COMMAND ${ARM_GCC} ${ARCH_CFLAGS_args} -print-multi-directory
        OUTPUT_VARIABLE ARM_CORTEXM_MULTI_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
message( "ARM_CORTEXM_MULTI_DIR = ${ARM_CORTEXM_MULTI_DIR}" )

# Get the arm-gcc libc builtins library (for this arch)
execute_process( COMMAND ${ARM_GCC} ${ARCH_CFLAGS_args} -print-libgcc-file-name
        OUTPUT_VARIABLE ARM_CORTEXM_BUILTINS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
message( "ARM_CORTEXM_BUILTINS = ${ARM_CORTEXM_BUILTINS}" )

set(CRT_SUPPORT_ROOT "${ARM_CORTEXM_SYSROOT}/../lib/gcc/arm-none-eabi/9.2.1/${ARM_CORTEXM_MULTI_DIR}" )
set( CRT_SUPPORT_LINK "-l:crt0.o -l:crtbegin.o -l:crtend.o -l:crti.o -l:crtn.o")

# clang needs the arm-gcc sysroot to use libc, std headers, etc
set( CMAKE_SYSROOT ${ARM_CORTEXM_SYSROOT} )

# Chip-specific linker script
set( LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32L072CZYx_FLASH.ld" )

# TODO: Is there a way to not have to exhaustively set all these options sets?
set( CLANG_ARCH_FLAGS "--target=arm-none-eabi -mthumb -march=armv6m -mcpu=cortex-m0 -mfloat-abi=soft -mfpu=none -ffreestanding -g -O0" )
set( CMAKE_C_FLAGS ${CLANG_ARCH_FLAGS} )
set( CMAKE_ASM_FLAGS ${CLANG_ARCH_FLAGS} )

# Carefully set up the linker to use arm-gcc builtins, the right arch version of libcc AND all the CRT objects
set( CMAKE_EXE_LINKER_FLAGS "-nostdlib -T${LINKER_SCRIPT} -L${ARM_CORTEXM_SYSROOT}/lib/${ARM_CORTEXM_MULTI_DIR} \
                                       -L${CRT_SUPPORT_ROOT} ${CRT_SUPPORT_LINK} \
                                       ${ARM_CORTEXM_BUILTINS} ${CRT_SUPPORT} -lc_nano -lnosys -lm" )

# Actual sources ; minimum working example
add_executable(clang_min_fw.elf src/main.c src/func.c inc/func.h src/system_stm32l0xx.c src/startup_stm32l072xx.s )
include_directories( inc )