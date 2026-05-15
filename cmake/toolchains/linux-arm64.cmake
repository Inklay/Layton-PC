set(LAYTON_TARGET_OS   "linux")
set(LAYTON_TARGET_ARCH "x64")

set(CMAKE_SYSTEM_NAME      Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(WARNING
        "Compiling Linux targets from Windows natively is not supported.\n"
        "Please use WSL2 or a Linux Docker container instead.")
endif()
