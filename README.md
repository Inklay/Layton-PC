# Layton-PC

A C++ rewrite of the DS Layton game that can read DS roms assets to run on PC. The end goal is to be able able to natively play _any_ Layton game on your computer (you would still need the game's assets to do that) but for now I'm focusing on the 4 DS games.

The project will only host the code C++ code that allows to convert the DS game assets and play the game, there will never be any assets hosted on this repository. To be able to use this project you will need a legally obtained dump of your Layton game.

For now I'm working on the data conversation from an unpacked DS roms, once the conversion side of thing is done I'll work on adding the basics of the gameplay (puzzles, maps, dialogues, cinematics...) Since this is a full rewrite of the games, it will take some time.

## Building

You need to use C++17 or later to, build this project.
The only dependencies needed for now are SDL3 (I'm specifically using SDL3-3.4.2) and SDL3_Image (3.4.2)
You will also need ffmpeg in a bin folder next to the game's executable

## Thanks and credits:

- The LZSS decompressor code is inspired by [magical's nlzss](https://github.com/magical/nlzss/tree/master)
- The PCM and BG converter as well as the RLE decompressor are inspired by [patataofcourse's Flora](https://github.com/patataofcourse/Flora/tree/main)
- The PNG file writter is from [nothings' stb](https://github.com/nothings/stb/tree/master)
- The Huffman decompressor and the GFX conversion are inpired by [pleonex's tinke](https://github.com/pleonex/tinke)
- The whole game rendering is using [SDL3](https://wiki.libsdl.org/SDL3/FrontPage)
- PNG file support is from [SDL3_Image](https://wiki.libsdl.org/SDL3_image/FrontPage)
- The MODS conversion is using [FFMPEG](https://ffmpeg.org/)
- The SADL conversion is using [vgmstream](https://github.com/vgmstream/vgmstream)
- The SDAT data extration is using [vgmtrans](https://github.com/vgmtrans/vgmtrans)
- The MID to WAV conversion is using [fluidsynth](https://github.com/fluidsynth/fluidsynth)
