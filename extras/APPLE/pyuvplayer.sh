# !/bin/sh

# script for generating the MAC app

rm -R pyuv.app dmg_tmp pyuv_*_install.dmg temp.dmg
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
dylibbundler -od -b -x ./pyuv.app/Contents/MacOS/pyuv -d ./pyuv.app/Contents/Frameworks

otool -L ./pyuv.app/Contents/MacOS/pyuv

rm -rf dmg_tmp
mkdir dmg_tmp
cp -R pyuv.app dmg_tmp
mkdir dmg_tmp/.background
cp ./backmac.bmp dmg_tmp/.background/mac_pyuv_bckg.bmp

hdiutil create -srcfolder dmg_tmp -volname "Pyuv for Mac" -format UDRW -ov temp.dmg

hdiutil attach -readwrite -noverify -noautoopen "temp.dmg"

cat <<EOM | osascript
tell application "Finder"
        tell disk "Pyuv for Mac"
                open
                set current view of container window to icon view
                set toolbar visible of container window to false
                set statusbar visible of container window to false
                set the bounds of container window to {400, 100, 885, 310}
                set theViewOptions to the icon view options of container window
                set background picture of theViewOptions to file ".background:mac_pyuv_bckg.bmp"
                set arrangement of theViewOptions to not arranged
                set icon size of theViewOptions to 104
                make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
                set position of item "pyuv" of container window to {90, 90}
                set position of item "Applications" of container window to {400, 90}
                update without registering applications
                delay 5
                set position of item "pyuv" of container window to {90, 90}
                close
        end tell
end tell
EOM


hdiutil detach "/Volumes/Pyuv for Mac"
hdiutil convert "temp.dmg" -format UDZO -imagekey zlib-level=9 -o "pyuv_070_install.dmg"


rm -R dmg_tmp temp.dmg