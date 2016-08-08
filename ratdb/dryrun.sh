# this script can make a environment to test RatDB.
# it's for whom doesn't wanna install it.
# 
# Modified ("2-clause") BSD License
# 
# Written by Mike Manilone <zhtx10@gmail.com>

echo \* Configure
./autoclean.sh > /dev/null
printf which\ cc\ do\ you\ prefer\?
read RATDB_DR_CC
./autogen.sh CC=$RATDB_DR_CC > /dev/null 2>/dev/null

echo \* Start building
make -j4 > /dev/null

echo \* Build environment
printf where\ do\ you\ want\?
read RATDB_DR_PATH
mkdir -p $RATDB_DR_PATH

echo \* Copy files
make install DESTDIR="$RATDB_DR_PATH"

echo \* Have Done! Try it out!
echo "Please manually add the schema into GSettings seach path."
