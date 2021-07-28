#!/usr/bin/env bash
#
# Copyright (c) 2021 Koen Vervloesem
# SPDX-License-Identifier: MIT
#
### openhaypatch.sh - Patch an OpenHaystack firmware file with a new public key
###
### Usage:
###   ./openhaypatch.sh <file> <base64 key>
###
### Options:
###   file                     Firmware .bin file to patch
###   base64 key               New public key in Base64 format
###   -h                       Show this message.
help() {
    sed -rn 's/^### ?//;T;p' "$0"
}

# Check arguments
if [ "$#" -eq 0 ] || [ "$#" -eq 1 ] || [ "$1" = "-h" ]; then
    help
    exit 1
fi

bin_file=$1
base64_key=$2

# Check whether we have to use -d or -D for the base64 command
base64_decode="-D"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  base64_decode="-d"
fi

default_key_hex=$(echo -n 'OFFLINEFINDINGPUBLICKEYHERE!' | xxd -p)
new_key_hex=$(echo -n $base64_key | base64 $base64_decode | xxd -p)
echo "Patching firmware with key 0x$new_key_hex..."

firmware_bytes=$(xxd -p -c 1000000 < $bin_file)
echo "${firmware_bytes/$default_key_hex/$new_key_hex}" | xxd -r -p > $bin_file.$new_key_hex
