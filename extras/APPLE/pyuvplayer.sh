# !/bin/sh

# script for generating the MAC app

rm -R pyuv.app dmg_tmp pyuv_install.dmg
mkdir -p pyuv.app/Contents
mkdir -p pyuv.app/Contents/MacOS
mkdir -p pyuv.app/Contents/Resources
mkdir -p pyuv.app/Contents/Resources/en.lproj
mkdir -p pyuv.app/Contents/Resources/en_GB.lproj
mkdir -p pyuv.app/Contents/Resources/en_US.lproj
mkdir -p pyuv.app/Contents/Resources/it.lproj
mkdir -p pyuv.app/Contents/Frameworks
cp Info.plist pyuv.app/Contents/Info.plist
echo -n "APPL????" >pyuv.app/Contents/PkgInfo
cp -f ../../src/pyuv pyuv.app/Contents/MacOS/pyuv
cp -f playuv.icns pyuv.app/Contents/Resources/playuv.icns
cp -f ../../src/po/it.mo pyuv.app/Contents/Resources/it.lproj/pyuv.mo
#touch pyuv.app/Contents/Resources/en.lproj/pyuv.mo
#touch pyuv.app/Contents/Resources/en_GB.lproj/pyuv.mo
#touch pyuv.app/Contents/Resources/en_US.lproj/pyuv.mo
cp -f ../../doc/doc.htb pyuv.app/Contents/MacOS/doc.htb
./dylibbundler -od -b -x ./pyuv.app/Contents/MacOS/pyuv -d ./pyuv.app/Contents/Frameworks

#cp /usr/local/lib/libwx_osx_cocoau_xrc-3.0.dylib pyuv.app/Contents/Frameworks
#install_name_tool -id @executable_path/../Frameworks/libwx_osx_cocoau_xrc-3.0.dylib pyuv.app/Contents/Frameworks/libwx_osx_cocoau_xrc-3.0.dylib
#install_name_tool -change /usr/local/lib/libwx_osx_cocoau_xrc-3.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_xrc-3.0.dylib pyuv.app/Contents/MacOS/pyuv

otool -L ./pyuv.app/Contents/MacOS/pyuv

rm -rf dmg_tmp
mkdir dmg_tmp
cp -R pyuv.app dmg_tmp
ln -s /Applications dmg_tmp
touch dmg_tmp/"Drag and drop pyuv over the Applications folder"
hdiutil create -srcfolder dmg_tmp -volname "Pyuv for Mac" -format UDZO -ov pyuv_070_install.dmg
rm -R dmg_tmp