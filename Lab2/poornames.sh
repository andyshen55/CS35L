#! /bin/bash

#Basic poornames implementation
DOMAIN="."
ARGS=$#

#argument error handling
if (($ARGS > 1))
then
    echo "Error: 2 or more arguments given." >&2
    exit 1
fi

if (($ARGS == 1))
then
    DOMAIN=$1
    if [ "${DOMAIN:0:1}" == "-" ]
    then
	echo "Error: argument cannot start with '-' char." >&2
	exit 1
    fi
fi

if [ !  -d "${DOMAIN}" ]
then
    echo "Error: argument is not the name of a directory." >&2
    exit 1
fi

#local files=`find "$DOMAIN" -mindepth 1 -maxdepth 1 -type f -exec basename {} \;`

files=`find "$DOMAIN" -mindepth 1 -maxdepth 1 -type f -exec basename {} \;`
invalidNames=`echo "$files" | grep -E "^\.[a-zA-z_-]|^\.{2}[a-zA-z_-]|[^a-zA-Z\._-]|^-|[a-zA-Z\._-]{14,}"`
caseDuplicates=`echo "$files" | sort -f | uniq -iD`
#validPreCaseCheck=`echo "$files" | grep -Ev "^\.[a-zA-z_-]|^\.{2}[a-zA-z_-]|[^a-zA-Z\._-]|^-|[a-zA-Z\._-]{14,}"`
