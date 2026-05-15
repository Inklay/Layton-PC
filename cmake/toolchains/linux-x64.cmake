set(LAYTON_TARGET_OS   "linux")
set(LAYTON_TARGET_ARCH "arm64")

set(CMAKE_SYSTEM_NAME      Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(WARNING
        "Compiling Linux ARM64 targets from Windows natively is not supported.\n"
        "Please use WSL2 or a Linux Docker container instead.")
endif()