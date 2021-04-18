# Installing
See the instructions for [Console](#Console) or [Dolphin](#Dolphin) depending on where you want to play.

## Console
* Install the Homebrew Channel and Riivolution if you don't already have them.
* Download the latest Riivolution zip for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases).
* Extract the zip to the root of your SD card.
* In Riivolution, enable the Practice Codes Mod option on the SPM Practice Codes page and make sure no other gecko code patches are enabled
    * If you want to use additional gecko codes, replace the GCT file for your version in the SpmPracticeCodes folder on the root of your SD card and make sure to include the rel loader code for your region from [here](https://github.com/SeekyCt/spm-rel-loader/tree/master/spm-rel-loader/loader).

## Dolphin
* Make sure `Options > Configuration > Enable Cheats` is enabled.
* Get the rel loader gecko code for your region [here](https://github.com/SeekyCt/spm-rel-loader/tree/master/spm-rel-loader/loader) and add it to your game by right clicking it in the list then going to `Properties > Gecko Codes > Add New Code`.
    * **Note:** this is a different version of the code to the one distributed with the SPM Desert Bus mod.
* Download the latest rel for your version [here](https://github.com/SeekyCt/spm-practice-codes/releases) and rename it to `mod.rel`.
* Extract your ISO with Dolphin or Wiimms ISO Tools.
* Go to `DATA/files` in your extracted ISO (should contain folders like 'a', 'eff' and 'font').
* Create a folder named `mod` here.
* Place `mod.rel` in the new `mod` folder.
* If using Wiimms ISO Tools, rebuild your ISO and run it in Dolphin.
* If using Dolphin's ISO extraction, open `DATA/sys/main.dol` in Dolphin.
