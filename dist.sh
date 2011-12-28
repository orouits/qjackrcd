#!/bin/bash

VERSION=$1

[ -n ${VERSION} ] || exit

make distclean
qmake -config release
make clean
make

rm -rf .tmp
mkdir -p .tmp/qjackrcd-${VERSION}

cp qjackrcd .tmp/qjackrcd-${VERSION}
cp *.qm .tmp/qjackrcd-${VERSION}
cd .tmp
tar -czvf qjackrcd-${VERSION}.tar.gz qjackrcd-${VERSION}
mv qjackrcd-${VERSION}.tar.gz ..
cd ..

rm -rf .tmp
mkdir -p .tmp/qjackrcd-${VERSION}

cp *.ui *.h *.pro *.pro.user *.cpp *.ts *.png *.qrc *.sh *.desktop Makefile README .tmp/qjackrcd-${VERSION}
cd .tmp
tar -czvf qjackrcd-src-${VERSION}.tar.gz qjackrcd-${VERSION}
mv qjackrcd-src-${VERSION}.tar.gz ..
cd ..

rm -rf .tmp


