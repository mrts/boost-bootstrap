#!/bin/bash

set -e
set -u

VERSION='1.55.0'
ZIPFILE='boost_1_55_0'
LIBRARIES='shared_ptr bind lambda make_shared foreach function type_traits'

HEADER='----------------------'

function download
{
	echo $HEADER
	echo ">> Downloading Boost ${VERSION} from SourceForge"
	curl -O http://netcologne.dl.sourceforge.net/project/boost/boost/${VERSION}/${ZIPFILE}.zip
}

function extract_zip
{
	echo $HEADER
	echo ">> Extracting ${ZIPFILE}.zip"
	python utils/unzip.py ${ZIPFILE}.zip
}

function extract_libraries
{
	echo $HEADER
	echo ">> Extracting minimal header-only subset of Boost with BCP, including the following:"
	echo $LIBRARIES
	mkdir include
	utils/bcp.exe --boost=${ZIPFILE} ${LIBRARIES} include
	echo ">> Boost ${VERSION} successfully extracted to 'include'"
	echo ">> Open VS command prompt and run 'cl /Iinclude /EHsc utils/test.cpp' to test"
}

function cleanup
{
	echo $HEADER
	read -p ">> All done, shall I clean up downloaded and extracted files? [y/N] " answer
	if [[ "`echo $answer | tr [:upper:] [:lower:]`" == "y" ]]
	then
		echo -n "Removing '${ZIPFILE}.zip' and '${ZIPFILE}' (will take a while)... "
		rm -r "${ZIPFILE}.zip" "${ZIPFILE}"
		echo "done"
	fi
}

[[ -f "${ZIPFILE}.zip" ]] || download
[[ -d "${ZIPFILE}" ]] || extract_zip
[[ -d include ]] || extract_libraries
cleanup
