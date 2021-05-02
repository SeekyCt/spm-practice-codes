# Manual

## Opening the Menu
The menu can currently be opened at any time the game's pause menu can (although this is probably going to be made less strict in a future version) by pressing `B`.

The menu can then be navigated with the D-Pad, `2` to select and `1` to exit.

## Map Warping
The `Change Map` option in the menu allows you to warp to any map by its internal name.

The first 3 letters of the name are the area (such as `he1` for 1-1, `ta3` for 3-3, `mac` for Flipside) and the 2 numbers are the number of the map within that area. These can scrolled through using the D-Pad. Once you've chosen the map you'd like to warp to, press `2`.

A full list of map names can be found [here](https://docs.google.com/document/d/10w4CS5oNBOHHYtM9OrNUYM7GIqNxIaR-b_Sr8FSG7Pk/edit). 

By default a Dimentio teleport animation will play when you do this (thanks to JohnP55 for the idea and documentation of relevant functions), but you can turn that off by pressing `-` in the menu if you'd like.

## HUD Displays
The `Edit HUD Display Options` option in the menu allows you to enable and disable various displays on the screen. The list can be navigated with the D-Pad and `2` can be pressed to switch between Enabled and Disabled.

### Map & Door Name
The Map & Door Name Display shows the current map name (without the underscore since the game's font doesn't support them) and the name of the door that it was entered with in the top left of the screen.

### Position
The Position Display shows your current co-ordinates in the top left of the screen.

## Logging Options
The `Edit Logging Options` option in the menu allows you to enable and disable various things being printed to an on-screen console or OSReport (for Dolphin). The list can be navigated with the D-Pad and `2` can be pressed to switch between Enabled and Disabled.

### Script Tracking
The `Evt Script Tracking` option will log the address of any script that starts.

### Script Variable Tracking
The `Evt Variable Tracking` option will log whenever a saved script variable (LSW, LSWF, GSW, GSWF) is changed.

### Script Debug Instruction
The `Evt Debug Instructions` option will log messages passed into the debug script instructions `debug_put_msg` and `debug_put_reg`.

### Script Variable Editor
The `Edit Script Variables` option in the menu allows you to view and edit the values of any GW, GF, GSW, GSWF, LSW or LSWF. The variable type (first few letters), variable number (first 4 decimal digits) and value (final 8 hex digits) can be scrolled through with the D-Pad.

## Inventory Editor
The `Edit Inventory` option in the menu allows you add and remove items, characters and pixls from your inventory. The menu can be navigated with the D-Pad, first by selecting an itmem type in the column on the left then pressing right to move into the item list. The buttons in this list can can be pressed with `2`. Pressing `2` on the + button at the end of the list allows you to add a new item and pressing `2` on an item button allows you to change the type of an item or delete it (pixls and characters can also be made selectable or not selectable here). In the screen for selecting an item id, the item ids can be scrolled through using the D-Pad and then `1` or `2` can be pressed to go back to the main inventory editor. 

## Stat Editor
The `Edit Stats` option in the menu allows you to change the values of your Level, XP, Attack, Max HP, HP, Coins and Arcade Tokens. The menu can be navigated with the D-Pad and `+`/`-` can be used to increase/decrease the increment that the values will be changed by when pressing left/right.

## Saved Settings
The `Manage Saved Settings` option in the menu allows you to save your settings for the mod to your Wii's NAND (to the file `pcset.bin` in the game's save folder) and they'll automatically load next time you boot the mod. You can also load your saved settings, load the default settings or delete them from your NAND. 

## Return to Title Screen
The `Return to Title Screen` option allows you to return to the main menu without needing to game over or reboot.