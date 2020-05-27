#!/bin/bash
PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" # location of program.
DIR="2018" # default search directory
CAT_NAME="Categories" # default categories name
ASK_FOR_DIR="0" # !0=yes
REGEX_NAME='\[[^\[]*\]' # for [content] format
REGEX_FILE='[0-9]\{4\}\/[0-9]\{2\}\/[0-9]\{2\}' #for YYYY-MM-DD format

prase_files () {
    tmp=$name
    array=()
    while [[ $tmp =~ $REGEX_NAME ]]; do
        array+=("${BASH_REMATCH[0]:1:${#BASH_REMATCH[0]}-2}")
        tmp=${tmp:${#BASH_REMATCH[0]}:${#tmp}}
    done
}

build_dirs () {
    i=0
    n=${#array[@]}
    while [[ $i -lt $n ]]; do
        dest="${PWD}/${CAT_NAME}/${array[$i]}"
        if [ ! -d ${dest} ]; then
            mkdir ${dest}
        fi
        if [ ! -L "${dest}/${date}_${name}" ]; then
            ln -s ${path} "${dest}/${date}_${name}"
        else
            echo "Error: ${dest}/${date}_${name}  already exists" > /dev/null
        fi
        let i++
    done
}

if [[ $ASK_FOR_DIR -ne 0 ]]; then
    ls -p $PWD | grep /$s
    echo "Enter Directory to process:"
    read DIR
fi
if [ ! -d ${PWD}/${CAT_NAME} ]; then
    mkdir ${PWD}/${CAT_NAME}
fi
for path in `find "$PWD/$DIR" -type f`; do
    name=$(echo $path | sed 's#.*/##')
    date=$(echo $path | grep -o $REGEX_FILE | sed 's/\//-/g')
    prase_files
    build_dirs
done

