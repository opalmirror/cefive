# Installing CEFive #
CEFive is a Custom Firmware Plugin.  For this reason it must be installed in the **seplugins** folder on the root of your PSP memory stick.  Copy the file **CEFive.prx** to the **seplugins** folder.

## game.txt ##
In the **seplugins** folder is a file called **game.txt**.  This file tells your CFW the plugins that are installed.  It is a plain text file with lines containing the full path to plugins followed by a single digit indicating whether the plugin is enabled.  Add a line to this file.

`ms0:/seplugins/CEFive.prx 0`

## Enable CEFive ##
In the recovery menu of your CFW (usually activated by holding the Right Trigger while powering on) is a **Plugins** menu.  This menu allows you to select which plugins are enabled at power on.  Select the CEFive.prx plugin and make sure that it is Enabled.  Make sure that _NitePr.prx_, _CoderPr.prx_, _CWCheat.prx_, and/or _mkultra.prx_ are disabled.

## Cheat files ##
If you already had NitePr or mkultra installed, you don't need to do anything, your cheats are already in the right place.

Currently, CEFive is meant to be a NitePr replacement, so the cheats are currently placed in the same directory as they are for NitePr.
Cheats are held in cheat files.  The cheat files themselves are plain text files with the SCE issued ID of the game as a filename.  Create a folder inside the **seplugins** folder named **nitePr**.  Place all of your cheat files in this folder.

**CEFive does not work with CWCheat files.**