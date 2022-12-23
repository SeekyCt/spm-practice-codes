# Super Paper Mario Practice Codes
A collection of code mods I've made to help with speedrunning practice and reverse engineering the game.

## Preview Video
https://youtu.be/Eqa4RUWRF3E

## Installing
See the [installation guide](https://github.com/SeekyCt/spm-practice-codes/blob/main/INSTALLING.md).

## Updating
See the [updating guide](https://github.com/SeekyCt/spm-practice-codes/blob/main/UPDATING.md).

## Usage
See the [manual](https://github.com/SeekyCt/spm-practice-codes/blob/main/MANUAL.md).

## Bugs and Suggestions
If you come across any bugs or have any suggestions for features, feel free to create an issue or contact me about it on on Discord in the #tools-dev channel of the [SPM Speedrunning Server](https://discord.gg/dbd733H).

## Branches
### main
The main branch contains the source of the latest release of the mod.
### dev
The dev branch contains in-development features and usually isn't ported to or tested on regions other than PAL. Settings files generated in builds from this branch may also not be supported in future versions.

## Building
To compile this yourself, you'll need the following:
* devkitPPC
* ninja
* Python 3
    - 3.8 is known to work, earlier versions may too
* The python modules from `requirements.txt` (`pip install -r requirements.txt`)
* The fork of PistonMiner's elf2rel from the [SPM Rel Loader repo](https://github.com/SeekyCt/spm-rel-loader/releases/tag/elf2rel-24-6-2021)
    * For v8 and above, the 21-12-2021 build is required. v5-7 require the 24-6-2021 build. Older versions are compatible with any build
* The ELF2REL environment variable set to the path to your compiled `elf2rel` executable

Once that's set up:
* Run `configure.py`
    * Versions can be listed as arguments to enable them, by default all will be enabled
* Run `ninja`
    * Versions can be listed as arguments to build them (must be enabled in `configure.py`), by default all will be built

## Credits
* This mod was made using the [SPM Rel Loader](https://github.com/SeekyCt/spm-rel-loader), which is based on the TTYD rel loader by PistonMiner and Zephiles.
* JohnP55 for help and suggestions creating the map change teleport effect.
* TheLordScruffy for the Dolphin & Riivolution detection methods.
* stebler for `tools/incbin.S`.
