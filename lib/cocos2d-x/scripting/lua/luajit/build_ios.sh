#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"


SRCDIR=$DIR/LuaJIT-2.1.0
echo "SRCDIR = $SRCDIR"
DESTDIR=$DIR/ios
echo "DESTDIR = $DESTDIR"
IXCODE="/Applications/Xcode.app/Contents/Developer"
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
echo "ISDK = $ISDK"
INFOPLIST_PATH=$IXCODE/Platforms/iPhoneOS.platform/version.plist
BUNDLE_ID=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" "${INFOPLIST_PATH}")
ISDKVER=iPhoneOS${BUNDLE_ID}.sdk
echo "ISDKVER = $ISDKVER"


if [ "${ISDKVER}" = "iPhoneOS8.0.sdk" ] || [ "${ISDKVER}" = "iPhoneOS9.1.sdk" ]; then
ISDKP=$IXCODE/usr/bin/
else
ISDKP=$ISDK/usr/bin/
fi
echo "ISDKP = $ISDKP"

if [ ! -e $ISDKP/ar ]; then
sudo cp /usr/bin/ar $ISDKP
fi


if [ ! -e $ISDKP/ranlib ]; then
sudo cp /usr/bin/ranlib $ISDKP
fi


if [ ! -e $ISDKP/strip ]; then
sudo cp /usr/bin/strip $ISDKP
fi


rm "$DESTDIR"/*.a
cd $SRCDIR




make clean
echo "======> make == armv7"
ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER"
if [ ${ISDKVER} = "iPhoneOS8.0.sdk" ] || [ ${ISDKVER} = "iPhoneOS9.1.sdk" ]; then
make HOST_CC="gcc -m32 -arch i386" TARGET_FLAGS="$ISDKF" TARGET=arm TARGET_SYS=iOS
else
make HOST_CC="gcc -m32 -arch i386" CROSS=$ISDKP TARGET_FLAGS="$ISDKF" TARGET_SYS=iOS
fi
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-armv7.a


make clean
echo "======> make == armv7s"
ISDKF="-arch armv7s -isysroot $ISDK/SDKs/$ISDKVER"
if [ ${ISDKVER} = "iPhoneOS8.0.sdk" ] || [ ${ISDKVER} = "iPhoneOS9.1.sdk" ]; then
make HOST_CC="gcc -m32 -arch i386" TARGET_FLAGS="$ISDKF" TARGET=arm TARGET_SYS=iOS
else
make HOST_CC="gcc -m32 -arch i386" CROSS=$ISDKP TARGET_FLAGS="$ISDKF" TARGET_SYS=iOS
fi
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-armv7s.a


make clean
echo "======> make == i386"
make CC="gcc -m32 -arch i386" clean all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-i386.a


make clean
echo "======> make == arm64"
ISDKF="-arch arm64 -isysroot $ISDK/SDKs/$ISDKVER"
make HOST_CC="gcc " CROSS="$ISDKP" TARGET_FLAGS="$ISDKF" TARGET=arm64 TARGET_SYS=iOS
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-arm64.a


make clean
echo "======> make == x86_64"
make CC="gcc -m64 -arch x86_64" clean all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-x86_64.a


$LIPO -create "$DESTDIR"/libluajit-*.a -output "$DESTDIR"/libluajit.a
$STRIP -S "$DESTDIR"/libluajit.a
$LIPO -info "$DESTDIR"/libluajit.a


rm "$DESTDIR"/libluajit-*.a


make clean
