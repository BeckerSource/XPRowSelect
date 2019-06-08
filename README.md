# XPRowSelect
## Windows XP Explorer Full Row Select

This program runs in the background with a visible tray icon and hooks into Windows Explorer to allow for full-row selection of Explorer items while in Details/Report view-mode.  Click anywhere on the item row to select.

* Developed under Windows XP 32-bit Service Pack 3.  Untested on anything else.
* Win32 API only (no .NET).
* Upon program launch, existing Explorer windows using Details/Report view will be adjusted for row selection.
* Right-click tray icon to view the program version and/or exit.
* Visual Studio 6 / VC98 project included.  All code in a single CPP file.  Release executable is ~40k.

* NOTE: Explorer windows _MUST_ be set to use "Details" viewing mode in order for full rows to be selected *

Project inspired by:
https://www.codeproject.com/Articles/32373/Windows-Explorer-in-XP-Now-with-Checkboxes-and-Ful
