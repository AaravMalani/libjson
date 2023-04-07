set -e
rm -rf build /tmp/jsonwork/* *.deb *.rpm
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/jsonwork/usr/local/ -DDO_NOT_LDCONFIG=true
cd build
make install 
cd ..
mkdir -p /tmp/jsonwork/DEBIAN/
cp DEBIAN_CONTROL /tmp/jsonwork/DEBIAN/control
dpkg-deb --build --root-owner-group /tmp/jsonwork
cp /tmp/jsonwork.deb .
VERSION=$(cat DEBIAN_CONTROL | grep Version: | sed -e 's/Version: //')
NAME=$(cat DEBIAN_CONTROL | grep Package: | sed -e 's/Package: //')
ARCH=$(cat DEBIAN_CONTROL | grep Architecture: | sed -e 's/Architecture: //')
mv jsonwork.deb $NAME-$VERSION-$ARCH.deb
fakeroot alien -r $NAME-$VERSION-$ARCH.deb
mv *.rpm $NAME-$VERSION-$ARCH.rpm