#!/bin/sh

##
## 
## For FreeBSD Users
## 
##

printf "Where to build (absolute)? "
read WRK
mkdir -p $WRK
OLD=`pwd`
cd $WRK

# build hsession
git clone git://github.com/ekd123/hsession.git
cd hsession
mkdir build
cd build
cmake ..
make 
if [ "$1" == "--with-install" ]; then
    make install
fi
cd ../..

# build eggtraymanager
git clone git://github.com/ekd123/eggtraymanager.git
cd eggtraymanager
mkdir build
cd build
cmake .. -DLIBDATA_INSTALL_DIR=/usr/local/libdata/
make
if [ "$1" == "--with-install" ]; then
    make install
fi
cd ../../

# build hshell
git clone git://github.com/ekd123/hshell.git
cd hshell
mkdir build
cd build
cmake ..
make 
if [ "$1" == "--with-install" ]; then
    make install
fi
cd ../..

# done
if [ "$1" != "--with-install" ]; then
    return 0
fi
cd $OLD
echo "Your installation has been done."
echo "Now check out the steps to make HSession work."
echo "WWW: https://github.com/ekd123/hsession/wiki"
echo ""
echo "Enjoy your life in HShell :-)"
echo "Suggestions are welcome"

