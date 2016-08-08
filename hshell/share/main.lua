-- 
-- main.lua
-- the default configuration of HShell
-- 
-- Note: 1. Don't remove comments, if you can 
--          memorize them clearly.
--       2. Don't remove this file! This action
--          will cause HShell not to work!
--       3. Don't change this file. You should
--          overwrite configuration in your home 
--          directory. The path is usually is 
--          ~/.local/share/hshell/extensions/main/.
--       4. Don't expand this file. One line's 
--          change will cause GUI configuration 
--          tool not to work.
--       5. If you are not an advanced user, we 
--          strongly recommend you to use the 
--          GUI configuration tool.

--[[--
Conf: Conf_Version
==================================================
How many times the configuration's format is changed.

This is very important to the settings tool! It 
uses this to change the line of the configuration.
==================================================
(for 0.1 release) Default: 0
--]]--
Conf_Version=0

--[[--
Conf: Blocked_Extensions
==================================================
Specify extensions that won't run automatically.
They are splitted with ',' and without spaces.
==================================================
Default:  "Sample,Sample2" 
--]]--
Blocked_Extensions="Sample,Sample2"

--[[--
Conf: Screen_Reserve
==================================================
How much space will HShell use. If it's n, for example, 
HShell will use 1/n of space of your monitor.

You don't usually need to change this. If you're 
sure what you're doing.
==================================================
Default: 23
--]]--
Screen_Reserve=23

--[[--
Conf: Dash_Favorites
==================================================
The list of your favorite launchers.
Please write file names here 

For example, "firefox.desktop,gimp.desktop"
==================================================
Default: ""
--]]--
Dash_Favorites=""

--[[--
Conf: Date_Format
==================================================
Format of date on the panel
==================================================
Default: "%B %d %G %A %H:%M:%S"
--]]--
Date_Format="%B %d %G %A %H:%M:%S"
