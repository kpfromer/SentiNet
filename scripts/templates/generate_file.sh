#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@varnsen)
# @file        : generate_file
# @created     : Monday Aug 12, 2019 14:45:31 MDT
#
# @description : Generates files from a given template
######################################################################

PATH_SENTINET="~/.sentinet"
FILE="local.vars"


get_name_from_file() {
  cat $1 | awk '{ s = ""; for (i = 2; i <= NF; i++) s = s $i " "; print s }'
}

input_name_to_file() {
  if [ -f "$PATH_SENTINET/$FILE" ]; then
    sed 's/name.*/name/' "$PATH_SENTINET/$FILE"
  fi
  if [ !-d "$PATH_SENTINET" ]
    mkdir ~/.sentinet
    touch ~/.sentinet/$FILE
  fi
  echo "name $@" > ~/.sentinet/$FILE
}


input_name_to_file theo
