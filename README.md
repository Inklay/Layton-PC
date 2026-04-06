# Layton-PC

A C++ rewrite of the DS Layton game that can read DS roms assets to run on PC. The end goal is to be able able to natively play *any* Layton game on your computer (you would still need the game's assets to do that) but for now I'm focusing on the 4 DS games.

The project will only host the code C++ code that allows to convert the DS game assets and play the game, there will never be any assets hosted on this repository. To be able to use this project you will need a legally obtained dump of your Layton game.

For I'm working on the data conversation from an unpacked DS roms, once the conversion side of thing is done I'll work on adding the basics of the gameplay (puzzles, maps, dialogues, cinematics...) Since this is a full rewrite of the games, it will take time.

## Building
You need to use C++17 or later to, build this project.
The only dependency needed for now is SDL3 (I'm specifically using SDL3-3.4.4)

## Thanks and credits:

- The LZSS decompressor code is inspired by [magical's nlzss](https://github.com/magical/nlzss/tree/master)
- The PCM and BG converter as well as the RLE decompressor are inspired by [patataofcourse's Flora](https://github.com/patataofcourse/Flora/tree/main)
- The PNG file writter is from [nothings's stb](https://github.com/nothings/stb/tree/master)
