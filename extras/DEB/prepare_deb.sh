# !/bin/bash

# Ubuntu/Debian packager for PYUV

rm -fR debian pyuv*.deb
mkdir -p debian/usr/bin
cp ../../src/pyuv  ./debian/usr/bin
strip ./debian/usr/bin/pyuv
chmod 755 ./debian/usr/bin/pyuv
mkdir -p debian/usr/share/doc/pyuv
cp copyright ./debian/usr/share/doc/pyuv
chmod 644 ./debian/usr/share/doc/pyuv/copyright
cp ../../doc/doc.htb ./debian/usr/share/doc/pyuv
chmod 644 ./debian/usr/share/doc/pyuv/doc.htb
gzip -n --best - <../../ChangeLog >./debian/usr/share/doc/pyuv/changelog.gz
chmod 644 ./debian/usr/share/doc/pyuv/changelog.gz
gzip -n --best - <changelog.Debian >./debian/usr/share/doc/pyuv/changelog.Debian.gz
chmod 644 ./debian/usr/share/doc/pyuv/changelog.Debian.gz
mkdir -p debian/usr/share/man/man1
gzip -n --best - <../../man/pyuv.1 >./debian/usr/share/man/man1/pyuv.1.gz
chmod 644 ./debian/usr/share/man/man1/pyuv.1.gz
mkdir -p debian/DEBIAN
find ./debian -type d | xargs chmod 755
cp control debian/DEBIAN/control
mkdir -p debian/usr/share/icons/hicolor/128x128/apps
chmod 755  debian/usr/share/icons
chmod 755  debian/usr/share/icons/hicolor
chmod 755  debian/usr/share/icons/hicolor/128x128
chmod 755  debian/usr/share/icons/hicolor/128x128/apps
cp -p ../../src/playuv128.png debian/usr/share/icons/hicolor/128x128/apps/pyuv.png
chmod 644 debian/usr/share/icons/hicolor/128x128/apps/pyuv.png
mkdir -p debian/usr/share/applications
chmod 755 debian/usr/share/applications
cp ../freedesktop/Pyuv.desktop ./debian/usr/share/applications/pyuv.desktop
chmod 644 ./debian/usr/share/applications/pyuv.desktop

mkdir -p debian/usr/share/locale/it/LC_MESSAGES
chmod 755  debian/usr/share/locale
chmod 755  debian/usr/share/locale/it
chmod 755  debian/usr/share/locale/it/LC_MESSAGES
cp -p ../../src/po/it.mo debian/usr/share/locale/it/LC_MESSAGES/pyuv.mo
chmod 644 debian/usr/share/locale/it/LC_MESSAGES/pyuv.mo

fakeroot dpkg-deb --build debian
mv debian.deb pyuv_0.7.0-1_amd64.deb
du -sk debian
rm -fr debian
lintian pyuv*deb
