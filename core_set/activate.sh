#!/usr/bin/env sh

while read -r line; do
    LINE="$(eval echo $line)";
    export "$LINE";
done < "activate.sh.env"
