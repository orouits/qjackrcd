#!/bin/bash

RELEASE="${1:-SNAPSHOT}"
TAGIT="${2:-nt}"

ARCNAME="qjackrcd-${RELEASE}"

echo "### Clean all"
rm "${ARCNAME}.tar.gz" &>/dev/null
rm -rf ".tmp" &>/dev/null
make distclean  &>/dev/null

echo "### Make release ${ARCNAME} from scratch"
qmake -config release
make

echo "### Make tar file ${ARCNAME}.tar.gz"
mkdir -p ".tmp/${ARCNAME}"

cp -r * .tmp/${ARCNAME}
cd .tmp
tar -czf "../${ARCNAME}.tar.gz" "${ARCNAME}"
cd ..

rm -rf ".tmp"

if [[ "${TAGIT}" == "t" ]]
then
    echo "### Make tag in SVN /tags/${ARCNAME}"
    svn cp . "svn+ssh://orouits@svn.code.sf.net/p/qjackrcd/code/tags/${ARCNAME}"
fi

echo "### Done."
