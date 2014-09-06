# Makefile for PYUV

# General configuration variables:
CC = $(shell wx-config --cxx)
AR = ar

#CFLAGS = $(shell wx-config-3.0 --cxxflags) -Wall # -g -p -pg
CFLAGS = $(shell wx-config --cxxflags) -std=c++11 -Wall -O2

PYUV_SRCS = source/converter.cpp source/playuvApp.cpp source/playuvFrame.cpp source/playuvFormat.cpp source/playuvSync.cpp

MODULES = $(PYUV_SRCS:.cpp=.o)

all: pyuv

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

pyuv: $(PYUV_SRCS)
	$(CC) $(CFLAGS) -I .. -o pyuv $(PYUV_SRCS) -lm -lstdc++ $(shell wx-config --libs)


clean:
	rm -f pyuv *.o *.a

deb: pyuv
	rm -f pyuv*.deb
	mkdir -p debian/usr/bin
	strip pyuv
	cp pyuv ./debian/usr/bin
	mkdir -p debian/usr/share/doc/pyuv
	cp copyright ./debian/usr/share/doc/pyuv
#	cp doc.htb ./debian/usr/share/doc/pyuv
	gzip --best - <changelog >./debian/usr/share/doc/pyuv/changelog.gz
	gzip --best - <changelog.Debian >./debian/usr/share/doc/pyuv/changelog.Debian.gz
	mkdir -p debian/usr/share/man/man1
	gzip --best - <pyuv.1 >./debian/usr/share/man/man1/pyuv.1.gz
	mkdir -p debian/DEBIAN
	find ./debian -type d | xargs chmod 755
	cp control debian/DEBIAN/control
	mkdir -p debian/usr/share/icons
#	cp -p resource/playuv64.png debian/usr/share/icons/pyuv.xpm
	cp -p source/playuv16.xpm debian/usr/share/icons/pyuv.xpm
	mkdir -p debian/usr/share/applications
	cp Pyuv.desktop ./debian/usr/share/applications/pyuv.desktop
	fakeroot dpkg-deb --build debian
	mv debian.deb pyuv_0.6.1-1_amd64.deb
	rm -fr debian

#http://endrift.com/blog/2010/06/14/dmg-files-volume-icons-cli/
app: pyuv
	mkdir -p pyuv.app/Contents
	mkdir -p pyuv.app/Contents/MacOS
	mkdir -p pyuv.app/Contents/Resources
	cp resource/Info.plist pyuv.app/Contents/Info.plist
	echo -n "APPL????" >pyuv.app/Contents/PkgInfo
	ln -f pyuv pyuv.app/Contents/MacOS/pyuv
	cp -f resource/playuv.icns pyuv.app/Contents/Resources/playuv.icns
	cp -f doc.htb pyuv.app/Contents/Resources/doc.htb
	rm -rf dmg_tmp
	mkdir dmg_tmp
	cp -R pyuv.app dmg_tmp
	hdiutil create -srcfolder dmg_tmp -volname "Pyuv for Mac" -format UDZO -ov pyuv_install.dmg
