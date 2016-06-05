#!/usr/bin/env bash

set -e -x

# arguments
input_dir="$1"
output_dir="${1}.crop"
#output_width=1024
#


rm -fr "$output_dir"
mkdir "$output_dir"

find "$input_dir" -iname '*.jp*g' \
    | while read fname; do
        outname="$(basename "$fname").$(date +%s_%N).jpg"

        ratio="$(file "$fname" | grep -o ', [0-9]\{2,5\}x[0-9]\{2,5\},' | sed -e 's/ //g' | sed -e 's/,//g')"
        width="$(echo "${ratio}" | cut -dx -f1)"
        height="$(echo "${ratio}" | cut -dx -f2)"
        if [[ "$width" -ge "$height" ]]; then
            delta="$(( ($width - $height) / 2))"
            convert "${fname}" -crop "${height}x${height}+${delta}+0" "${output_dir}/${outname}"
        else
            delta="$(( ($height - $width) / 2))"
            convert "${fname}" -crop "${width}x${width}+0+${delta}" "${output_dir}/${outname}"
        fi
    done
