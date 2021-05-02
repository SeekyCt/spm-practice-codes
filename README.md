# Super Paper Mario Practice Codes
A collection of code mods I've made to help with speedrunning practice and reverse engineering the game.

## Preview Video
https://youtu.be/Eqa4RUWRF3E

## Installing
See the [installation guide](https://github.com/SeekyCt/spm-practice-codes/blob/main/INSTALLING.md).

## Usage
See the [manual](https://github.com/SeekyCt/spm-practice-codes/blob/main/MANUAL.md).

## Bugs and Suggestions
If you come across any bugs or have any suggestions for features, feel free to create an issue or contact me about it on Discord (Seeky#4628).

## Branches
### main
The main branch contains the source of the latest release of the mod.
### dev
The dev contains in-development features and usually isn't ported to or tested on regions other than PAL.

## Building
To compile this yourself, you'll need the following:
* devkitPPC
* PistonMiner's elf2rel, from either the SPM Rel Loader repo or the original TTYD Tools repo
* The TTYDTOOLS environment variable set to the folder outside of the `bin` folder with your compiled `elf2rel` in (so `$(TTYDTOOLS)/bin/elf2rel` will point to it)

Once that's set up you can use `make rgX` to build region `rg` ('eu', 'us', 'jp' or 'kr') revision `X` (0-1 for eu & jp, 0-2 for us, 0 for kr) or just `make` to build for all regions and revisions.

## Credits
* JohnP55 for help and suggestions creating the map change teleport effect.
* This mod was made using the [SPM Rel Loader](https://github.com/SeekyCt/spm-rel-loader), which is based on the TTYD rel loader by PistonMiner, Zephiles, Jdaster64 and JasperRLZ.
