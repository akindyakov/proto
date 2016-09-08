#!/usr/bin/env bash

set -e -x

print_help() {
    echo "
Use smth to install:
  - gmail
  - magictwin
  - yateam
"
}

intall_general() {
    local all="$@"
    local accounts="$(echo "$all" | sed -e 's/ /, /g')"
    cat "general/offlineimaprc" | sed -e "s/#@ACCOUNTS/$accounts/" > "$HOME/.offlineimaprc"
    cp "general/msmtprc" "$HOME/.msmtprc"
    cp "general/mutt_bash_aliases" "$HOME/.mutt/bash_aliases"
}

install_account() {
    local what="$1"
    cp "${what}/muttrc" "$HOME/.mutt/muttrc.${what}"
    cat "${what}/msmtprc" >> "$HOME/.msmtprc"
    cat "${what}/offlineimaprc" >> "$HOME/.offlineimaprc"
}

if [[ -z "$@" ]]; then
    print_help
else
    for acc in $@; do
        [[ -d "$acc" ]]
    done
    intall_general "$@"
    for acc in $@; do
        install_account "$acc"
    done
fi
