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
make doxy

echo "### Make tar file ${ARCNAME}.tar.gz"
mkdir -p ".tmp/${ARCNAME}"

cp -r * .tmp/${ARCNAME}
cd .tmp
tar -czf "../${ARCNAME}.tar.gz" "${ARCNAME}"
cd ..

rm -rf ".tmp"

if [[ "${RELEASE}" == "SNAPSHOT" ]]
then
    echo "### Commit in GIT"
    git commit -m "${ARCNAME}" -a
    git push
elif [[ "${RELEASE}" =~ ^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+$ ]]
then
    echo "### Make ${ARCNAME} tag in GIT"
    git commit -m "${ARCNAME}" -a
    git tag -a "${ARCNAME}" -m "${ARCNAME}"
    git push --tags
#elif [[ "${RELEASE}" =~ ^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+[[:alpha:]]+$ ]]
#then
    #not managed yet with git
fi

echo "### Done."
