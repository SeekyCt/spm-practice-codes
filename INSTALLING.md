# Extra Help
If you have any issues following this guide, or with the mod in general, feel free to [create an issue](https://github.com/SeekyCt/spm-practice-codes/issues) or reach out to me on Discord in the #tools-dev channel of the [SPM Speedrunning Server](https://discord.gg/dbd733H).

# Installing
* First you'll need to find out your disc region and revision
    * The regions can be determined by where your disc is from:
        * America: us (NTSC-U)
        * Japan: jp (NTSC-J)
        * Europe/Australia: eu (PAL)
        * Korea: kr (NSTC-K)
    * The revision of a physical disc can be determined by reading the text in the innert part of your disc (on the reflective side)
        * There should be text saying `RVL-R8P*-0A-*`
        * The first * is represents your disc region (`E` for us, `J` for jp, `P` for eu, `K` for kr)
        * The second * is your disc revision
    * The revision of an ISO can be determined by right clicking it in the Dolphin game list, then going to `Properties > Info`
        * In older versions, the revision will be listed in a `Revision` box
        * In newer versions, the revision will be listed in the `Name` box
    * From now on, the abbreviation `rgX` will be used, where `rg` is the 2 letter region code and `X` is the revision number (for example, `us1` for American revision 1)
* Now see the instructions for how you'd like to install:
    * [Console with Save Exploit](#console-with-save-exploit)
        * For use on Wii (disc) or Wii U (disc or Virtual Console)
        * No Homebrew or CIOS required
        * Requires replacing at least 1 save file (can be restored later)
        * Has the best accuracy to the original game
    * [Console with Riivolution](#console-with-riivolution)
        * For use on Wii or Wii U (disc)
        * Requires Homebrew, no CIOS required
        * No save file replacements needed
    * [Console with USB Loader and Save Exploit](#console-with-usb-loader-and-save-exploit)
        * For use on Wii or Wii U (ISO)
        * Requires Homebrew and CIOS
        * Requires replacing at least 1 save file (can be restored later)
        * No ISO modifications needed
    * [Console with USB Loader and Patched ISO](#console-with-usb-loader-and-patched-iso)
        * For use on Wii or Wii U (ISO)
        * Requires Homebrew and CIOS
        * No save file replacements needed
        * Requires Wiimms ISO Tools
    * [Dolphin with Save Exploit](#dolphin-with-save-exploit)
        * For use on emulator (ISO)
        * Requires replacing at least 1 save file (can be restored later)
        * No ISO modifications needed
    * [Dolphin with Patched ISO](#dolphin-with-patched-iso)
        * For use on emulator (ISO)
        * No save file replacements needed
        * Requires Wiimms ISO Tools

## Console with Save Exploit
See the instructions for whether you'd like to:
* [Replace existing save data](#replacing-existing-saves)
    * Slightly easier
    * Your existing save files can't be used with the mod
    * A copy of your existing save data is kept and can be restored at any time (this will uninstall the mod)
* [Patch existing saves with the mod](#patching-an-existing-save-file)
    * One of your save files will be replaced with the rel loader, the other 3 can be used with the mod
    * A copy of your existing save data is kept and can be restored at any time (this will uninstall the mod)

### Replacing Existing Saves
* If you want to back up your existing saves to be restored later (recommended), follow these steps first:
    * Insert your SD card into your Wii and load the Wii menu
    * Open `Wii > Data Management > Save Data > Wii`
        * On Wii U, this is just located under `Wii > Save data`
    * Select your SPM save data and choose `Move` to move it to your SD card
    * Eject your SD card and insert it into your computer
    * Navigate to `private/wii/title/R8P*` on your SD card
        * Replace the * with the character for your region (`E` for us, `J` for jp, `P` for eu, `K` for kr)
    * Move data.bin from here to somewhere safe on your computer
        * This contains your original save data, move it back to this location & import it in the Wii menu to restore it when you want to (this will uninstall the mod)
* Download the latest `data.***.bin` for your version from [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Create a folder on the root of your SD card named `private` if it doesn't already exist
* Create a folder inside `private` named `wii` if it doesn't already exist
* Create a folder inside `wii` named `title` if it doesn't already exist
* Create a folder inside `title` named `R8P*` if it doesn't already exist
    * Replace the * with the character for your region (`E` for us, `J` for jp, `P` for eu, `K` for kr)
* Move the `data.***.bin` into your `R8P*` folder and rename it `data.bin`
    * If a `data.bin` already exists here, delete it
* Insert your SD card into your Wii and load the Wii menu
* Open `Wii > Data Management > Save Data > Wii` and delete any existing SPM save data in the `Wii` tab
    * On Wii U, this is just located under `Wii > Save data`
* Select the `SD Card` tab at the top of the screen
* Select the SPM save here and choose `Move`
* You can now use the Rel Loader save file in your game to load the mod by opening the pause menu and selecting the `Items` tab
    * See [this video](https://youtu.be/aqeQ21WHMVE?t=42) for a demonstration
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen
* The save file `A` is also included in this save so you can easily start in game without having to replay the beginning.

### Patching an Existing Save File
* Insert your SD card into your Wii and load the Wii menu
* Open `Wii > Data Management > Save Data > Wii`
    * On Wii U, this is just located under `Wii > Save data`
* Select your SPM save data and choose `Move` to move it to your SD card
* Eject your SD card and insert it into your computer
* Navigate to `private/wii/title/R8P*` on your SD card
    * Replace the * with the character for your region (`E` for us, `J` for jp, `P` for eu, `K` for kr)
* Move data.bin from here to somewhere on your computer, and create another copy of it somewhere safe
    * This contains your original save data (including the save file you'll replace with the rel loader), move it back to this location & import it in the Wii menu to restore it when you want to (this will uninstall the mod)
* Download the latest `spm-practice-codes.***.rel` for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Follow [this guide](https://github.com/SeekyCt/spm-save-exploit/blob/main/INJECTING.md) to patch your data.bin with that rel
* Navigate back to `private/wii/title/R8P*` on your SD card
* Move the output.bin you created here and rename it data.bin
* Insert your SD card into your console and load the Wii menu
* Open `Wii > Data Management > Save Data > Wii`
    * On Wii U, this is just located under `Wii > Save data`
* Select the `SD Card` tab at the top of the screen
* Select the SPM save here and choose `Move`
* You can now use the Rel Loader save file in your game to load the mod by opening the pause menu and selecting the `Items` tab
    * See [this video](https://youtu.be/aqeQ21WHMVE?t=42) for a demonstration
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen

## Console with Riivolution
* Install the Homebrew Channel and Riivolution if you don't already have them
    * [Wii Homebrew Guide](https://wii.guide/get-started)
    * [vWii (Wii U) Homebrew Guide](https://wiiu.hacks.guide/#/vwii-modding)
    * [Riivolution Download](http://rvlution.net/wiki/Riivolution/)
* Download the latest `riivolutionfiles.zip` [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Extract the zip to the root of your SD card
* Insert your SD card into your Wii, load the Homebrew Channel, then load Riivolution from there
* In Riivolution, enable the `Practice Codes Mod` option on the `SPM Practice Codes` page and make sure no other gecko code patches are enabled, then choose `Launch`
    * If you want to use additional gecko codes (not recommended), replace the GCT file for your version in the `SpmPracticeCodes` folder on the root of your SD card and make sure to include the rel loader code for your version from `loader ***.txt` [here](https://github.com/SeekyCt/spm-rel-loader/tree/master/spm-rel-loader/loader)
    * Other file patches are not recommended but generally should work
    * If you come across any issues, please ensure they happen without any additional gecko codes or file patches before reporting them
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen

## Console with USB Loader and Save Exploit
* Install the Homebrew Channel and a USB Loader if you don't already have them
    * [Wii Homebrew Guide](https://wii.guide/get-started)
    * [vWii (Wii U) Homebrew Guide](https://wiiu.hacks.guide/#/vwii-modding)
    * [Configurable USB Loader Mod](https://wiidatabase.de/downloads/usb-und-disk-loader/configurable-usb-loader-mod/)
* If you haven't already, create an ISO of your disc with [CleanRip](https://wii.guide/cleanrip.html)
    * Rips done with other tools or in other formats like WBFS should be fine
    * NKIT rips will need to be converted back to another format
    * **Support will not be provided for any issues with illegally downloaded ISOs**
* Transfer your ISO to your USB storage if you haven't already
    * [Wii Backup Manager](https://wii.guide/wiibackupmanager) will work for most loaders
* If you're using a loader other than Configurable USB Loader Mod:
    * USB Loader GX:
        * Insert your SD card and USB into your Wii, load the Homebrew Channel, then load USB Loader GX from there
        * Go into `Settings > Loader Settings` and make sure `Ocarina` is turned off
        * This fixes it crashing when a game tries to restart, which is required by the exploit
    * Other USB loaders are currently untested
* Follow [Console with Save Exploit](#console-with-save-exploit)

## Console with USB Loader and Patched ISO
* Install the Homebrew Channel and a USB Loader if you don't already have them
    * [Wii Homebrew Guide](https://wii.guide/get-started)
    * [vWii (Wii U) Homebrew Guide](https://wiiu.hacks.guide/#/vwii-modding)
    * [Configurable USB Loader Mod](https://wiidatabase.de/downloads/usb-und-disk-loader/configurable-usb-loader-mod/)
* If you haven't already, create an ISO of your disc with [CleanRip](https://wii.guide/cleanrip.html)
    * Rips done with other tools or in other formats like WBFS should be fine
    * NKIT rips will need to be converted back to another format
    * **Support will not be provided for any issues with illegally downloaded ISOs**
* Download the latest `spm-practice-codes.***.rel` for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Follow [this guide](https://github.com/SeekyCt/spm-rel-loader/blob/master/ISO.md) to patch your ISO with that rel
* Transfer the new ISO to your USB storage
    * [Wii Backup Manager](https://wii.guide/wiibackupmanager) will work for most loaders
* Download the latest `loader.***.gct` for your version [here](https://github.com/SeekyCt/spm-rel-loader/releases)
* Reename the GCT to `R8P*01.gct`
    * Replace the * with the character for your region (`E` for us, `J` for jp, `P` for eu, `K` for kr)
* Create a folder named `codes` on the root of your SD card if it doesn't already exist
* Move your GCT into the `codes` folder
    * If you want to use additional gecko codes (not recommended), replace the GCT file with your own and make sure to include the rel loader code for your version from `loader ***.txt` [here](https://github.com/SeekyCt/spm-rel-loader/tree/master/spm-rel-loader/loader)
    * If you come across any issues, please ensure they happen without any additional gecko codes before reporting them
* Insert your SD card and USB into your Wii, load the Homebrew Channel, then load your USB Loader from there
* Make sure gecko codes are enabled in your USB Loader:
    * For Configurable USB Loader Mod:
        * Select your SPM ISO
        * Select `Options`
        * On page 2, turn on `Ocarina (cheats)`
        * Set `Hook Type` to `GXDraw`
    * For USB Loader GX:
        * Go into `Settings > Loader Settings`
        * Turn on `Ocarina`
    * This can be turned off again to boot the game without the mod (running the original ISO alone is not enough, this code must be disabled too, otherwise it will crash on boot when it can't find the mod rel; running the patched ISO without the code will just load the vanilla game)
* Load your new ISO
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen

## Dolphin with Save Exploit
* If you haven't already, install [Dolphin](https://dolphin-emu.org/) (preferrably a recent development/beta version rather than 5.0-stable)
    * Steps ahead expect you to have the SPM ISO displayed in Dolphin's game list. If you haven't already set this up:
        * Go to `Options > Configuration > Paths` and add the directory your ISO is in to `Game Folders`
        * You may need to restart Dolphin for the game to show up once this is done
* If you haven't already, create an ISO of your disc with [CleanRip](https://wii.guide/cleanrip.html)
    * Rips done with other tools or in other formats like WBFS should be fine
    * NKIT rips will need to be converted back to another format
    * **Support will not be provided for any issues with illegally downloaded ISOs**
* Some versions of the game will crash in dolphin when shutdown or rebooted. Since this exploit relies on rebooting the game, a patch is needed to fix this. (jp0, jp1 and us0 do not need this patch)
    * Right click your ISO in the Dolphin game list list and go to `Properties > Patches > Add`
    * Set `Name` to anything you want (for example, `Restart Fix`)
    * Set `Type` to `32-bit` (named `dword` in older versions)
    * Set `Offset` to the value for your version:
        * eu0 or eu1: 80238598
        * us1: 80236688
        * us2: 802369A0
        * kr0: 80230E14
    * Set `Value` to 4e800020
    * Click `Save`
* The instructions for what to do next will depend on how your save is stored in Dolphin. Right click your ISO in the Dolphin game list and click `Open Wii Save Folder` to check (you need to have ran SPM in dolphin before for long enough to create a save). See the guide for if:
    * [The folder contains files like `wiimario00`](#extracted-save)
    * [The folder contains `data.bin`](#databin-save)

### Extracted Save
* Download the latest `loader.***.wiimario` for your version from [here](https://github.com/SeekyCt/spm-save-exploit/releases)
* Rename it to `wiimario0*`
    * Replace * with a number from 0-3 for the save file you want to replace with the rel loader
    * `wiimario00`-`wiimario03` are save file 1-4 respectively
* Move the original file with this name in your SPM save folder somewhere else if you'd like to restore it later, otherwise just delete it
* Move in the new save file into your SPM save folder
* Download the latest `pcrel.***.bin` for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Rename that bin file to `pcrel.bin` and place it in your SPM save folder
* You can now use the Rel Loader save file in your game to load the mod by opening the pause menu and selecting the `Items` tab
    * See [this video](https://youtu.be/aqeQ21WHMVE?t=42) for a demonstration
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen

### Data.bin Save
See the instructions for whether you'd like to:
* [Replace existing save data](#replacing-existing-saves-1)
    * Slightly easier
    * Your existing save files can't be used with the mod
    * A copy of your existing save data is kept and can be restored at any time (this will uninstall the mod)
* [Patch existing saves with the mod](#patching-an-existing-save-file-1)
    * One of your save files will be replaced with the rel loader, the other 3 can be used with the mod
    * A copy of your existing save data is kept and can be restored at any time (this will uninstall the mod)

#### Replacing Existing Saves
* Move data.bin somewhere safe
    * This contains your original save data, move it back to this location to restore it when you want to (this will uninstall the mod)
* Download the latest `data.***.bin` for your version from [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Rename it to `data.bin` and place it in your SPM save folder
* You can now use the Rel Loader save file in your game to load the mod by opening the pause menu and selecting the `Items` tab
    * See [this video](https://youtu.be/aqeQ21WHMVE?t=42) for a demonstration
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen
* The save file `A` is also included in this save so you can easily start in game without having to replay the beginning.

#### Patching an Existing Save File
* Make a copy of the original `data.bin` somewhere safe
    * This contains your original save data (including the save file you'll replace with the rel loader), move it back to this location to restore it when you want to (this will uninstall the mod)
* Download the latest `spm-practice-codes.***.rel` for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Follow [this guide](https://github.com/SeekyCt/spm-save-exploit/blob/main/INJECTING.md) to patch your data.bin with that rel
* Move `output.bin` into your SPM save folder and rename it `data.bin`
* You can now use the Rel Loader save file in your game to load the mod by opening the pause menu and selecting the `Items` tab
    * See [this video](https://youtu.be/aqeQ21WHMVE?t=42) for a demonstration
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen

## Dolphin with Patched ISO
* If you haven't already, install [Dolphin](https://dolphin-emu.org/) (preferrably a recent development/beta version rather than 5.0 Stable)
    * Steps ahead expect you to have the SPM ISO displayed in Dolphin's game list. If you haven't already set this up:
        * Go to `Options > Configuration > Paths` and add the directory your ISO is in to `Game Folders`
        * You may need to restart Dolphin for the game to show up once this is done
* If you haven't already, create an ISO of your disc with [CleanRip](https://wii.guide/cleanrip.html)
    * Rips done with other tools or in other formats like WBFS should be fine
    * NKIT rips will need to be converted back to another format
    * **Support will not be provided for any issues with illegally downloaded ISOs**
* Make sure `Options > Configuration > Enable Cheats` is enabled
* Right click your ISO in the Dolphin game list and go to `Properties > Gecko Codes > Add New Code`
    * This option doesn't exist on older versions of Dolphin such as 5.0-stable, and you'll instead have to use `Edit Config`. Either update to a newer version or search online for a guide on how to add codes there.
* Set `Name` to whatever you want (`Rel Loader`, for example)
* No input is needed in `Creator` or `Notes`
* Copy the gecko code for your version from `loader ***.txt` [here](https://github.com/SeekyCt/spm-rel-loader/tree/master/spm-rel-loader/loader) and paste it into `Code`
* Click `Save`
* Make sure to tick the box next to the code to enable it if it isn't already
    * You can untick this later to disable the mod (running the original ISO alone is not enough, this code must be disabled too, otherwise it will crash on boot when it can't find the mod rel; running the patched ISO without the code will just load the vanilla game)
    * It's recommended that you disable any additional gecko codes; if you come across any issues, please make sure they happen without any additional gecko codes before reporting them
* Download the latest `spm-practice-codes.***.rel` for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases)
* Follow [this guide](https://github.com/SeekyCt/spm-rel-loader/blob/master/ISO.md) to patch your ISO with that rel
* Open your new ISO in Dolphin
* If everything has worked, you should see text saying `SPM Practice Codes v*` at the top of the title screen
