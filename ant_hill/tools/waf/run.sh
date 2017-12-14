#!/usr/bin/env bash

set -x -e

install() {
    local version="waf-2.0.2"
    local dir_name="${version}"
    local archive_filename="${dir_name}.tar.bz2"
    curl --output "${archive_filename}" "https://waf.io/${archive_filename}"
    tar -xjvf "${archive_filename}"
    cd "${dir_name}"
    ./waf-light
    cp waf ../../
}

install
