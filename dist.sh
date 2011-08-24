#!/bin/bash

VERSION=$1

[ -d .tmp ] || exit
[ -n ${VERSION} ] || exit

make
rm -rf .tmp/qjackrcd-${VERSION}
mkdir -p .tmp/qjackrcd-${VERSION}
cp qjackrcd .tmp/qjackrcd-${VERSION}
cd .tmp
tar -czvf qjackrcd-${VERSION}.tar.gz qjackrcd-${VERSION}

