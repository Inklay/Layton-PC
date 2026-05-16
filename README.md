# Layton-PC

A C++ rewrite of the DS Layton game that can read DS roms assets to run on PC. The end goal is to be able to natively play _any_ Layton game on your computer (you would still need the game's assets to do that) but for now I'm focusing on the 4 DS games.

The project will only host the code C++ code that allows to convert the DS game assets and play the game, there will never be any assets hosted on this repository. To be able to use this project you will need a legally obtained dump of your Layton game.

For now I'm working on adding the basics of the gameplay (I'm almost done writing the map class). Since this is a full rewrite of the games, it will take some time.

## Usage

For now no prebuilt binairies are posted since the project isn't really usable so you'll have to build it yourself.
You'll also need to put the corresponding files in the bin/ folder.

## Supported platforms

Currently only Windows x64 is fully supported but I'm currently working on adding Windows arm64 support.

Remaining steps for Windows arm64 full support:
- somehow build fluidsynth for arm
- build vgmstream-cli for arm

Support is also planned for:
- Linux x64 (Cmake target existing but no dependencies is included/built yet)
- Linux arm64 (Cmake target existing but no dependencies is included/built yet)
- MacOS arm64 (Apple Silicon)

## Building

This project can be built using and needs C++20 or later to build.

All libs for currently supported platforms are in the repo but if you somehow need to add them yourself the only dependencies needed for now are SDL3 (I'm specifically using SDL3-3.4.8) and SDL3_Image (3.4.4) and some FFmpeg libs.

## Thanks and credits:

- The LZSS decompressor code is inspired by [magical's nlzss](https://github.com/magical/nlzss/tree/master)
- The PCM and BG converter as well as the RLE decompressor are inspired by [patataofcourse's Flora](https://github.com/patataofcourse/Flora/tree/main)
- The PNG file writter is from [nothings' stb](https://github.com/nothings/stb/tree/master)
- The Huffman decompressor and the GFX conversion are inpired by [pleonex's tinke](https://github.com/pleonex/tinke)
- The whole game rendering is using [SDL3](https://wiki.libsdl.org/SDL3/FrontPage)
- PNG file support is from [SDL3_Image](https://wiki.libsdl.org/SDL3_image/FrontPage)
- The MODS conversion and video playback are using [FFmpeg](https://ffmpeg.org/)
- The SADL conversion is using [vgmstream](https://github.com/vgmstream/vgmstream)
- The SDAT data extration is using [vgmtrans](https://github.com/vgmtrans/vgmtrans)
- The MID to WAV conversion is using [fluidsynth](https://github.com/fluidsynth/fluidsynth)
- The Sound effect extration from the SADT file is using [oreo639's sdatxtract](https://github.com/oreo639/sdatxtract)
