#!/bin/bash

RELEASE="${1:-SNAPSHOT}"

ARCNAME="qjackrcd-${RELEASE}"

echo "### Clean all"
rm qjackrcd-*.tar.gz &>/dev/null
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

if [[ "${RELEASE}" == "SNAPSHOT" ]]
then
    echo "### Commit in SVN"
    svn ci -m "${ARCNAME}"
    echo "### publish ${ARCNAME} archive to TESTING"
    scp "${ARCNAME}.tar.gz" "orouits,qjackrcd@frs.sourceforge.net:/home/frs/project/q/qj/qjackrcd/testing/"
elif [[ "${RELEASE}" =~ ^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+$ ]]
then
    echo "### Make tag in SVN /tags/${ARCNAME}"
    svn cp . "svn+ssh://orouits@svn.code.sf.net/p/qjackrcd/code/tags/${ARCNAME}" -m "${ARCNAME}"
    echo "### publish ${ARCNAME} archive to STABLE"
    scp "${ARCNAME}.tar.gz" "orouits,qjackrcd@frs.sourceforge.net:/home/frs/project/q/qj/qjackrcd/stable/"
    scp README "orouits,qjackrcd@frs.sourceforge.net:/home/frs/project/q/qj/qjackrcd/"
elif [[ "${RELEASE}" =~ ^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+[[:alpha:]]+$ ]]
then
    echo "### Make tag in SVN /tags/${ARCNAME}"
    svn cp . "svn+ssh://orouits@svn.code.sf.net/p/qjackrcd/code/tags/${ARCNAME}" -m "${ARCNAME}"
    echo "### publish ${ARCNAME} archive to TESTING"
    scp "${ARCNAME}.tar.gz" "orouits,qjackrcd@frs.sourceforge.net:/home/frs/project/q/qj/qjackrcd/testing/"
fi

echo "### Done."
