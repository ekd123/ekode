print("lua launched")
local lgi=require 'lgi'
local T = lgi.require('T')
local obj = T.Obj()
local str = obj.plus ("you input", 2)
print(str)
print("Lua done")
