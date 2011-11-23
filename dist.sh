#!/bin/bash

VERSION=$1

[ -n ${VERSION} ] || exit

make clean
make
mkdir .tmp
rm -rf .tmp/qjackrcd-${VERSION}
mkdir -p .tmp/qjackrcd-${VERSION}
cp qjackrcd .tmp/qjackrcd-${VERSION}
cd .tmp
tar -czvf qjackrcd-${VERSION}.tar.gz qjackrcd-${VERSION}
mv qjackrcd-${VERSION}.tar.gz ..
cd ..
rm -rf .tmp/qjackrcd-${VERSION}


