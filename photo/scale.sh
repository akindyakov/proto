#!/usr/bin/env bash

set -e -x

# arguments
input_dir="$1"
output_dir="${1}.product"
output_width=1024
#


rm -fr "$output_dir"
mkdir "$output_dir"

find "$input_dir" -iname '*.jp*g' \
    | while read fname; do
        width="$(file "$fname" | grep -o ', [0-9]\{2,5\}x[0-9]\{2,5\},' | sed -e 's/ //g' | sed -e 's/,//g'  | cut -dx -f1)"
        outname="$(basename "$fname").$(date +%s_%N).jpg"
        if [[ "$width" -ge "$output_width" ]]; then
            convert "${fname}" -scale "${output_width}" "${output_dir}/${outname}"
        else
            cp "${fname}" "${output_dir}/${outname}"
        fi
    done
