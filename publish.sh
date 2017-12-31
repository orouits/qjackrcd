#!/bin/bash

RELEASE="${1:-SNAPSHOT}"

ARCNAME="qjackrcd-bin-${RELEASE}"

echo "### Clean all"
rm *.tar.gz &>/dev/null
rm -rf ".tmp" &>/dev/null
make distclean  &>/dev/null

echo "### Make release ${RELEASE} from scratch"
qmake -config release
make
#make doxy

echo "### Make tar file ${ARCNAME}.tar.gz"
mkdir -p ".tmp/${ARCNAME}"

cp -r * -r .tmp/${ARCNAME}
cd .tmp
tar -czf "../${ARCNAME}.tar.gz" "${ARCNAME}"
cd ..

rm -rf ".tmp"

if [[ "${RELEASE}" == "SNAPSHOT" ]]
then
    echo "### Commit in GIT"
    git commit -m "prepare ${RELEASE}" -a
    git push
    echo "### Remove ${RELEASE} tag in GIT"
    git tag -d ${RELEASE}
    git push origin :${RELEASE}
    echo "### Make ${RELEASE} tag in GIT"
    git tag -a "${RELEASE}" -m "publish ${RELEASE}"
    git push --tags
elif [[ "${RELEASE}" =~ ^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+$ ]]
then
    echo "### Commit in GIT"
    git commit -m "prepare v${RELEASE}" -a
    git push
    echo "### Make v${RELEASE} tag in GIT"
    git tag -a "v${RELEASE}" -m "publish v${RELEASE}"
    git push --tags
#elif [[ "${RELEASE}" =~ ^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+[[:alpha:]]+$ ]]
#then
    #not managed yet with git
fi

echo "### Done."
