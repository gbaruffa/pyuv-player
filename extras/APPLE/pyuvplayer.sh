# !/bin/sh

# script for generating the MAC app

rm -R pyuv.app dmg_tmp pyuv_install.dmg
mkdir -p pyuv.app/Contents
mkdir -p pyuv.app/Contents/MacOS
mkdir -p pyuv.app/Contents/Resources
cp Info.plist pyuv.app/Contents/Info.plist
echo -n "APPL????" >pyuv.app/Contents/PkgInfo
ln -f ../../src/pyuv pyuv.app/Contents/MacOS/pyuv
cp -f playuv.icns pyuv.app/Contents/Resources/playuv.icns
cp -f ../../doc/doc.htb pyuv.app/Contents/Resources/doc.htb
rm -rf dmg_tmp
mkdir dmg_tmp
cp -R pyuv.app dmg_tmp
hdiutil create -srcfolder dmg_tmp -volname "Pyuv for Mac" -format UDZO -ov pyuv_062_install.dmg
rm -R dmg_tmp