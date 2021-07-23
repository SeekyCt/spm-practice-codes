# Manual

## Opening the Menu
The menu can currently be opened at any time the game's pause menu can (although this is probably going to be made less strict in a future version) by pressing `B`.

The menu can then be navigated with the D-Pad, `2` to select, `1` to go back, and `B` to fully close the menu from any section.

## Map Warping
The `Change Map` option in the menu allows you to warp to any map with an entrance of your choice by their internal names.

The first 3 letters of the name are the area (such as `he1` for 1-1, `ta3` for 3-3, `mac` for Flipside) and the 2 numbers are the number of the map within that area. The entrance column allows you to choose how the room is entered, with the `default` option using the default entrance for the map if it has one, or spawning you at the default coordinates for the map otherwise. These can scrolled through using the D-Pad.

Once you've chosen the map you'd like to warp to and the entrance you'd like to use, press `2`.

A full list of map names can be found [here](https://docs.google.com/document/d/10w4CS5oNBOHHYtM9OrNUYM7GIqNxIaR-b_Sr8FSG7Pk/edit). 

By default a Dimentio teleport animation will play when you do this (thanks to JohnP55 for the idea and documentation of relevant functions), but you can turn that off by pressing `-` in the menu if you'd like.

## Pit Warping
The `Warp To Pit Floor` option in the menu allows you to warp to any floor of either Pit of 100 Trials.

In the left column you can choose which pit you'd like to warp to, and in the right column you can choose the floor of that pit. These can be scrolled through using the D-Pad.

Once you've chosen the floor and pit you'd like to warp to, press `2`.

By default a Dimentio teleport animation will play when you do this (thanks to JohnP55 for the idea and documentation of relevant functions), but you can turn that off by pressing `-` in the menu if you'd like.

## HUD Displays
The `Edit HUD Display Options` option in the menu allows you to enable and disable various displays on the screen. The list can be navigated with the D-Pad and `2` can be pressed to switch between Enabled and Disabled.

### Map & Door Name
The Map & Door Name Display shows the current map name (without the underscore since the game's font doesn't support them) and the name of the door that it was entered with in the top left of the screen.

### Position
The Position Display shows your current co-ordinates in the top left of the screen.

The `Position Update Delay` option allows you to change the amount of frames between each time the coordinates are read and the display is updated.

The `Position Decimal Places` option allows you to set how many decimal places the coordinates are displayed to.

## Logging Options
The `Edit Logging Options` option in the menu allows you to enable and disable various things being printed to an on-screen console or OSReport (for Dolphin). The list can be navigated with the D-Pad and `2` can be pressed to switch between Enabled and Disabled.

### Script Tracking
The `Evt Script Tracking` option will log the address of any script that starts.

### Script Variable Tracking
The `Evt Variable Tracking` option will log whenever a saved script variable (LSW, LSWF, GSW, GSWF) is changed.

### Script Debug Instruction
The `Evt Debug Instructions` option will log messages passed into the debug script instructions `debug_put_msg` and `debug_put_reg`.

## Script Variable Editor
The `Edit Script Variables` option in the menu allows you to view and edit the values of any GW, GF, GSW, GSWF, LSW or LSWF. The variable type (first few letters), variable number (first 4 decimal digits) and value (final 8 hex digits) can be scrolled through with the D-Pad.

## Inventory Editor
The `Edit Inventory` option in the menu allows you add and remove items, characters and pixls from your inventory. The menu can be navigated with the D-Pad, first by selecting an itmem type in the column on the left then pressing right to move into the item list. The buttons in this list can can be pressed with `2`. Pressing `2` on the + button at the end of the list allows you to add a new item and pressing `2` on an item button allows you to change the type of an item or delete it (pixls and characters can also be made selectable or not selectable here). In the screen for selecting an item id, the item ids can be scrolled through using the D-Pad and then `1` or `2` can be pressed to go back to the main inventory editor. 

## Stat Editor
The `Edit Stats` option in the menu allows you to change the values of your Level, XP, Attack, Max HP, HP, Coins and Arcade Tokens. The menu can be navigated with the D-Pad and `+`/`-` can be used to increase/decrease the increment that the values will be changed by when pressing left/right.

## Miscellaneous Options
The `Miscellaneous Options` option in the menu allows you to configure settings that aren't especially related to speedrunning or reverse engineering.

## wiidungeon.xml Replacement
The `Load Pit Text From File` option allows you to customise the Pit of 100 Trials definition text with a file either added to your ISO or patched in with Riivolution to create your own Pit, or play someone else's. A clean copy of the file can be obtained [here](https://github.com/SeekyCt/spm-docs/blob/master/misc/wiidungeon.xml)
* Warning: this option is not supported when loading the mod exclusively through a save exploit on console (either USB Loader with a patched ISO or Riivolution must be used) because the game frequently reloads this file, so loading it from a save file would wear out the Wii's NAND.
* To use this feature with a patched ISO, follow one of the ISO guides in [INSTALLING.md](https://github.com/SeekyCt/spm-practice-codes/blob/main/INSTALLING.md) and add the custom `wiidungeon.xml` to the `mod` folder that `mod.rel` is placed in.
* To use this feature with Riivolution, edit / replace `wiidungeon.xml` in the SpmPracticeCodes folder on the root of your SD card.

## Saved Settings
The `Manage Saved Settings` option in the menu allows you to save your settings for the mod to your Wii's NAND (to the file `pcset.bin` in the game's save folder) and they'll automatically load next time you boot the mod. You can also load your saved settings, load the default settings or delete them from your NAND. 

## Game Saving and Reloading
The `Game Save Options` option in the menu allows you to save the game anywhere (it's recommended to make a backup of your save file before using this feature, if you do have any issues with it please report them either as an issue in the Github repo or by contacting me on Discord) and to reload the current save file.

## Return to Title Screen
The `Return to Title Screen` option allows you to return to the main menu without needing to game over or reboot.

## Exception Handler
When the game crashes or sets of an OSPanic, a custom exception handler will be displayed on screen showing debugging information.Unlike regular crashes, the power button can still be used to turn off the game as normal when the exception handler runs. This is currently not supported in the Korean version.

If you do have any crashes (not including ones with invalid wiidungeon.xml replacements), please report it by either [creating an issue](https://github.com/SeekyCt/spm-practice-codes/issues) or reaching out to me on Discord in the #tools-dev channel of the [SPM Speedrunning Server](https://discord.gg/dbd733H).
    * For exception messages that scroll, a video of the full text would be ideal, but if that's not possible then an image of the part that says `Address: Back Chain: LR Save:` to the end of the text (including the red text at the top) would be ok.
    * For exception messages that display without scrolling, please include an image of the entire screen.
