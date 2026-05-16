cmake_minimum_required(VERSION 3.20)
project("Layton PC" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------
set(SOURCES
    source/FileFormat/Compression/Huffman.cpp
    source/FileFormat/Compression/RLE.cpp
    source/FileFormat/Compression/LZSS.cpp
    source/FileFormat/Custom/Anim.cpp
    source/FileFormat/Conversion/GFX.cpp
    source/FileFormat/Conversion/BGX.cpp
    source/FileFormat/Conversion/Converter.cpp
    source/FileFormat/Conversion/MID.cpp
    source/FileFormat/Conversion/MODS.cpp
    source/FileFormat/Conversion/NFTR.cpp
    source/FileFormat/Conversion/SADL.cpp
    source/FileFormat/Conversion/SDAT.cpp
    source/FileFormat/Conversion/LPCK.cpp
    source/Game/Game.cpp
    source/Game/Save.cpp
    source/Game/Layton1/Layton1.cpp
    source/Game/Layton1/Layton1Save.cpp
    source/Game/Layton1/Scene/CreateSave.cpp
    source/Game/Layton1/Scene/Intro.cpp
    source/Game/Layton1/Scene/IntroCarDialogue1.cpp
    source/Game/Layton1/Scene/IntroCarDialogue2.cpp
    source/Game/Layton1/Scene/TitleDrop.cpp
    source/Game/Layton1/Scene/TitleScreen.cpp
    source/Game/Layton1/Scene/Map/Map.cpp
    source/Game/Layton1/Scene/Map/DrawBridge.cpp
    source/Game/Layton1/Scene/Puzzle/Puzzle.cpp
    source/Game/Layton1/Scene/Puzzle/Puzzle1.cpp
    source/Game/Scene/Cinematic.cpp
    source/Game/Scene/Dialogue.cpp
    source/Game/Scene/Scene.cpp
    source/Game/Sprite/AnimatedSprite.cpp
    source/Game/Sprite/ClickableSprite.cpp
    source/Game/Sprite/ParallaxSprite.cpp
    source/Game/Sprite/Sprite.cpp
    source/Game/Sprite/SpriteSheet.cpp
    source/Game/Sprite/CharSprite.cpp
    source/Game/Sprite/TextSprite.cpp
    source/Utils/FileUtils.cpp
    source/Utils/FolderUtils.cpp
    source/Utils/SdlUtils.cpp
    source/Utils/StringUtils.cpp
    source/LaytonPC.cpp
)

add_executable(LaytonPC ${SOURCES})

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
target_include_directories(LaytonPC PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/dependencies/SDL3/include
    ${CMAKE_SOURCE_DIR}/dependencies/SDL3_image/include
    ${CMAKE_SOURCE_DIR}/dependencies/FFmpeg_lgpl/include
)

# ---------------------------------------------------------------------------
# Compiler options
# ---------------------------------------------------------------------------
target_compile_options(LaytonPC PRIVATE
    # UTF-8 source & execution charset (MSVC) / equivalent for GCC/Clang
    $<IF:$<CXX_COMPILER_ID:MSVC>,
        /W3 /WX /utf-8 /MP,
        -Wall -Wextra -Werror -finput-charset=UTF-8>
)

# ---------------------------------------------------------------------------
# Preprocessor definitions
# ---------------------------------------------------------------------------
target_compile_definitions(LaytonPC PRIVATE
    UNICODE _UNICODE
    $<$<CONFIG:Debug>:_DEBUG _CONSOLE>
)

# ---------------------------------------------------------------------------
# Link-time optimisation (Release only — mirrors WholeProgramOptimization)
# ---------------------------------------------------------------------------
set_target_properties(LaytonPC PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
)

# ---------------------------------------------------------------------------
# Target OS / Arch — set by the toolchain file, with fallbacks
# ---------------------------------------------------------------------------
if(NOT DEFINED LAYTON_TARGET_OS)
    if(WIN32)
        set(LAYTON_TARGET_OS "windows")
    else()
        set(LAYTON_TARGET_OS "linux")
    endif()
endif()

if(NOT DEFINED LAYTON_TARGET_ARCH)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
        set(LAYTON_TARGET_ARCH "arm64")
    else()
        set(LAYTON_TARGET_ARCH "x64")
    endif()
endif()

set(ARCH_SUFFIX "${LAYTON_TARGET_OS}_${LAYTON_TARGET_ARCH}")
message(STATUS "Target: ${LAYTON_TARGET_OS}-${LAYTON_TARGET_ARCH}  (lib suffix: ${ARCH_SUFFIX})")

target_link_directories(LaytonPC PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/SDL3/lib/${ARCH_SUFFIX}
    ${CMAKE_SOURCE_DIR}/dependencies/SDL3_image/lib/${ARCH_SUFFIX}
    ${CMAKE_SOURCE_DIR}/dependencies/FFmpeg_lgpl/lib/${ARCH_SUFFIX}
)

target_link_libraries(LaytonPC PRIVATE
    avformat
    avutil
    avcodec
    swscale
    SDL3
    SDL3_image
)

# ---------------------------------------------------------------------------
# Add symlink for UNIX systems
# ---------------------------------------------------------------------------
if(UNIX)
    add_custom_target(prebuild_deps
        COMMAND bash ${CMAKE_SOURCE_DIR}/dependencies/createSymLink.sh ${ARCH_SUFFIX}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/
    )

    add_dependencies(LaytonPC prebuild_deps)
endif()

# ---------------------------------------------------------------------------
# Subsystem: Console in Debug, Windows in Release (MSVC / MinGW)
# ---------------------------------------------------------------------------
if(MSVC)
    target_link_options(LaytonPC PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
    )
elseif(MINGW)
    target_link_options(LaytonPC PRIVATE
        $<$<CONFIG:Debug>:-mconsole>
        $<$<CONFIG:Release>:-mwindows>
    )
endif()

# ---------------------------------------------------------------------------
# INSTALL RULES (Windows + Linux AppImage friendly)
# ---------------------------------------------------------------------------

# Binary
install(TARGETS LaytonPC
    RUNTIME DESTINATION .
)

# Docs
install(FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/LICENSE
    DESTINATION .
)

# Third-party licenses folder
install(DIRECTORY
    ${CMAKE_SOURCE_DIR}/licenses/
    DESTINATION licenses
)

# Third-party executables
install(DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/${ARCH_SUFFIX}/
    DESTINATION bin
)

if(WIN32)
    # Windows DLL
    file(GLOB_RECURSE DLLS
        "${CMAKE_SOURCE_DIR}/dependencies/*/bin/${ARCH_SUFFIX}/*.dll"
    )

    install(FILES
        ${DLLS}
        DESTINATION .
    )

    # VC redist
    install(DIRECTORY
        "${CMAKE_SOURCE_DIR}/_commonRedist/${ARCH_SUFFIX}/"
        DESTINATION _commonRedist
    )
endif()
