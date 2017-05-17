#!/usr/bin/env bash

set -e

find ./product/ -name '*_ut' \
    | while read ut; do
        "${ut}"
    done
