# demo

This program demostrates how to set up a typical GNOME 3 program.

It utilizes:
* GtkApplication and its friends to manage windows
* GtkBuilder to create windows
* GResource to put resources into an executable
* GSettings to save settings
* AppMenu

And it tries to follow the GNOME coding style.

To get a deeper understanding of what happens in the low level, all code is 
written in C.

Note: You should set "GSETTINGS\_SCHEMA\_DIR=."

There's more to cover, like .desktop files, icons, and a fully functional 
program rather than this one, but I'm fed up with C. If you like to see more, 
refer to other materials.
