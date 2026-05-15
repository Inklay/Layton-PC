set(LAYTON_TARGET_OS   "windows")
set(LAYTON_TARGET_ARCH "arm64")

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    if(NOT DEFINED LLVM_MINGW_ROOT)
        message(FATAL_ERROR
            "LLVM_MINGW_ROOT must be set to the llvm-mingw installation path.\n"
            "Example: cmake ... -DLLVM_MINGW_ROOT=/opt/llvm-mingw")
    endif()

    set(CMAKE_SYSTEM_NAME      Windows)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)

    set(CMAKE_C_COMPILER   ${LLVM_MINGW_ROOT}/bin/aarch64-w64-mingw32-clang)
    set(CMAKE_CXX_COMPILER ${LLVM_MINGW_ROOT}/bin/aarch64-w64-mingw32-clang++)
    set(CMAKE_RC_COMPILER  ${LLVM_MINGW_ROOT}/bin/aarch64-w64-mingw32-windres)

    set(CMAKE_FIND_ROOT_PATH ${LLVM_MINGW_ROOT}/aarch64-w64-mingw32)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()
