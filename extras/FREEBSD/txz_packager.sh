#!/bin/sh
# http://lastsummer.de/creating-custom-packages-on-freebsd/

STAGEDIR=./tmp-stage
rm -rf ${STAGEDIR} pyuv*.txz
mkdir -p ${STAGEDIR}

# is this needed?
cat >> ${STAGEDIR}/+PRE_DEINSTALL <<EOF
# careful here, this may clobber your system
echo "Resetting root shell"
pw usermod -n root -s /bin/csh
EOF

# is this needed?
cat >> ${STAGEDIR}/+POST_INSTALL <<EOF
# careful here, this may clobber your system
echo "Registering root shell"
pw usermod -n root -s /bin/sh
EOF

cat >> ${STAGEDIR}/+MANIFEST <<EOF
name: pyuv
version: "0.7.0_1"
origin: multimedia/pyuv
comment: "Raw YUV file player"
desc: "A tool to play raw YUV files and similar"
maintainer: giuseppe.baruffa@unipg.it
www: https://github.com/gbaruffa/pyuv-player
prefix: /
EOF

echo "deps: {" >> ${STAGEDIR}/+MANIFEST
pkg query "  %n: { version: \"%v\", origin: %o }" wx30-gtk2 >> ${STAGEDIR}/+MANIFEST
echo "}" >> ${STAGEDIR}/+MANIFEST

mkdir -p ${STAGEDIR}/usr/local/bin
mkdir -p ${STAGEDIR}/usr/local/share/applications
mkdir -p ${STAGEDIR}/usr/local/share/icons/hicolor/128x128/apps
mkdir -p ${STAGEDIR}/usr/local/share/locale/it/LC_MESSAGES
mkdir -p ${STAGEDIR}/usr/local/share/doc/pyuv
mkdir -p ${STAGEDIR}/usr/local/man/man1
cp ../../src/pyuv ${STAGEDIR}/usr/local/bin
echo "/usr/local/bin/pyuv" >> ${STAGEDIR}/plist
cp ../freedesktop/Pyuv.desktop ${STAGEDIR}/usr/local/share/applications/pyuv.desktop
echo "/usr/local/share/applications/pyuv.desktop" >> ${STAGEDIR}/plist
cp ../../src/playuv128.png ${STAGEDIR}/usr/local/share/icons/hicolor/128x128/apps/pyuv.png
echo "/usr/local/share/icons/hicolor/128x128/apps/pyuv.png" >> ${STAGEDIR}/plist
cp ../../src/po/it.mo ${STAGEDIR}/usr/local/share/locale/it/LC_MESSAGES/pyuv.mo
echo "/usr/local/share/locale/it/LC_MESSAGES/pyuv.mo" >> ${STAGEDIR}/plist
cp ../../doc/doc.htb ${STAGEDIR}/usr/local/share/doc/pyuv/doc.htb
echo "/usr/local/share/doc/pyuv/doc.htb" >> ${STAGEDIR}/plist
gzip -n --best - <../../man/pyuv.1 >${STAGEDIR}/usr/local/man/man1/pyuv.1.gz
echo "/usr/local/man/man1/pyuv.1.gz" >> ${STAGEDIR}/plist

pkg create -m ${STAGEDIR}/ -r ${STAGEDIR}/ -p ${STAGEDIR}/plist -o .

# clean
rm -rf ${STAGEDIR} 
