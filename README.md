# XPRowSelect
## Windows XP Explorer Full Row Select & CTRL+N

This program does the following:
* Hooks into Windows Explorer to allow for full-row selection of Explorer items while in Details/Report view-mode. Click anywhere on the item row to select.
* CTRL+N when an explorer window is focused will open a new explorer window at that same location.  CTRL+N on invalid paths (Desktop, My Commputer, etc) will go to My Computer.

Other Stuff:
* Developed under Windows XP 32-bit Service Pack 3.  Untested on anything else.
* Win32 API only (no .NET).
* Visual Studio 6 / VC98 project included.  All code in a single CPP file.
* Runs in the background with an optional visible tray icon. Right-click tray icon to exit. Run program with "/H" argument to hide the tray icon.
* Upon program launch, existing Explorer windows using Details/Report view will be adjusted for row selection.
* Mouse-hover on tray icon to view version/info.
* See main .cpp file for release notes.
  
___NOTE1: Explorer windows _MUST_ be set to use "Details" viewing mode in order for full rows to be selected___ \
___NOTE2: Explorer Folder View option "Full Path in Title Bar" must be enabled for CTRL+N feature to work___
